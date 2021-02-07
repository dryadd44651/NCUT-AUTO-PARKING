#define pi 3.1415926
#include "distance.cpp"

extern CvMemStorage* storage;
extern CvSeq* lines;
extern float angle1;
extern struct record_line success[50];
extern CvPoint parallel_line[2];
//extern short the_line;

float count_angle(CvPoint* line)
{
    if(fabs(line[1].x-line[0].x)==0)
        return 90.0;
    else
        return (float)asin((line[0].y-line[1].y)/sqrt((line[1].x-line[0].x)*(line[1].x-line[0].x)+(line[1].y-line[0].y)*(line[1].y-line[0].y)))*180/pi;
}

float distance_point2line(CvPoint point,CvPoint* line)
{
float b=0,slope=0,distance=0;
/*
printf("//////////////////////\n");
printf("point.x %d\n",point.x);
printf("point.y %d\n",point.y);

printf("line[0].x %d\n",line[0].x);
printf("line[0].y %d\n",line[0].y);
printf("line[1].x %d\n",line[1].x);
printf("line[1].y %d\n",line[1].y);
printf("//////////////////////\n");*/
   if((line[0].x-line[1].x)==0)
    {
        distance=line[0].x-line[0].x;
    }
    else
    {
        slope =(float) (line[1].y-line[0].y)/(line[1].x-line[0].x);
        b=-slope*line[0].x+line[0].y;
        distance=(float)sqrt((float)(slope*point.x-point.y+b)*(float)(slope*point.x-point.y+b))/(float)sqrt(slope*slope+1);
    }

return distance;
}
//==========================================================================================================================================================
void choose_line(IplImage *birds_img)
{
int row[4]={1000,1000,1000,1000};
CvPoint* line;
float vertical_angle=0,vertical_distance=0;
int line_count=0;
CvPoint vertical_point;
float parallel_angle=-angle1;
    for(int p = 0; p < lines->total; p++ )
    {
    line = (CvPoint*)cvGetSeqElem(lines,p);
    vertical_angle=count_angle(line);
    //printf("vertical_angle : %f\n",vertical_angle);

        cvLine(birds_img,cvPoint(parallel_line[0].x,parallel_line[0].y),cvPoint(parallel_line[1].x,parallel_line[1].y), cvScalar(0,0,255), 2, CV_AA, 0 ); 
        if(fabs(fabs(vertical_angle-parallel_angle)-90)<10)
        {//printf("vertical_angle   %f\n",vertical_angle);
         //printf("angle   %f\n",angle1);
         //printf("angle_parallel   %f\n",fabs(fabs(vertical_angle-parallel_angle)-90));
            if(line[0].y>line[1].y)
            {
                vertical_point.y = line[0].y;
                vertical_point.x = line[0].x;
            }
            else
            {
                vertical_point.y = line[1].y;
                vertical_point.x = line[1].x;
            }
            vertical_distance=distance_point2line(vertical_point,parallel_line);
            //printf("vertical_distance : %f\n",vertical_distance);

            if(vertical_distance<20)
            {
                float three_edge=sqrt((line[1].y-line[0].y)*(line[1].y-line[0].y)+(line[1].x-line[0].x)*(line[1].x-line[0].x));
                
                if(line[0].y<line[1].y)
                {
                    success[line_count].x1=line[0].x;
                    success[line_count].y1=line[0].y;
                    success[line_count].x2=line[1].x;
                    success[line_count].y2=line[1].y;
                }
                else
                {
                    success[line_count].x1=line[1].x;
                    success[line_count].y1=line[1].y;
                    success[line_count].x2=line[0].x;
                    success[line_count].y2=line[0].y;
                }
                
                
                success[line_count].sin=(float)((line[1].y-line[0].y)/three_edge);
                success[line_count].cos=(float)((line[1].x-line[0].x)/three_edge);
                
/*                
                if(success[line_count].x1+success[line_count].x2<row[1]+row[3])
                {
                row[0]=success[line_count].x1;
                row[1]=success[line_count].y1;
                row[2]=success[line_count].x2;
                row[3]=success[line_count].y2;
                }*/
                
                //cvLine(birds_img,cvPoint(line[0].x,line[0].y),cvPoint(line[1].x,line[1].y), cvScalar(255,0,0), 1, CV_AA, 0 ); 
                //cvShowImage("Birds_Eye", birds_img);
                //cvWaitKey(0);
                line_count++;
            }
        }//if(angle)
    }//for
line_count = Merger(success,line_count);
//cvLine(birds_img,cvPoint(row[0],row[1]),cvPoint(row[2],row[3]), cvScalar(0,0,255), 1, CV_AA, 0 ); 



struct record_line temp_success;
//直線方程式-sinx+cosy-d=0 
float d1,d2;

for (int i=0 ; i<line_count+1 ; i++)
    for (int j=i+1 ; j<line_count+1 ; j++)
    {
        if((-success[i].x1*success[i].sin+success[i].y1*success[i].cos)<(-success[j].x1*success[j].sin+success[j].y1*success[j].cos))
        //success[i].x1>success[j].x1)
        {
           //printf("%f\n",(-success[i].x1*success[i].sin+success[i].y1*success[i].cos)); 
            temp_success=success[i];
            success[i]=success[j];
            success[j]=temp_success;
        }
    }

CvPoint temp1[2];
CvPoint temp2[2];
CvPoint temp3[2];
printf("line_count  %d\n",line_count);
//for (int i=0 ; i<line_count+1 ; i++)
//{

    for (int j=0 ; j<line_count-3 ;j++)
    {
        if(line_count>=4)
        {
                temp1[0].x=(int)success[j+1].x1;
                temp1[0].y=(int)success[j+1].y1;
                temp1[1].x=(int)success[j+1].x2;
                temp1[1].y=(int)success[j+1].y2;
                
                temp2[0].x=(int)success[j+2].x1;
                temp2[0].y=(int)success[j+2].y1;
                temp2[1].x=(int)success[j+2].x2;
                temp2[1].y=(int)success[j+2].y2;
                
                temp3[0].x=(int)success[j+3].x1;
                temp3[0].y=(int)success[j+3].y1;
                temp3[1].x=(int)success[j+3].x2;
                temp3[1].y=(int)success[j+3].y2;
                
                //printf("\n i = %d  s.x = %d s.y = %d t1.x = %d t1.y = %d t2.x = %d t2.y = %d  \n",i,(int)success[i].x1,(int)success[i].y1,temp1[1].x,temp1[1].y,temp1[2].x,temp1[2].y);
                //printf("\n  distance :  %f\n",distance_point2line(cvPoint((int)success[j].x1,(int)success[j].y1),temp1));
                
                //cvLine(birds_img,cvPoint(success[j].x1,success[j].y1),cvPoint(success[j].x2,success[j].y2), cvScalar(255,0,0),2, CV_AA, 0 );
                
                
                if(fabs(distance_point2line(cvPoint((int)success[j].x1,(int)success[j].y1),temp1))<50)
                if(fabs(distance_point2line(cvPoint((int)success[j+1].x1,(int)success[j+1].y1),temp2))<200&&fabs(distance_point2line(cvPoint((int)success[j+1].x1,(int)success[j+1].y1),temp2))>90)
                if(fabs(distance_point2line(cvPoint((int)success[j+2].x1,(int)success[j+2].y1),temp3))<25)
                {
                //cvLine(birds_img,cvPoint(success[j].x1,success[j].y1),cvPoint(success[j].x2,success[j].y2), cvScalar(255,0,0),2, CV_AA, 0 );
                //cvLine(birds_img,cvPoint(temp1[0].x,temp1[0].y),cvPoint(temp1[1].x,temp1[1].y), cvScalar(0,255,0),2, CV_AA, 0 );
                //cvLine(birds_img,cvPoint(temp2[0].x,temp2[0].y),cvPoint(temp2[1].x,temp2[1].y), cvScalar(0,255,0),2, CV_AA, 0 );
                //cvLine(birds_img,cvPoint(temp3[0].x,temp3[0].y),cvPoint(temp3[1].x,temp3[1].y), cvScalar(0,255,0),2, CV_AA, 0 );
                printf("%f\n",fabs(distance_point2line(cvPoint((int)success[j].x1,(int)success[j].y1),temp1)));
                printf("%f\n",fabs(distance_point2line(cvPoint((int)success[j+1].x1,(int)success[j+1].y1),temp2)));
                printf("%f\n",fabs(distance_point2line(cvPoint((int)success[j+2].x1,(int)success[j+2].y1),temp3)));
                //1.704259   2.118371
                
                distance_park(birds_img,j);
                cvShowImage("Birds_Eye", birds_img);
                cvWaitKey(0);
                }
                
                //printf("*****************\n");
        }
   }
//=============
//cvLine(birds_img,cvPoint(success[i].x1,success[i].y1),cvPoint(success[i].x2,success[i].y2), cvScalar(255,0,0),2, CV_AA, 0 );
//cvShowImage("Birds_Eye", birds_img);
//cvWaitKey(0);
//}

//printf("%d\n",lines->total);
//printf("%d\n",line_count);
}//choose_line
