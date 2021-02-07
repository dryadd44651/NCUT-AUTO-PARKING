#include "cv.h"
#include "highgui.h"
#include <math.h>

float sin1,cos1,d;// -sinx+cosy-d=0
int x11,y11,x22,y22,ax,bx;
    IplImage *img ;
    IplImage *birds_img=NULL;
    
void on_mouse(int Event, int x, int y, int flags, void* param)
{
if(Event==CV_EVENT_LBUTTONDOWN)
    {
    printf("x = %d ,y = %d\n",x,y);
    x11=x;
    y11=y;
    }
if(Event==CV_EVENT_RBUTTONDOWN)
    {
    printf("x = %d ,y = %d\n",x,y);
    x22=x;
    y22=y;
    sin1=(y22-y11)/sqrt((y22-y11)*(y22-y11)+(x22-x11)*(x22-x11));
    cos1=(x22-x11)/sqrt((y22-y11)*(y22-y11)+(x22-x11)*(x22-x11));
    d=-sin1*x+cos1*y;
    printf("sin = %f ,cos = %f d = %f\n",sin1,cos1,d);
    ax=(cos1*0-d)/sin1;
    bx=(cos1*480-d)/sin1;
    }
}



int main()
{
    CvCapture *cap = cvCreateCameraCapture(0);

    char k;
    CvMat * H =cvCreateMat(3,3,CV_32F);
    H=(CvMat*)cvLoad("invertible_test_H.xml");
    cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_WIDTH, 640);  
    cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_HEIGHT, 480);
    cvSetCaptureProperty( cap, CV_CAP_PROP_FPS, 15);
    cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_COUNT, 10);
   
   while(k!=27)
   {
       
       img = cvQueryFrame(cap);
       birds_img=cvCloneImage(img);
       cvWarpPerspective(img,birds_img,H,CV_INTER_LINEAR|CV_WARP_FILL_OUTLIERS);
       cvLine(birds_img,cvPoint(x11,y11),cvPoint(x22,y22), cvScalar(255,0,0), 1, CV_AA, 0 ); 
       cvLine(birds_img,cvPoint(ax,0),cvPoint(bx,480), cvScalar(255,0,0), 1, CV_AA, 0 );
       cvShowImage("img",birds_img);
       k = cvWaitKey(33);
       cvSetMouseCallback("img",on_mouse,0);
       if(img==NULL)
       break;
   }
   cvDestroyWindow("img");
   cvReleaseCapture(&cap);

   return 0;    
}
