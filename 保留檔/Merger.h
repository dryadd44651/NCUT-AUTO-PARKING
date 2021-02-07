#include "struct.h"
void Merger(struct record_line *line_data,short line_data_point)
{
float d,distance,distance1;
for (int i=0 ; i<line_data_point ; i++)
   {  
      d = -line_data[i].sin*line_data[i].x1 +line_data[i].cos*line_data[i].y1;//直線方程式-sinx+cosy-d=0 
      for (int j=i+1 ; j<line_data_point ; j++)
      {
         if(fabs(line_data[i].angle-line_data[j].angle)<5&&line_data[i].x1!=999)
         { //printf("%f %f\n",distance1,distance);           
            distance=fabs((-line_data[i].sin*line_data[j].x1+line_data[i].cos*line_data[j].y1-d)/sqrt(line_data[i].sin*line_data[i].sin+line_data[i].cos*line_data[i].cos));
            distance1=fabs((-line_data[i].sin*line_data[j].x2+line_data[i].cos*line_data[j].y2-d)/sqrt(line_data[i].sin*line_data[i].sin+line_data[i].cos*line_data[i].cos)); 
            if((distance+distance1)<30)//===============================dis1+dis2小於range內   做線段合併動作 
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
   }
  // return 0;
}
