float count_ti(float x,float y,CvMat * H)
{
float ti = CV_MAT_ELEM(*H,float,2,0)*x+CV_MAT_ELEM(*H,float,2,1)*y+CV_MAT_ELEM(*H,float,2,2);
return ti;
}

CvPoint2D32f GetPerspectiveTransformPoint(float x,float y,CvMat * H)
{
CvPoint2D32f p;
float ti;
      ti = count_ti(x,y,H);
      p.x= (CV_MAT_ELEM(*H,float,0,0)*x+CV_MAT_ELEM(*H,float,0,1)*y+CV_MAT_ELEM(*H,float,0,2))/ti;
      p.y= (CV_MAT_ELEM(*H,float,1,0)*x+CV_MAT_ELEM(*H,float,1,1)*y+CV_MAT_ELEM(*H,float,1,2))/ti;
      
return p;
}


float scale(CvPoint2D32f p,CvPoint2D32f p1,CvMat * H)
{
     float scale = 0;
     p = GetPerspectiveTransformPoint(p.x,p.y,H);
     p1 = GetPerspectiveTransformPoint(p1.x,p1.y,H);
     scale=hypot((p.x-p1.x),(p.y-p1.y));
return scale;
}
