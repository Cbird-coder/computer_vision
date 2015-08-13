#define LINUX_FILEOPT 
#include"picture.h"

extern "C"{
   #include"file.h"
}

#define WID  912
#define HIG  1140

IplImage *src;
IplImage *extendimg;
IplImage *dst;

char savepath[20]="./cutpicture";

int main(int argc,char **argv)
{
	int width=0,height=0;
	int exwidth=0,exheight=0,exchannels=0;
	int x_coordinate=0,y_coodinate=0;
	char savename[20]={0};
	char filename[40]={0};
	int flag=0;
	CvRect zone;

    if(argv[1]==NULL){
		printf("usage:program picture\n");
		return 0;
	}

     #ifdef LINUX_FILEOPT
        L_operation_dir(savepath);
	#else 
        W_operation_dir(savepath);
     #endif
	   src=cvLoadImage(argv[1],1);//1为读取彩色图,0为强制转化为灰度图，-1为默认读取图像的原通道数。
	   if(!src){
		   printf("can not find files,please check the path...!");
           return 0;
	   }
	   cvNamedWindow("source",1);
	   cvShowImage("source",src);
	   width=src->width;
	   height=src->height;

	   if(width%WID!=0)
		   exwidth=width+WID-width%WID;
	   else
		   exwidth=width;
	   if(height%HIG!=0)
		   exheight=height+HIG-height%HIG;
	   else
		   exheight=height;

	   /*图片扩展*/
	   //exchannels=src->nChannels;
	   exchannels=3;
	   extendimg=cvCreateImage(cvSize(exwidth,exheight),IPL_DEPTH_8U,exchannels);
	   cvCopyMakeBorder(src,extendimg,cvPoint(1,1),IPL_BORDER_CONSTANT,cvScalar(0,0,0));
	   printf("origal:%d*%d\nafter extend:%d*%d\nchannels:%d\n",width,height,exwidth,exheight,extendimg->nChannels);
       
       cvNamedWindow("externimg",1);
       cvShowImage("externimg",extendimg);
       /*图片切割*/
	   while(y_coodinate<exheight){

	            while(x_coordinate<exwidth){

	                  zone=cvRect(x_coordinate,y_coodinate,WID,HIG);
	                  cvSetImageROI(extendimg,zone);
	                  dst=cvCreateImage(cvSize(WID,HIG),IPL_DEPTH_8U,src->nChannels);

	                  cvCopy(extendimg,dst,0);

	                  cvResetImageROI(extendimg);
                      sprintf(savename,"img_%d.bmp",flag); 
					  sprintf(filename,"%s/%s",savepath,savename);
	    			  cvSaveImage(filename,dst);

	                  cvNamedWindow(savename,1);
	                  cvShowImage(savename,dst);
				      x_coordinate=x_coordinate+WID;
                      flag++;
	           }

		   y_coodinate=y_coodinate+HIG;
		   x_coordinate=0;

	   }

	cvWaitKey(0);
	cvReleaseImage(&src);
	cvReleaseImage(&extendimg);
	cvReleaseImage(&dst);

     #ifdef LINUX_FILEOPT
        L_display(savepath);
	#else 
        W_display(savepath);
     #endif

	return 0;
}
