#include <cv.h>
#include <highgui.h>
#include <math.h>
#include "Merger.h"
#include "struct.h"

int main()
{  
   struct record_line line_data[50]={0};
   char k='o';
   short board_w=4;             //定義棋盤格
   short board_h=3;
   short line_data_point=0;  //struct record_line用指標 
//================================================================================   
   float sin_1,cos_1,three_edge;
   float angle1;
   float d,d1,distance,distance1;
   short swap;
//================================================================================   
   IplImage * img ;
   IplImage *pImg2=NULL; //抓取灰階視訊
 //  IplImage *img1;
   CvMemStorage* storage = cvCreateMemStorage(0);
   CvSeq* lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
   CvCapture * cap = cvCreateCameraCapture(0);
    
   cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_WIDTH,640);
   cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_HEIGHT,480);
   cvSetCaptureProperty( cap, CV_CAP_PROP_FPS, 30);   
   cvSetCaptureProperty( cap,  CV_CAP_PROP_FRAME_COUNT, 10);
   
   CvMat * H =H=(CvMat*)cvLoad("invertible_test_H.xml");  //讀取鳥瞰圖 H 矩陣
/*
   CvPoint2D32f objPts[4],imgPts[4];   //鳥瞰八點 
   int w=120,h=59;

   objPts[0] = cvPoint2D32f(0+w,0+h);                 //鳥瞰圖參數設定   轉換來源
   objPts[1] = cvPoint2D32f(board_w*8+w,0+h);
   objPts[2] = cvPoint2D32f(0+w,board_h*8+h);
   objPts[3] = cvPoint2D32f(board_w*8+w,board_h*8+h);
   imgPts[0] = cvPoint2D32f(207,369);//185,263        //鳥瞰圖參數設定   轉換目標
   imgPts[1] = cvPoint2D32f(403,369);//510,267
   imgPts[2] = cvPoint2D32f(179,479);//101,474
   imgPts[3] = cvPoint2D32f(427,479);//639,469

cvGetPerspectiveTransform(objPts,imgPts,H); //計算透視變換矩陣 
*/
while(1)
{
   img=cvQueryFrame(cap);
   if (img==NULL)
   {
       printf("輸入資料有問題請檢察");
       break;
   } 
      
   pImg2 = cvCreateImage(cvGetSize(img), 8, 1);
   IplImage *birds_img = cvCreateImage(cvGetSize(img), 8, 3);
   cvWarpPerspective(img,birds_img,H,CV_INTER_LINEAR|CV_WARP_FILL_OUTLIERS);    //webcam鳥瞰圖變形
   //圖片初步處理 
   cvCvtColor( birds_img, pImg2, CV_BGR2GRAY );            
   cvDilate( pImg2, pImg2, 0, 1 );//膨脹
   cvErode( pImg2, pImg2,0, 1 );//侵蝕
   cvSmooth(pImg2,pImg2,CV_MEDIAN,1,0);
   cvCanny(pImg2, pImg2, 50,200, 3);
         
//=================================================================================================測試影像!! 
  // img1 = cvCreateImage(cvGetSize(img), 8, 3);
  // cvCvtColor(  birds_img,img1, CV_GRAY2BGR ); 
//================================================================================================= 把符合的線存入STRUCT     
   lines = cvHoughLines2( pImg2, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 70, 120, 20 );  //霍夫line 
   
   for(short i=0; i < lines->total; i++ )  //計算畫面中所有線的角度 
   {
         
      CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);   
   //   cvLine( birds_img, line[0], line[1], cvScalar(0,200,200), 1, CV_AA, 0 );
  //  printf("第1點 : %3d   %3d  第2點 : %3d  %3d\n",line[0].x,l0ine[0].y,line[1].x,line[1].y);
      three_edge=sqrt((line[1].y-line[0].y)*(line[1].y-line[0].y)+(line[1].x-line[0].x)*(line[1].x-line[0].x));
      sin_1=(float)((line[1].y-line[0].y)/three_edge);
      cos_1=(float)((line[1].x-line[0].x)/three_edge);
      angle1=asin(sin_1)*180/3.14159265;
    //  printf("%f\t%f\n",sin_1,angle1);
      
      if (fabs(angle1)<45&&angle1>0)    //角度於正負45度角內直線存入struct內，並判斷兩點Y值低的存入X1 
      { 
         if(line[0].y>line[1].y)
         {
             line_data[line_data_point].x1=line[1].x;
             line_data[line_data_point].x2=line[0].x;
             line_data[line_data_point].y1=line[1].y;
             line_data[line_data_point].y2=line[0].y;
             line_data[line_data_point].angle=angle1;
             line_data[line_data_point].sin=sin_1;   
             line_data[line_data_point].cos=cos_1;  
         }
         else
         {
             line_data[line_data_point].x1=line[0].x;
             line_data[line_data_point].x2=line[1].x;
             line_data[line_data_point].y1=line[0].y;
             line_data[line_data_point].y2=line[1].y;
             line_data[line_data_point].angle=angle1;
             line_data[line_data_point].sin=sin_1;   
             line_data[line_data_point].cos=cos_1;  
         }
      //   printf("第1點 : %3d   %3d    第2點 : %3d  %3d 斜率:%.5f \n" , line_data[line_data_point].x1, line_data[line_data_point].y1 , line_data[line_data_point].x2,line_data[line_data_point].y2,line_data[line_data_point].angle);
         line_data_point++;
      }
   }
      
//---------------------------------------------------------縫線判斷  
  Merger(&line_data[0],line_data_point);
// cvWaitKey(0);
 /*  for (int i=0 ; i<line_data_point ; i++)
   {  
      d = -line_data[i].sin*line_data[i].x1 +line_data[i].cos*line_data[i].y1;//直線方程式-sinx+cosy-d=0 
      for (int j=i+1 ; j<line_data_point ; j++)
      {
         if(fabs(line_data[i].angle-line_data[j].angle)<5&&line_data[i].x1!=999)
         {            
            distance=fabs((-line_data[i].sin*line_data[j].x1+line_data[i].cos*line_data[j].y1-d)/sqrt(line_data[i].sin*line_data[i].sin+line_data[i].cos*line_data[i].cos));
            distance1=fabs((-line_data[i].sin*line_data[j].x2+line_data[i].cos*line_data[j].y2-d)/sqrt(line_data[i].sin*line_data[i].sin+line_data[i].cos*line_data[i].cos)); 
            if((distance+distance1)<20)//===============================dis1+dis2小於range內   做線段合併動作 
            {
               if(line_data[i].y1<line_data[j].y1)
               {
                  if(line_data[i].y2<line_data[j].y2)
                  {
                     line_data[i].y2=line_data[j].y2;
                     line_data[i].x2=line_data[j].x2;
                  }
               }
               else
               {
                  line_data[i].y1=line_data[j].y1;
                  line_data[i].x1=line_data[j].x1;
                  if(line_data[i].y2<line_data[j].y2)
                  {
                     line_data[i].y2=line_data[j].y2;
                     line_data[i].x2=line_data[j].x2;
                  }
               }
               line_data[j].x1=999;
            }
         }           
      }
   }*/
//==============================================================車道線判斷   
	unsigned short statistics=0,Cb,Cr,Cg,count_x,count_y;
    for (int i=0 ; i<line_data_point ; i++)
   {  
      d = -line_data[i].sin*line_data[i].x1 +line_data[i].cos*line_data[i].y1;
      if(line_data[i].x1!=999)
      for (int j=i+1 ; j<line_data_point ; j++)
      {//cvLine( img1, cvPoint(line_data[i].x1,line_data[i].y1),cvPoint(line_data[i].x2,line_data[i].y2), cvScalar(255,0,0), 1, CV_AA, 0 );
         if(fabs(line_data[i].angle-line_data[j].angle)<2 && line_data[j].x1!=999)
         {    
            distance=fabs((-line_data[i].sin*line_data[j].x2+line_data[i].cos*line_data[j].y2-d)/sqrt(line_data[i].sin*line_data[i].sin+line_data[i].cos*line_data[i].cos));
            if(distance<35&&distance>20)
            {  
               d = -line_data[i].sin*line_data[i].x1 +line_data[i].cos*line_data[i].y1;
               d1= -line_data[j].sin*line_data[j].x1 +line_data[j].cos*line_data[j].y1;
               d=(d+d1)/2;
               for(int c=0;c<30;c++)
               {
                   count_y=(line_data[i].y1+line_data[i].y2)/2;
                   count_x=(count_y*line_data[i].cos-d)/line_data[i].sin;
                  
                 //  cvLine( birds_img, cvPoint(count_x,count_y),cvPoint(count_x,count_y), cvScalar(0,250,0), 2, CV_AA, 0 );
                   
                   Cb = (unsigned char)birds_img -> imageData[count_x*3 + 0 + count_y*birds_img -> widthStep];
                   Cr = (unsigned char)birds_img -> imageData[count_x*3 + 1 + count_y*birds_img -> widthStep];
                   Cg = (unsigned char)birds_img -> imageData[count_x*3 + 2 + count_y*birds_img -> widthStep]; 
                   if(Cr>120 && Cr<Cg+50 && Cr<Cb+50 && Cr>Cg-50 && Cr>Cb-50 && Cb<Cg+50 && Cb<Cr+50 && Cb>Cg-50 && Cb>Cr-50 )
                   {
                      statistics++;
                   }
                   count_y++;
               }  
               if(statistics>10) 
               { 
              //printf("\n%d\n",statistics);                     
               cvLine( birds_img, cvPoint(line_data[i].x1,line_data[i].y1),cvPoint(line_data[i].x2,line_data[i].y2), cvScalar(0,0,250), 2, CV_AA, 0 );
               cvLine( birds_img, cvPoint(line_data[j].x1,line_data[j].y1),cvPoint(line_data[j].x2,line_data[j].y2), cvScalar(0,0,250), 2, CV_AA, 0 );
               }
            }//distance 
         }
      }   
   } 
//=============================================================== 

   line_data_point=0;
   cvShowImage("dst",birds_img);
   cvShowImage("dst1",pImg2);
//   cvWaitKey(0);
            
   k = cvWaitKey(10);   
   if (k == 27) break;        
   cvReleaseImage(&pImg2);
   cvReleaseImage(&birds_img); 
  // cvReleaseImage(&img1);    
   cvClearSeq(lines);
   
}
   
cvDestroyWindow("img");
cvDestroyWindow("pImg2");
cvDestroyWindow("dst");
cvReleaseCapture(&cap);

    return 0;    
}




