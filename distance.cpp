extern struct record_line success[50];
extern CvPoint parallel_line[2];
extern float angle1;


int distance_park(IplImage *birds_img,int j)
{
    float den=0,den1=0;
    float x=0,y=0;
    
    //cvLine(birds_img,cvPoint(parallel_line[0].x,parallel_line[0].y),cvPoint(parallel_line[1].x,parallel_line[1].y), cvScalar(255,0,0),2, CV_AA, 0 );
    if(angle1==90)
       angle1=89.999;
    if(angle1==-90)
       angle1=-89.999;
    //-----------------------------------------------------------------------そ퐑ㅸ�{─1  m1x-y+d=0//そ퐑ㅸ�{─2  m2x-y+c=0  
    //printf("\nsuccess=%f\n%f\n",angle1,tan(angle1*3.141596/180));
    if(success[j].x2==success[j].x1)
    {
       den1=-tan(angle1*3.141596/180)*parallel_line[1].x+parallel_line[1].y;                                
       x=success[j].x2;
       y=tan(angle1*3.141596/180)*x+den1;                           
    }
    else
    {
    cvLine(birds_img,cvPoint(success[j].x1,success[j].y1),cvPoint(success[j].x2,success[j].y2), cvScalar(255,0,0),2, CV_AA, 0 );
    cvLine(birds_img,cvPoint(success[j+1].x1,success[j+1].y1),cvPoint(success[j+1].x2,success[j+1].y2), cvScalar(255,0,0),2, CV_AA, 0 );
    cvLine(birds_img,cvPoint(success[j+2].x1,success[j+2].y1),cvPoint(success[j+2].x2,success[j+2].y2), cvScalar(255,0,0),2, CV_AA, 0 );
    cvLine(birds_img,cvPoint(success[j+3].x1,success[j+3].y1),cvPoint(success[j+3].x2,success[j+3].y2), cvScalar(255,0,0),2, CV_AA, 0 );
    
       den=-((success[j].y2-success[j].y1)/(success[j].x2-success[j].x1))*success[j].x2+success[j].y2;                //---------------------d
       den1=-tan(angle1*3.141596/180)*parallel_line[0].x+parallel_line[0].y;                                          //---------------------c
       x=-(den-den1)/(((success[j].y2-success[j].y1)/(success[j].x2-success[j].x1))-tan(angle1*3.141596/180));        //--------------------x=-(d-c)/(m1-m2)
       y=x*tan(angle1*3.141596/180)+den1;
       cvLine(birds_img,cvPoint(x,y),cvPoint(x,y), cvScalar(255,50,200),8, CV_AA, 0 );
    
       float vertical_sin,horizontal_sin,vertical_cos,horizontal_cos,vertical_d,horizontal_d;// -sinx+cosy-d=0
       
       vertical_sin=0.769410;
       vertical_cos=0.638755;
       vertical_d=9.711984;
       horizontal_sin=0.613914;
       horizontal_cos=-0.789352;
       horizontal_d=-579.121399;
       
       float xx,yy,xx1,yy1;
       xx=(-vertical_d+vertical_cos*0)/vertical_sin;
       xx1=(-vertical_d+vertical_cos*480)/vertical_sin;   
       cvLine(birds_img,cvPoint(xx,0),cvPoint(xx1,480), cvScalar(0,255,0),2, CV_AA, 0 );
    
       xx=(-horizontal_d+vertical_cos*0)/vertical_sin;
       xx1=(-vertical_d+vertical_cos*480)/vertical_sin;   
       cvLine(birds_img,cvPoint(xx,0),cvPoint(xx1,480), cvScalar(0,255,0),2, CV_AA, 0 );
       
       printf("xx = %f yy = %f xx1 = %f yy1 = %f \n",xx,yy,xx1,yy1);
       

       float vertical_distance,horizontal_distance;
       vertical_distance=fabs(-vertical_sin*x+vertical_cos*y-vertical_d)/sqrt(vertical_sin*vertical_sin+vertical_cos*vertical_cos);
       horizontal_distance=fabs(-horizontal_sin*x+horizontal_cos*y-horizontal_d)/sqrt(horizontal_sin*horizontal_sin+horizontal_cos*horizontal_cos);
       //printf("vd = %f hd = %f \n",vertical_distance+110,horizontal_distance+68);
       return (int)(horizontal_distance+68)*1000+vertical_distance+110;
    
    }
    

}
