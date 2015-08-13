//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_KALMAN_H__ED3D740F_01D2_4616_8B74_8BF57636F2C0__INCLUDED_)
#define AFX_KALMAN_H__ED3D740F_01D2_4616_8B74_8BF57636F2C0__INCLUDED_

#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000

#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

class kalman
{
public:
	void init_kalman(int x,int xv,int y,int yv);
	CvKalman* cvkalman;
	CvMat* state; 
	CvMat* process_noise;
	CvMat* measurement;
	const CvMat* prediction;
	CvPoint2D32f get_predict(float x, float y);
	kalman(int x=0,int xv=0,int y=0,int yv=0);
//virtual ~kalman();
};

#endif // !defined(AFX_KALMAN_H__ED3D740F_01D2_4616_8B74_8BF57636F2C0__INCLUDED_)
#define stateNum  4
#define measureNum 4

CvRandState rng;
const double T = 0.1;

void kalman::init_kalman(int x,int xv,int y,int yv)
{
    state->data.fl[0]=x;
    state->data.fl[1]=xv;
    state->data.fl[2]=y;
    state->data.fl[3]=yv;
    cvkalman->state_post->data.fl[0]=x;
    cvkalman->state_post->data.fl[1]=xv;
    cvkalman->state_post->data.fl[2]=y;
    cvkalman->state_post->data.fl[3]=yv;
}

kalman::kalman(int x,int xv,int y,int yv)
{     
    cvkalman = cvCreateKalman(stateNum,measureNum,0);
    state = cvCreateMat(stateNum, 1, CV_32FC1 );
    process_noise = cvCreateMat(stateNum, 1, CV_32FC1 );
    measurement = cvCreateMat(stateNum, 1, CV_32FC1 );
    int code = -1;
    /* create matrix data */
     const float A[stateNum][stateNum]={ 
  	 1, 1, 0, 0,
  	 0, 1, 0, 0,
  	 0, 0, 1, 1,
   	 0, 0, 0, 1
	};
     const float H[] ={ 
   	 1, 0, 0, 0,
   	 0, 0, 0, 0,
  	 0, 0, 1, 0,
  	 0, 0, 0, 0
	};
       //prediction covariance
     const float P[] = 
     	{
    	pow(320,2), pow(320,2)/T, 0, 0,
   	pow(320,2)/T, pow(320,2)/pow(T,2), 0, 0,
   	0, 0, pow(240,2), pow(240,2)/T,
   	0, 0, pow(240,2)/T, pow(240,2)/pow(T,2)
    	};
    	//system covariance
     const float Q[] = 
     	{
   	pow(T,3)/3, pow(T,2)/2,0, 0,
   	pow(T,2)/2, T, 0, 0,
   	0, 0, pow(T,3)/3, pow(T,2)/2,
   	0, 0, pow(T,2)/2, T
   	};
   
      const float R[] = 
	{
	  1, 0, 0, 0,
	  0, 0, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 0
 	 };
    
    cvRandInit( &rng, 0, 1, -1, CV_RAND_UNI );
    cvZero( measurement );
    cvRandSetRange( &rng, 0, 0.1, 0 );
    rng.disttype = CV_RAND_NORMAL;
    cvRand( &rng, state );
    memcpy( cvkalman->transition_matrix->data.fl, A, sizeof(A));
    memcpy( cvkalman->measurement_matrix->data.fl, H, sizeof(H));
    memcpy( cvkalman->process_noise_cov->data.fl, Q, sizeof(Q));
    memcpy( cvkalman->error_cov_post->data.fl, P, sizeof(P));
    memcpy( cvkalman->measurement_noise_cov->data.fl, R, sizeof(R));
    //cvSetIdentity( cvkalman->process_noise_cov, cvRealScalar(1e-5) );    
    //cvSetIdentity( cvkalman->error_cov_post, cvRealScalar(1));
    //cvSetIdentity( cvkalman->measurement_noise_cov, cvRealScalar(1e-1) );
    /* choose initial state */
    state->data.fl[0]=x;
    state->data.fl[1]=xv;
    state->data.fl[2]=y;
    state->data.fl[3]=yv;
    cvkalman->state_post->data.fl[0]=x;
    cvkalman->state_post->data.fl[1]=xv;
    cvkalman->state_post->data.fl[2]=y;
    cvkalman->state_post->data.fl[3]=yv;
    cvRandSetRange( &rng, 0, sqrt(cvkalman->process_noise_cov->data.fl[0]), 0 );
    cvRand( &rng, process_noise );

}
     
CvPoint2D32f kalman::get_predict(float x, float y)
{   
    /* update state with current position */
    state->data.fl[0]=x;
    state->data.fl[2]=y;
    
    /* predict point position */
    /* x'k=Aâ€ k+Bâ€ k
       P'k=Aâ€ k-1*AT + Q */
    cvRandSetRange( &rng, 0, sqrt(cvkalman->measurement_noise_cov->data.fl[0]), 0 );
    cvRand( &rng, measurement );
    
     /* xk=A?xk-1+B?uk+wk */
    cvMatMulAdd( cvkalman->transition_matrix, state, process_noise, cvkalman->state_post );
    
    /* zk=H?xk+vk */
    cvMatMulAdd( cvkalman->measurement_matrix, cvkalman->state_post, measurement, measurement );
    
    /* adjust Kalman filter state */
    /* Kk=P'kâ€ Tâ€?Hâ€ 'kâ€ T+R)-1
       xk=x'k+Kkâ€?zk-Hâ€ 'k)
       Pk=(I-Kkâ€ )â€ 'k */
    cvKalmanCorrect( cvkalman,measurement);
    float measured_value_x = measurement->data.fl[0];
    float measured_value_y = measurement->data.fl[2];
    
    const CvMat* prediction = cvKalmanPredict( cvkalman, 0 );
    float predict_value_x = prediction->data.fl[0];
    float predict_value_y = prediction->data.fl[2];
    return(cvPoint2D32f(predict_value_x,predict_value_y));
}

