#include <cv.h>
#include <highgui.h>
#include "struct.h"
 
//extern struct record_line line_data;
int Merger(struct record_line *line_data,int line_data_point)
{
float d,distance,distance1;
short count=line_data_point;
for (int i=0 ; i<line_data_point ; i++)
   {  
      d = -line_data[i].sin*line_data[i].x1 +line_data[i].cos*line_data[i].y1;//直線方程式-sinx+cosy-d=0 
      for (int j=i+1 ; j<line_data_point ; j++)
      {
         if(fabs(line_data[i].angle-line_data[j].angle)<5&&line_data[i].x1!=999)
         {
            distance=fabs((float)(-line_data[i].sin*line_data[j].x1+line_data[i].cos*line_data[j].y1-d)/sqrt(line_data[i].sin*line_data[i].sin+line_data[i].cos*line_data[i].cos));
            distance1=fabs((float)(-line_data[i].sin*line_data[j].x2+line_data[i].cos*line_data[j].y2-d)/sqrt(line_data[i].sin*line_data[i].sin+line_data[i].cos*line_data[i].cos)); 
            
            //printf("%f %f\n",-line_data[i].sin,line_data[j].x1+line_data[i].cos*line_data[j].y1);
            if((distance+distance1)<20)//===============================dis1+dis2小於range內   做線段合併動作 
            {count--;
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
               line_data[j].x2=999;
            }//if(distance)
         }//if(fabs)
      }//for line_data_point j
   }//for line_data_point i

//for (int i=0 ; i<count ; i++)
//cvLine(birds_img,cvPoint(line_data[i].x1,line_data[i].y1),cvPoint(line_data[i].x2,line_data[i].y2), cvScalar(255,0,0),2, CV_AA, 0 );


//printf("count : %d\n",count);
return count;
}//Merger
