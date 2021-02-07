#include <cv.h>
#include <highgui.h>
#include <math.h>
#include "time.h"
#include "produce_h.h"
#include "capinit.h"

CvPoint2D32f p[2];

CvCapture *cap = cvCreateCameraCapture(0);

int main()
{
 int key=0;
 IplImage * img= NULL;
 IplImage *birds_img=NULL;
 CvMat * H =cvCreateMat(3,3,CV_32F);
 IplImage * img1 = cvCreateImage( cvSize(640,480), 1, 1);
 IplImage * gray = cvCreateImage( cvSize(640,480), 8, 1);
 capinit();
 produce_h(img);
 H=(CvMat*)cvLoad("invertible_test_H.xml");
 
  while(key!=27)
  {
  img = cvQueryFrame(cap);
  birds_img=cvCloneImage(img);
  cvWarpPerspective(img,birds_img,H,CV_INTER_LINEAR|CV_WARP_FILL_OUTLIERS);//CV_WARP_INVERSE_MAP
  
  
 
 // cvCircle(birds_img,cvPointFrom32f(p[0]),18,CV_RGB(255,0,255),4);
  cvCvtColor( img, gray, CV_BGR2GRAY );
  cvCanny(gray, gray, 50,255, 3);
  cvThreshold( gray, gray, 100,255, CV_THRESH_BINARY );
  /*
  for(int i=0;i<640;i++)
  for(int j=0;j<480;j++)
  ((uchar *)(img1->imageData + i*img1->widthStep))[j*img1->nChannels]=1;//((uchar *)(gray->imageData + i*gray->widthStep))[j*gray->nChannels + 0];
  key=cvWaitKey(0);
  
  cvShowImage("img111",img1);
  printf("%d\n",img->widthStep);
  */
  cvShowImage("img",gray);
  cvShowImage("birds_img",birds_img);
  key=cvWaitKey(10);
  cvReleaseImage(&birds_img);  
  }
  cvReleaseImage(&img);  
  cvReleaseImage(&birds_img);
  cvDestroyWindow("birds_img");
  cvReleaseCapture(&cap);
  return 0;
}
