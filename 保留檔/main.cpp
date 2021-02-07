#include <cv.h>
#include <highgui.h>
#include <math.h>
#include "Merger.h"
#include "struct.h"
#include "hough.h"


   CvMemStorage* storage = cvCreateMemStorage(0);
   CvSeq* lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
   
IplImage *main_img,*button,*button_pressed,* img;
bool enable_img=0; 
short part=0;
int main()
{
   CvCapture * cap_1 = cvCreateCameraCapture(0);  
   CvCapture * cap_2 = cvCreateCameraCapture(1);  
   IplImage * img ;
   IplImage *pImg2=NULL; //抓取灰階視訊
   int k;
   if(part==0)
      CvCapture * cap = cvCreateCameraCapture(0);
   else
      CvCapture * cap = cvCreateCameraCapture(1);
 
   cvSetCaptureProperty( cap_1, CV_CAP_PROP_FRAME_WIDTH,640);
   cvSetCaptureProperty( cap_1, CV_CAP_PROP_FRAME_HEIGHT,480);
   cvSetCaptureProperty( cap_1, CV_CAP_PROP_FPS, 30);   
   cvSetCaptureProperty( cap_1,  CV_CAP_PROP_FRAME_COUNT, 10);
   
   cvSetCaptureProperty( cap_2, CV_CAP_PROP_FRAME_WIDTH,640);
   cvSetCaptureProperty( cap_2, CV_CAP_PROP_FRAME_HEIGHT,480);
   cvSetCaptureProperty( cap_2, CV_CAP_PROP_FPS, 30);   
   cvSetCaptureProperty( cap_2,  CV_CAP_PROP_FRAME_COUNT, 10);
   CvMat * H =H=(CvMat*)cvLoad("invertible_test_H.xml");  //讀取鳥瞰圖 H 矩陣
   
   while(1)
   {
      if(part == 0)
         img = cvQueryFrame(cap_1);
      else
         img = cvQueryFrame(cap_2);
      struct record_line line_data[50]={0};
          
      pImg2 = cvCreateImage(cvGetSize(img), 8, 1);
      IplImage *birds_img = cvCreateImage(cvGetSize(img), 8, 3);
      cvWarpPerspective(img,birds_img,H,CV_INTER_LINEAR|CV_WARP_FILL_OUTLIERS);    //webcam鳥瞰圖變形
      cvCvtColor( birds_img, pImg2, CV_BGR2GRAY );            
      cvDilate( pImg2, pImg2, 0, 1 );//膨脹
      cvErode( pImg2, pImg2,0, 1 );//侵蝕
      cvSmooth(pImg2,pImg2,CV_MEDIAN,1,0);
      cvCanny(pImg2, pImg2, 50,200, 3);
//=============================================================================      
      parallel(&line_data[0],pImg2,birds_img);
//=============================================================================      
      cvShowImage("img",birds_img);
      k = cvWaitKey(10);   
      cvReleaseImage(&pImg2);
      cvReleaseImage(&birds_img); 
      if (k == 27) break;        
   }
   cvDestroyWindow("img");
   if(part==0)
   cvReleaseCapture(&cap_1);
   else
   cvReleaseCapture(&cap_2);
   return 0;
}
