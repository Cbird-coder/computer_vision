#include"picture.h"
#include"opencv2/photo/photo_c.h"
#include"opencv2/photo/photo.hpp"

int main(int argc,char **argv)
{
    IplImage *src,*extendimg;

	IplImage *mask,*fixed;

	int width=0,height=0;
	int exwidth=0,exheight=0;
	CvRect zone;

    if(argv[1]==NULL){
		printf("usage:program picture\n");
		return 0;
	}

	   src=cvLoadImage(argv[1],-1);//1为读取彩色图,0为强制转化为灰度图，-1为默认读取图像的原通道数。
	   if(!src){
		   printf("can not find files,please check the path...!");
           return 0;
	   }
	   cvNamedWindow("source",1);
	   cvShowImage("source",src);
	   width=src->width;
	   height=src->height;

	   exwidth=1*width;
	   exheight=1*height;

	   extendimg=cvCreateImage(cvSize(exwidth,exheight),IPL_DEPTH_8U,3);
	   cvResize(src,extendimg,CV_INTER_CUBIC);

       fixed=cvCloneImage(extendimg);
	   mask=cvCreateImage(cvGetSize(extendimg),8,1);
	   cvZero(mask);
	   cvZero(fixed);
       
	   cvCvtColor(extendimg,mask,CV_RGB2GRAY);
	   cvThreshold(mask,mask,230,255,CV_THRESH_BINARY);

	   cvInpaint(extendimg,mask,fixed,3.0,CV_INPAINT_TELEA);
	   cvSaveImage("fixed.jpg",fixed);

	  cvNamedWindow("fixed",1);
	  cvShowImage("fixed",fixed);

	cvWaitKey(0);
	cvReleaseImage(&src);
	cvReleaseImage(&extendimg);
	cvReleaseImage(&mask);
	cvReleaseImage(&fixed);
	return 0;
}
