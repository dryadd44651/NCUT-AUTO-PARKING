#include "GetPerspectiveTransformPoint.h"
#include "invertible_matrix.h"
extern CvCapture *cap;
extern CvPoint2D32f p[2];
void produce_h(IplImage *img)
{
 img = cvQueryFrame(cap);

 
 int board_w=8;
 int board_h=6;
 int board_n=board_h*board_w;
 CvSize board_sz=cvSize(board_w,board_h);
 CvMat * H =cvCreateMat(3,3,CV_32F);
  
 IplImage * gray_img=NULL;
 gray_img=cvCreateImage(cvGetSize(img),8,1);
 cvCvtColor(img,gray_img,CV_BGR2GRAY);

 CvPoint2D32f * corners=new CvPoint2D32f[board_n];
 int corner_count=0;

 int found=cvFindChessboardCorners(gray_img,board_sz,corners,&corner_count,CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_FILTER_QUADS);


 while(!found)
 {
     printf("fail finding the corner !\n");
     img = cvQueryFrame(cap);
     cvCvtColor(img,gray_img,CV_BGR2GRAY);
     found=cvFindChessboardCorners(gray_img,board_sz,corners,&corner_count,CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_FILTER_QUADS);
 }


 
 

while(!((corners[0].y-corners[board_w-1].y)>-0.1 && (corners[0].y-corners[board_w-1].y)<0.1))
{
     printf("adjust your chessboard to parallel\n");
     printf("left.y = %f   right.y = %f\n\n",corners[0].y,corners[board_w-1].y);
     img = cvQueryFrame(cap);
     cvCvtColor(img,gray_img,CV_BGR2GRAY);
     found=cvFindChessboardCorners(gray_img,board_sz,corners,&corner_count,CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_FILTER_QUADS);
     cvDrawChessboardCorners(img, board_sz, corners, corner_count, found);
     cvShowImage( "chessboard", img );
     cvWaitKey(10);
}


//cvFindCornerSubPix(gray_img,corners,corner_count,cvSize(11,11),cvSize(-1,-1),cvTermCriteria(CV_TERMCRIT_EPS|CV_TERMCRIT_ITER,30,0.1));
//精確角點位置 



 CvPoint2D32f objPts[4], imgPts[4];
 
 imgPts[0] = corners[0];
 imgPts[1] = corners[board_w-1];
 imgPts[2] = corners[(board_h-1)*board_w];
 imgPts[3] = corners[(board_h-1)*board_w + board_w-1];
 
float m1=fabs((imgPts[1].y-imgPts[3].y+30)/(imgPts[1].x-imgPts[3].x));
float m2=fabs((imgPts[0].y-imgPts[2].y+30)/(imgPts[0].x-imgPts[2].x));


CV_MAT_ELEM(*H,float,0,0)=1;
CV_MAT_ELEM(*H,float,0,1)=(float)(1/m1)*((1/(1-(1/m1+1/m2))))*640/480;
CV_MAT_ELEM(*H,float,0,2)=0;

CV_MAT_ELEM(*H,float,1,0)=0;
CV_MAT_ELEM(*H,float,1,1)=(float)1+((1/(1-(1/m1+1/m2)))-1);
CV_MAT_ELEM(*H,float,1,2)=0;

CV_MAT_ELEM(*H,float,2,0)=0;
CV_MAT_ELEM(*H,float,2,1)=(float)((1/(1-(1/m1+1/m2)))-1)/480;
CV_MAT_ELEM(*H,float,2,2)=1;
/*

 int adjust=0,down_shift=imgPts[0].y,right_shift=imgPts[0].x;
 adjust=sqrt((imgPts[0].x-imgPts[1].x)*(imgPts[0].x-imgPts[1].x)+(imgPts[0].y-imgPts[1].y)*(imgPts[0].y-imgPts[1].y));
 adjust=adjust/board_w/2;
 
 objPts[0].x = 0+right_shift;         objPts[0].y = 0+down_shift; 
 objPts[1].x = board_w*adjust+right_shift; objPts[1].y = 0+down_shift; 
 objPts[2].x = 0+right_shift;         objPts[2].y = board_h*adjust+down_shift;
 objPts[3].x = board_w*adjust+right_shift; objPts[3].y = board_h*adjust+down_shift; 
 */
 
 cvCircle(img,cvPointFrom32f(imgPts[0]),9,CV_RGB(0,0,255),3);
 cvCircle(img,cvPointFrom32f(imgPts[1]),9,CV_RGB(0,255,0),3);
 cvCircle(img,cvPointFrom32f(imgPts[2]),9,CV_RGB(255,0,0),3);
 cvCircle(img,cvPointFrom32f(imgPts[3]),9,CV_RGB(255,255,0),3);//cvPointFrom32f from f to int

 //DRAW THE FOUND CHECKERBOARD
 
 cvShowImage( "chessboard", img );
// cvGetPerspectiveTransform(objPts,imgPts,H);
 

/*

      CV_MAT_ELEM(*H,float,1,0)=0;//造成 y 軸傾斜的兩個元素 
      CV_MAT_ELEM(*H,float,2,0)=0;
      
      
float ti;
int d;

      
      p[0]=GetPerspectiveTransformPoint(319,479,H);
      ti = count_ti(318,479,H);
      d=319-p[0].x;
      CV_MAT_ELEM(*H,float,0,2)+=d*ti;
      d=479-p[0].y;
      CV_MAT_ELEM(*H,float,1,2)+=d*ti;


      cvSave("test_H.xml",H); 
      invertible_matrix();
      H=(CvMat*)cvLoad("invertible_test_H.xml");      
      
*/
      float x_scale,y_scale;
      x_scale=scale(imgPts[0],imgPts[1],H);
      y_scale=scale(imgPts[0],imgPts[2],H);
      

      x_scale=28/x_scale;
      y_scale=39.5/y_scale;
      printf("%f  %f \n",x_scale,y_scale);
//39.5   28

cvSave("invertible_test_H.xml",H);
}
