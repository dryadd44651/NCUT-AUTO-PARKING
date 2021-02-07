
#include "struct.h"
extern CvMemStorage* storage;
extern CvSeq* lines;
int parallel(struct record_line *line_data,IplImage *pImg2,IplImage *birds_img)
{  
   char k='o';
   short line_data_point=0;  //struct record_line�Ϋ��� 
//================================================================================   
   float sin_1,cos_1,three_edge;
   float angle1;
   float d,d1,distance,distance1;
   short swap;
//================================================================================   

//================================================================================================= ��ŦX���u�s�JSTRUCT     
   lines = cvHoughLines2( pImg2, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 70, 120, 20 );  //�N��line 
   
   for(short i=0; i < lines->total; i++ )  //�p��e�����Ҧ��u������ 
   {
         
      CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);   
      //cvLine( birds_img, cvPoint(line[0].x,line[0].y),cvPoint(line[1].x,line[1].y), cvScalar(0,0,250), 1, CV_AA, 0 );
      three_edge=sqrt((line[1].y-line[0].y)*(line[1].y-line[0].y)+(line[1].x-line[0].x)*(line[1].x-line[0].x));
      sin_1=(float)((line[1].y-line[0].y)/three_edge);
      cos_1=(float)((line[1].x-line[0].x)/three_edge);
      angle1=asin(sin_1)*180/3.14159265;
      
      if (fabs(angle1)<45&&angle1>0)    //���ש󥿭t45�ר������u�s�Jstruct���A�çP�_���IY�ȧC���s�JX1 
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
      //   printf("��1�I : %3d   %3d    ��2�I : %3d  %3d �ײv:%.5f \n" , line_data[line_data_point].x1, line_data[line_data_point].y1 , line_data[line_data_point].x2,line_data[line_data_point].y2,line_data[line_data_point].angle);
         line_data_point++;
      }
   }
      
//---------------------------------------------------------�_�u�P�_  
  Merger(&line_data[0],line_data_point);
//==============================================================���D�u�P�_   
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
            if(distance<35&&distance>10)
            {  
               d = -line_data[i].sin*line_data[i].x1 +line_data[i].cos*line_data[i].y1;
               d1= -line_data[j].sin*line_data[j].x1 +line_data[j].cos*line_data[j].y1;
               d=(d+d1)/2;
               for(int c=0;c<30;c++)
               {
                   count_y=(line_data[i].y1+line_data[i].y2)/2;
                   count_x=(count_y*line_data[i].cos-d)/line_data[i].sin;
                  
                   
                   Cb = (unsigned char)birds_img -> imageData[count_x*3 + 0 + count_y*birds_img -> widthStep];
                   Cr = (unsigned char)birds_img -> imageData[count_x*3 + 1 + count_y*birds_img -> widthStep];
                   Cg = (unsigned char)birds_img -> imageData[count_x*3 + 2 + count_y*birds_img -> widthStep]; 
                   if(Cr>100 && Cr<Cg+50 && Cr<Cb+50 && Cr>Cg-50 && Cr>Cb-50 && Cb<Cg+50 && Cb<Cr+50 && Cb>Cg-50 && Cb>Cr-50 )
                   {
                      statistics++;
                   }
                  // cvLine( birds_img, cvPoint(count_x,count_y),cvPoint(count_x,count_y), cvScalar(0,250,0), 2, CV_AA, 0 );
                   count_y++;
               }  
               if(statistics>20) 
               { 
              //printf("\n%d\n",statistics);                     
               cvLine( birds_img, cvPoint(line_data[i].x1,line_data[i].y1),cvPoint(line_data[i].x2,line_data[i].y2), cvScalar(0,0,250), 2, CV_AA, 0 );
               cvLine( birds_img, cvPoint(line_data[j].x1,line_data[j].y1),cvPoint(line_data[j].x2,line_data[j].y2), cvScalar(0,0,250), 2, CV_AA, 0 );
               statistics=0;
               printf("%f\t",line_data[i].angle);
               }
            }//distance 
         }
      }   
   } 
//=============================================================== 

   line_data_point=0;
   cvClearMemStorage(storage);
   cvClearSeq(lines);
    return 9;    
}




