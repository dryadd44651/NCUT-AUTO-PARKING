#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include "txt.c"

HANDLE hCom;           //handle for serial port I/O
 
/*----------------------------------------------------------------------------*
 * Serial port: initialise io_port, set baud rate, set data bits, one stop bit*/
 
HANDLE rs_initialise (int io_port, const long int BaudRate, const char parity, const char data)
{
    BOOL bPortReady;
    DCB dcb;
    char ComPortName[]="COM1";           // set up COM port number in COM?
    ComPortName[3]='5'+io_port;
    hCom = CreateFile(ComPortName, GENERIC_READ | GENERIC_WRITE,
                                  0,            // exclusive access
                                  NULL,         // no security
                                  OPEN_EXISTING,
                                  0,            // no overlapped I/O
                                  NULL);        // null template
 
 
    if ((int)hCom <= 0) { printf("1serial port COM%d connect fail %s error %d\n\r", io_port, ComPortName, GetLastError()); return 0;   } 
    //else                printf(" serial port COM%d connect OK \n\r", io_port); 
 
    bPortReady = SetupComm(hCom, 2, 128); // set buffer sizes
    if (!bPortReady ) { printf("2serial port COM%d SetupComm fail  %d\n\r", io_port,  GetLastError()); return 0;   } 
    //else                printf(" serial port COM%d connect OK \n\r", io_port); 
 
    bPortReady = GetCommState(hCom, &dcb);
    if (!bPortReady ) { printf("3serial port COM%d  GetCommState fail  %d\n\r", io_port,  GetLastError()); return 0;   } 
  //  else                printf(" serial port COM%d connect OK \n\r", io_port); 
    dcb.BaudRate = BaudRate;
    if( data == '7') dcb.ByteSize = 7;
    else             dcb.ByteSize = 8;
    if( parity == 'E') dcb.Parity = EVENPARITY;
    if( parity == 'O') dcb.Parity = ODDPARITY;
    else               dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fAbortOnError = TRUE;
 
    // set XON/XOFF
    dcb.fOutX = FALSE;                       // XON/XOFF off for transmit
    dcb.fInX = FALSE;                        // XON/XOFF off for receive
    // set RTSCTS
    dcb.fOutxCtsFlow = FALSE;               // turn off CTS flow control
    dcb.fRtsControl = FALSE;                // RTS_CONTROL_HANDSHAKE; //
    // set DSRDTR
    dcb.fOutxDsrFlow = FALSE;               // turn off DSR flow control
    //dcb.fDtrControl = DTR_CONTROL_ENABLE; // DTR handshake
    dcb.fDtrControl = DTR_CONTROL_DISABLE;  //
    // dcb.fDtrControl = DTR_CONTROL_HANDSHAKE; //
 
    bPortReady = SetCommState(hCom, &dcb);
    if (!bPortReady ) { printf("4serial port COM%d  SetCommState fail  %d\n\r", io_port,  GetLastError()); return 0;   } 
 
    // Communication timeouts 
    COMMTIMEOUTS CommTimeouts;
    bPortReady = GetCommTimeouts (hCom, &CommTimeouts);
    CommTimeouts.ReadIntervalTimeout = 5 ;
    CommTimeouts.ReadTotalTimeoutConstant = 5 ;
    CommTimeouts.ReadTotalTimeoutMultiplier = 1 ;
    CommTimeouts.WriteTotalTimeoutConstant = 5 ;
    CommTimeouts.WriteTotalTimeoutMultiplier = 1 ;
    bPortReady = SetCommTimeouts (hCom, &CommTimeouts);
    if (!bPortReady ) { printf("5serial port COM%d SetCommTimeouts fail  %d\n\r", io_port,  GetLastError()); return 0;   } 
    else                printf("6serial port COM%d connect OK \n\r", io_port); 
    return hCom;
}
 
 
/*----------------------------------------------------------------------------*
 * Serial port: terminate io_port, sets DTR and RTS to low                     */
void rs_terminate(const int io_port)
{
  // Close(hCom);
}
 
 
 
/*----------------------------------------------------------------------------*
 * Serial port: read character from io_port (ignored in this version)         */
char rs_getch(const int io_port)
{
    char rxchar;
    BOOL bReadRC;
    static DWORD iBytesRead;
    bReadRC = ReadFile(hCom, &rxchar, 1, &iBytesRead, NULL);
    if (iBytesRead) return rxchar; else return 0;         // return 0 if no character read
}
 
/*----------------------------------------------------------------------------*
 * Serial port: transmit character to io_port                                 */
void rs_putch(const int io_port, const int txchar)
{
    BOOL bWriteRC;
    static DWORD iBytesWritten;
    bWriteRC = WriteFile(hCom, &txchar, 1, &iBytesWritten,NULL);
    return;
}
 
/*----------------------------------------------------------------------------*
 * Serial port: transmit a string of characters to io_port                    */
void rs_putstring(const int io_port, const char *string)
{
    while (*string != '\0')
        rs_putch(io_port, *string++);
}
 
//#include <conio.h>






int port = 1,car_auto=0,dis=0;
int ns,count=0,fun,f_go=0,up_dn_go=0,auto_car;
char letter;


void rs232_var_start(void){
   count   =0;
   f_go    =0;
   up_dn_go=0;
   port    =1;
   car_auto=0;
}

//rs232_1
int *rs232_1(void){
   int i,j,k;
   char memory[10];
   count=0;
   int resive=0,start_count=0,out[2],now;
   rs_putch(1, '7' ) ;
    while(1){
       if(kbhit()){           //鍵盤觸發 
          i = getche();                 //把鍵盤輸入丟進i
          rs_putch(1, i ) ;
          if(i=='a'){                   //開始自動平行 
             auto_car=1;                //自動平行旗號 
             rs_putch(1, '1' ) ;        //往前走 
          }
          if(i=='q'){                   //停止自動平行 
             rs_putch(1, '3' ) ;        //停止前進 
             rs_putch(1, '4' ) ;        //停止左右轉 
             auto_car=0;
          }   
       }
       if(count==0)start_count++;
       if(start_count>10) rs_putch(1, '7' ) ;
       if((letter=rs_getch(port))>0){  
          if(letter=='A' || letter==9){//前進後退 
             if(letter=='A' ){             //霍爾元件 
                dis++;
             }
          }
          else{
             if(letter==0x40){
                resive = 1;
             }
             if(resive == 1){
                memory[count]=letter;//把接收到的存入memory這個矩陣內 
                count++;             //計算矩陣位置 
                if(count==7){//左右的電位 
                   //txt_w(memory,num);//把momory的資料丟進去txt檔 
                   resive=0;
                   now = txt_r(memory);       //讀出現在電阻上面的值 
                   rs_putch(1, '8' ) ;
                   break;
                }
             }
          }   
       }
    }
    out[0] = now;
    out[1] = dis;
    return out; 
   
} 

int rs232_3(void)
{    
   int i,j,k;
      //rs232_var_start();
      //if(!rs_initialise(port ,9600, '8', 'N')) { getch(); exit(1); }//開啟RS232 
   count=0;      
   int resive=0,now,fun_num =0;
   char memory[10];
   rs_putch(1, '7' ) ;
    fun_num=0;       //函數從第0個開始 
    f_go=1;          //開始動作 
    car_auto=1;
    dis=0;
   while(1){   
      //if(kbhit()){           //鍵盤觸發 
      //   i = getche();       //把鍵盤輸入丟進i
       //  rs_putch(port, i ) ;//傳送i到8051 
       //  if(i=='a'){         //按a後開始停車 
           
       //  }
      //   if(i=='q')
      //      car_auto=0;
     // }
           
      if((letter=rs_getch(port))>0) { 
         if(letter=='A'){
            if(letter=='A'){             //霍爾元件 
               dis++;
            }
            if(dis > ns )  {              //經過10個磁鐵
               fun_num++;               //執行下一個函數
               dis=0;                   //磁鐵計數器規0
               f_go=1;                  //啟動左右控制
               up_dn_go=0;
               if(car_auto==1)
                  rs_putch(port, '3' ) ;   //0代表停 
            }
            printf("dis=%d\n",dis);
         }
         else{//控制左右 
            if(letter==0x40){
                 resive=1;
            }
            if(resive==1){  
               if(letter==61 || letter==62);//控制訊號
               else{
                  
                  memory[count]=letter;//把接收到的存入memory這個矩陣內 
                  count++;             //計算矩陣位置 
                  
                  if(count==7){//左右的電位 
                     count=0;resive=0;
                     //txt_w(memory,num);//把momory的資料丟進去txt檔 
                     now = txt_r(memory);       //讀出現在電阻上面的值 
                     
                     fun = txt_r_f1_rf(fun_num);//讀出現在函數的值 
                     
                     printf("now=%d\tfun=%d\tfun_num=%d\tkey=%c\t\n",now,fun,fun_num,(fun > now ? '5' : '6') );
                     if(car_auto==1){
                        if( now-100 < fun && now+100 >fun){
                           if(up_dn_go==0){
                              up_dn_go=2;      //開啟前進後退控制
                              rs_putch(port, txt_r_f1_ud(fun_num,0) ) ;
                              ns=txt_r_f1_ud(fun_num,1);
                           }
                           if(up_dn_go!=2){
                              up_dn_go=1;      //開啟前進後退控制    
                              
                           } 
                        }
                        rs_putch(port, (fun > now ? '5' : '6' )) ;  //左右 左或右
                           
                        if(now > 298000 || now < 18000 || fun<10000 ||fun>30000){//超出範圍就停止 
                           printf("stop!!\n");
                           car_auto=0;
                           rs_putch(port, '4' ) ;      //左右 停 
                           rs_putch(port, '3' ) ;      //前後 停 
                           break;
                        }
                     }//auto
                  }//else
               }//data
            }
         }//up down or right left
      }//letter 8051傳給電腦的值       
   }
     
   return 3;
}
 

