#include <conio.h>
#include <stdio.h>

void up_down_txt(int input){
   FILE *outptr;
   int i,j,*ptr;
   int c[6]={'2','3','6','5',' ','2'};
   int in[3];
   ptr=c;
   
   in[0]=input/100;
   in[1]=(input/10)%10;
   in[2]=input%10;
   
   outptr = fopen("8051/function1_ud.txt","w");
   if(outptr != NULL){
      for(j=0;j<6;j++){
         fputc(*(ptr+j),outptr);
      }
      for(j=0;j<3;j++){
         fputc(in[j]+0x30,outptr);
      }
      fclose(outptr);
   }
}


int txt_r(char *mem){
   int i=10000,j,out=0;
   
    for(j=1;j<6;j++,i/=10){
       out+=(*(mem+j)-48)*i;
    }
    return out;
}

int txt_r_f1_rf(int num){
   FILE *inptr;
   
   int i=10000,j,out=0;
   char data;
   inptr = fopen("8051/function1.txt","r");
   data=fgetc(inptr);
   inptr -> _ptr--;
   
   if(inptr != NULL){
      for(j=0;j<5;j++,i/=10){
         out+=(*((inptr -> _ptr)+j+num*7)-48)*i;
      }
      fclose(inptr);
   }
   return out;
}

int txt_r_f1_ud(int num,int count_flog){
   FILE *inptr;
   
   int i,j=100,out=0;
   char data;
   inptr = fopen("8051/function1_ud.txt","r");
   
   data=fgetc(inptr);
   inptr -> _ptr--;
   
   if(inptr != NULL){
      if(count_flog==0)
         out=(*((inptr -> _ptr)+num*5));
      else if(count_flog==1){
         for(i=1;i<4;i++){
            out+=(*((inptr -> _ptr)+i+num*5)-48)*j;
            j/=10;
         } 
      }
      fclose(inptr);
   }
   return out;
}
