#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace cv;

//给定圆心和周长，和圆周上的角度，求圆周上的坐标
static inline Point calcPoint(Point2f center, double R, double angle)
{
    return center + Point2f((float)cos(angle), (float)-sin(angle))*(float)R;
}


int main(int, char**)
{
    Mat img(800,800, CV_8UC3);
    //状态维数2，测量维数1，没有控制量
    KalmanFilter KF(2, 1, 0);
    Mat state(2, 1, CV_32F); /* (phi, delta_phi) */
    Mat processNoise(2, 1, CV_32F);
    Mat measurement = Mat::zeros(1, 1, CV_32F);
    char code = (char)-1;

    for(;;)
    {

        /*
        使用kalma步骤一
        kalman的初始化过程，一般在使用kalman这个类时需要初始化的值有：
        转移矩阵，测量矩阵，过程噪声协方差，测量噪声协方差，后验错误协方差矩阵，
        前一状态校正后的值，当前观察值
        */

        //产生均值为0，标准差0.1的二维高斯列向量
        randn( state, Scalar::all(0), Scalar::all(0.1) );
        //transitionMatrix为类KalmanFilter中的一个变量，Mat型，是Kalman模型中的状态转移矩阵
       //转移矩阵为[1,1;0,1],2*2维matrix
        KF.transitionMatrix = *(Mat_<float>(2, 2) << 1, 1, 0, 1);

       //函数setIdentity是给参数矩阵对角线赋相同值，默认对角线值值为1. A
        setIdentity(KF.measurementMatrix);
        //系统过程噪声方差矩阵  Q
        setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
        //测量过程噪声方差矩阵  R
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
         //后验错误估计协方差矩阵 
        setIdentity(KF.errorCovPost, Scalar::all(1));
        //statePost为校正状态，其本质就是前一时刻的状态
        randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));


        for(;;)
        {
            Point2f center(img.cols*0.5f, img.rows*0.5f);
            float R = img.cols/3.f;
            //state中存放起始角，state为初始状态
            double stateAngle = state.at<float>(0);
            Point statePt = calcPoint(center, R, stateAngle);
            /*
                使用kalma步骤二
                调用kalman这个类的predict方法得到状态的预测值矩阵
            */
            Mat prediction = KF.predict();
            double predictAngle = prediction.at<float>(0);
            Point predictPt = calcPoint(center, R, predictAngle);

            randn(measurement, Scalar::all(0), Scalar::all(KF.measurementNoiseCov.at<float>(0)));
            // generate measurement
            measurement += KF.measurementMatrix*state;

            double measAngle = measurement.at<float>(0);
            Point measPt = calcPoint(center, R, measAngle);

            // plot points
            #define drawCross(center,color,d)                                 \
                line(img,Point(center.x - d,center.y - d ),                \
                             Point(center.x + d,center.y + d ), color, 1, CV_AA, 0);\
                line(img,Point(center.x + d,center.y - d ),                \
                             Point(center.x - d, center.y + d ), color, 1, CV_AA, 0 )

            img = Scalar::all(0);

            drawCross( statePt, Scalar(255,255,255), 3 );//white
            drawCross( measPt, Scalar(0,0,255), 3 );//bule
            drawCross( predictPt, Scalar(0,255,0), 3 );//green

            line( img, statePt, measPt, Scalar(0,0,255), 3, CV_AA, 0 );//red,real and measure
            line( img, statePt, predictPt, Scalar(0,255,255), 3, CV_AA, 0 );//yellow,real and predict
            /*
                使用kalma步骤三
                调用kalman这个类的correct方法得到加入观察值校正后的状态变量值矩阵
            */
            if(theRNG().uniform(0,4) != 0)
                KF.correct(measurement);

            randn(processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));
            //不加噪声的话就是匀速圆周运动，加了点噪声类似匀速圆周运动，因为噪声的原因，运动方向可能会改变
            state = KF.transitionMatrix*state + processNoise;

            imshow( "Kalman", img );
            code = (char)waitKey(100);

            if( code > 0 )
                break;
        }
        if( code == 27 || code == 'q' || code == 'Q' )
            break;
    }

    return 0;
}
