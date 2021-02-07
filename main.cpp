#include <cv.h>
#include <highgui.h>
#include <math.h>
#include "Merger.h"
#include "choose_line.h"
#include "struct.h"
#include "hough.h"

//===========================�F�F 
#include "8051\rs232.c"
#include <conio.h>
#include <stdio.h>
#include <windows.h>

   struct record_line success[50];
   CvMemStorage* storage = cvCreateMemStorage(0);
   CvSeq* lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
   CvPoint parallel_line[2];
   //short the_line[4];
//IplImage *main_img,*button,*button_pressed,* img;
//==============================================================================
float angle1;
IplImage *main_img,*button,*button_pressed,* img;
bool enable_img=0; 
//---------------------------------------------------------------
//8int array[2];
//---------------------------------------------------------------
int part=0;//�����B�J 
/*int go_stop=0,count_0=0,error_count=0,stop_count=0;
int par(void){
   int i=0,j,input,end=24800,*ptr;
   int now;                         //����q�� 
   ptr=rs232_1();
   now = *(ptr);
   dis = *(ptr+1);
   
   int in=parallel();               //����O�_���� 
   if(auto_car == 1){
      if(count_0 == 1 && (now+100 > end && now-100 < end)){
         rs_putch(1, '4' ) ;
         rs_putch(1, '1' ) ;
         count_0 = 2;
      }
      if(in == -1){//�V���� 
         end = 22000;
         stop_count=0;
         error_count = 0;
      }
      else if(in == 0){//����� 
         end = 24800;
         if(count_0!=2)   
            cvWaitKey(300);
         if(count_0==0){
            rs_putch(1, '3' ) ;
            count_0 = 1;
         }
      }
      else if(in == 1){//�V�k�� 
         end = 28000;
         stop_count  = 0;
         error_count = 0;
      }
      else if(in == 2){//�䤣����
         error_count++;
         if(error_count == 10){
            rs_putch(1, '3' ) ;//����e�i 
            rs_putch(1, '4' ) ;//����k�� 
            auto_car = 2;
            return 99;
         }
      }
      if(dis>200){
         return 1;
      }
      if(now == end || auto_car == 0) 
         rs_putch(1, '4' ) ;
      else
         rs_putch(1, now > end? '6' : '5' ) ;//��V�L�����w�������� 
   }//�����Ĥ@�B�J�_�� 
   printf("in=%d\tnow=%d\tend=%d\tauto_car=%d\tdis=%d\tstop_count=%d\n",in,now,end,auto_car,dis,stop_count);
   return 0;
}


int adv_count=0,adv_x=0,adv_y=0;
int part2(void){
   int x,y,end=24800,*ptr;
   int now;              //����q�� 
   int in,part2_auto=0;  //����Z�� 
   
   in=parallel();
   ptr=rs232_1();
   now = *(ptr);
   dis = *(ptr+1);
   if(dis>15 && part2_auto==0){
      //rs_putch(1, '3' ) ;
      //rs_putch(1, '4' ) ;
      if(in!=2){
         adv_x+=in/1000;
         adv_y+=in%1000;
         adv_count++;
      }
      
      if(adv_count==2){
         adv_count=0;
         dis      =0;
         adv_x/=3;adv_y/=3;
         printf("%d\t%d\n",adv_x,adv_y);
         part2_auto=1;
      }
   }
   if(part2_auto==1){
      if(adv_x > 160 && adv_x < 170){
         rs_putch(1, '4' ) ;
         rs_putch(1, '3' ) ;
         up_down_txt(adv_x-40);
         printf("***********************");
         return 2;
      }
      else if(adv_x > 169){
         printf("��h\n");
         rs_putch(1, '2' ) ;
      }
      else{
         printf("�e�i\n");
         rs_putch(1, '1' ) ;
      }
   }
   
   
   
   
   //rs_putch(1, '3' ) ;
   //printf("dis=%d\tnow=%d\t",dis,now);
  // printf("dis=%d\t",dis); 
      
   if(now == end || auto_car == 0) 
      rs_putch(1, '4' ) ;
   else
      rs_putch(1, now > end? '6' : '5' ) ;//��V�L�����w�������� 
   
   return 1;
}
*/

//==============================================================================
int main()
{
//==============================================================================
 //  rs232_var_start();
 //  if(!rs_initialise(port ,9600, '8', 'N')) { getch(); exit(1); }//�}��RS232 
//==============================================================================    
   CvCapture * cap_1 = cvCreateCameraCapture(0);  
   CvCapture * cap_2 = cvCreateCameraCapture(1);  
   IplImage * img ;
   IplImage *pImg2=NULL; //����Ƕ����T
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
   
   CvMat * H =(CvMat*)cvLoad("invertible_test_H.xml");  //Ū�������� H �x�}
//==============================================================================
   dis = 0;
   angle1=0;
   while(1)
   {
      if(part == 0)
         img = cvQueryFrame(cap_1);
      else
         img = cvQueryFrame(cap_2);
      struct record_line line_data[50]={0};
          
      pImg2 = cvCreateImage(cvGetSize(img), 8, 1);
      IplImage *birds_img = cvCreateImage(cvGetSize(img), 8, 3);
      cvWarpPerspective(img,birds_img,H,CV_INTER_LINEAR|CV_WARP_FILL_OUTLIERS);    //webcam�������ܧ�
      cvCvtColor( birds_img, pImg2, CV_BGR2GRAY );            
      cvDilate( pImg2, pImg2, 0, 1 );//����
      cvErode( pImg2, pImg2,0, 1 );//�I�k
      cvSmooth(pImg2,pImg2,CV_MEDIAN,1,0);
      cvCanny(pImg2, pImg2, 50,200, 3);
//=============================================================================  
/* if(part==0)          //�����Ĥ@�B�J
         part=par();
      else if(part==1){    //�����ĤG�B�J
         part=part2();
      }
      else if(part==2)     //�����ĤT�B�J
         rs232_3();
      else if(part == 98){
         printf("\n\n�䤣�쨮��\n");
         part = 100;
      }
      else if(part == 99){
         printf("\n\n���հհհհ�\n");
         part = 100;
      }    */
      printf("%d\t",parallel(&line_data[0],pImg2,birds_img));
      choose_line(birds_img);
      
      printf("%f\n",angle1);
//=============================================================================       
      cvShowImage("canny",pImg2);     
      cvShowImage("img",birds_img);
      k = cvWaitKey(10);   
      cvReleaseImage(&pImg2);
      cvReleaseImage(&birds_img); 
      if (k == 27) break;        
   }
   cvDestroyWindow("canny");
   cvDestroyWindow("img");
//   if(part==0)
   cvReleaseCapture(&cap_1);
//   else
   cvReleaseCapture(&cap_2);
   return 0;
}
