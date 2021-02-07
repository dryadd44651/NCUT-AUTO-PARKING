#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>


void trmul(double *a,double *b,int m,int n,int k,double *c)//為了驗算矩陣相乘函數
  //int m,n,k;
  //double a[],b[],c[];
  {
int i,j,l,u;
    for (i=0; i<=m-1; i++)
    for (j=0; j<=k-1; j++)
      { u=i*k+j; c[u]=0.0;
        for (l=0; l<=n-1; l++)
          c[u]=c[u]+a[i*n+l]*b[l*k+j];
      }
    return;
  }
  int rinv(double *a,int n)//求逆(反)矩陣
  //int n;
  //double a[];
  { int *is,*js,i,j,k,l,u,v;
    double d,p;
    is=(int *)malloc(n*sizeof(int));
    js=(int *)malloc(n*sizeof(int));
    for (k=0; k<=n-1; k++)
      { d=0.0;
        for (i=k; i<=n-1; i++)
        for (j=k; j<=n-1; j++)
          { l=i*n+j; p=fabs(a[l]);
            if (p>d) { d=p; is[k]=i; js[k]=j;}
          }
        if (d+1.0==1.0)
          { free(is); free(js); printf("err**not inv\n");
            return(0);
          }
        if (is[k]!=k)
          for (j=0; j<=n-1; j++)
            { u=k*n+j; v=is[k]*n+j;
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
        if (js[k]!=k)
          for (i=0; i<=n-1; i++)
            { u=i*n+k; v=i*n+js[k];
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
        l=k*n+k;
        a[l]=1.0/a[l];
        for (j=0; j<=n-1; j++)
          if (j!=k)
            { u=k*n+j; a[u]=a[u]*a[l];}
        for (i=0; i<=n-1; i++)
          if (i!=k)
            for (j=0; j<=n-1; j++)
              if (j!=k)
                { u=i*n+j;
                  a[u]=a[u]-a[i*n+k]*a[k*n+j];
                }
        for (i=0; i<=n-1; i++)
          if (i!=k)
            { u=i*n+k; a[u]=-a[u]*a[l];}
      }
    for (k=n-1; k>=0; k--)
      { if (js[k]!=k)
          for (j=0; j<=n-1; j++)
            { u=k*n+j; v=js[k]*n+j;
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
        if (is[k]!=k)
          for (i=0; i<=n-1; i++)
            { u=i*n+k; v=i*n+is[k];
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
      }
    free(is); free(js);
    return(1);
  }

  void invertible_matrix()
  { int i,j;
int size=2;
double *pa,*pb,*pc;
    /*
    static double a[3][3]={ {1,0,2},
                            {2,1,1},
                            {1,1,1}};*/
    CvMat * H =cvCreateMat(3,3,CV_32F);
    CvMat * invertible_H =cvCreateMat(3,3,CV_32F);
    H=(CvMat*)cvLoad("test_H.xml");
    
    double a[3][3];
    for (i=0; i<=size; i++)
    for (j=0; j<=size; j++)
    a[i][j]=CV_MAT_ELEM(*H,float,i,j);
    
    static double b[4][4],c[4] [4];
pa=(double *)a;
pb=(double *)b;
pc=(double *)c;

    for (i=0; i<=size; i++)
    for (j=0; j<=size; j++)
      b[i][j]=a[i][j];
    i=rinv(pa,size+1);
    if (i!=0)
      { printf("MAT A IS:\n");
        for (i=0; i<=size; i++)
          { for (j=0; j<=size; j++)
              printf("%13.7e ",b[i][j]);
            printf("\n");
          }
        printf("\n");
        printf("MAT A- IS:\n");
        for (i=0; i<=size; i++)
          { for (j=0; j<=size; j++)
              printf("%13.7e ",a[i][j]);
            printf("\n");
          }
        printf("\n");
        printf("MAT AA- IS:\n");
        trmul(pb,pa,size+1,size+1,size+1,pc);
        for (i=0; i<=size; i++)
          { for (j=0; j<=size; j++)
              printf("%13.7e ",c[i][j]);
            printf("\n");
          }
      }
    for (i=0; i<=size; i++)
    for (j=0; j<=size; j++)
    CV_MAT_ELEM(*invertible_H,float,i,j)=a[i][j];

    cvSave("invertible_test_H.xml",invertible_H);
  }
