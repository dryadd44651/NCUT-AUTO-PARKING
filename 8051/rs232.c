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
       if(kbhit()){           //��LĲ�o 
          i = getche();                 //����L��J��ii
          rs_putch(1, i ) ;
          if(i=='a'){                   //�}�l�۰ʥ��� 
             auto_car=1;                //�۰ʥ���X�� 
             rs_putch(1, '1' ) ;        //���e�� 
          }
          if(i=='q'){                   //����۰ʥ��� 
             rs_putch(1, '3' ) ;        //����e�i 
             rs_putch(1, '4' ) ;        //����k�� 
             auto_car=0;
          }   
       }
       if(count==0)start_count++;
       if(start_count>10) rs_putch(1, '7' ) ;
       if((letter=rs_getch(port))>0){  
          if(letter=='A' || letter==9){//�e�i��h 
             if(letter=='A' ){             //�N������ 
                dis++;
             }
          }
          else{
             if(letter==0x40){
                resive = 1;
             }
             if(resive == 1){
                memory[count]=letter;//�Ⱶ���쪺�s�Jmemory�o�ӯx�}�� 
                count++;             //�p��x�}��m 
                if(count==7){//���k���q�� 
                   //txt_w(memory,num);//��momory����ƥ�i�htxt�� 
                   resive=0;
                   now = txt_r(memory);       //Ū�X�{�b�q���W������ 
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
      //if(!rs_initialise(port ,9600, '8', 'N')) { getch(); exit(1); }//�}��RS232 
   count=0;      
   int resive=0,now,fun_num =0;
   char memory[10];
   rs_putch(1, '7' ) ;
    fun_num=0;       //��Ʊq��0�Ӷ}�l 
    f_go=1;          //�}�l�ʧ@ 
    car_auto=1;
    dis=0;
   while(1){   
      //if(kbhit()){           //��LĲ�o 
      //   i = getche();       //����L��J��ii
       //  rs_putch(port, i ) ;//�ǰei��8051 
       //  if(i=='a'){         //��a��}�l���� 
           
       //  }
      //   if(i=='q')
      //      car_auto=0;
     // }
           
      if((letter=rs_getch(port))>0) { 
         if(letter=='A'){
            if(letter=='A'){             //�N������ 
               dis++;
            }
            if(dis > ns )  {              //�g�L10�Ӻ��K
               fun_num++;               //����U�@�Ө��
               dis=0;                   //���K�p�ƾ��W0
               f_go=1;                  //�Ұʥ��k����
               up_dn_go=0;
               if(car_auto==1)
                  rs_putch(port, '3' ) ;   //0�N�� 
            }
            printf("dis=%d\n",dis);
         }
         else{//����k 
            if(letter==0x40){
                 resive=1;
            }
            if(resive==1){  
               if(letter==61 || letter==62);//����T��
               else{
                  
                  memory[count]=letter;//�Ⱶ���쪺�s�Jmemory�o�ӯx�}�� 
                  count++;             //�p��x�}��m 
                  
                  if(count==7){//���k���q�� 
                     count=0;resive=0;
                     //txt_w(memory,num);//��momory����ƥ�i�htxt�� 
                     now = txt_r(memory);       //Ū�X�{�b�q���W������ 
                     
                     fun = txt_r_f1_rf(fun_num);//Ū�X�{�b��ƪ��� 
                     
                     printf("now=%d\tfun=%d\tfun_num=%d\tkey=%c\t\n",now,fun,fun_num,(fun > now ? '5' : '6') );
                     if(car_auto==1){
                        if( now-100 < fun && now+100 >fun){
                           if(up_dn_go==0){
                              up_dn_go=2;      //�}�ҫe�i��h����
                              rs_putch(port, txt_r_f1_ud(fun_num,0) ) ;
                              ns=txt_r_f1_ud(fun_num,1);
                           }
                           if(up_dn_go!=2){
                              up_dn_go=1;      //�}�ҫe�i��h����    
                              
                           } 
                        }
                        rs_putch(port, (fun > now ? '5' : '6' )) ;  //���k ���Υk
                           
                        if(now > 298000 || now < 18000 || fun<10000 ||fun>30000){//�W�X�d��N���� 
                           printf("stop!!\n");
                           car_auto=0;
                           rs_putch(port, '4' ) ;      //���k �� 
                           rs_putch(port, '3' ) ;      //�e�� �� 
                           break;
                        }
                     }//auto
                  }//else
               }//data
            }
         }//up down or right left
      }//letter 8051�ǵ��q������       
   }
     
   return 3;
}
 

