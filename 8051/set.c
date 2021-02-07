#include <windows.h>
#include <stdio.h>
#include <conio.h>

int set()
{
   DCB dcb;
   HANDLE hCom;
   BOOL fSuccess;
   char c,*pcCommPort = "COM1";
   
   hCom = CreateFile( pcCommPort,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
   
   if (hCom == INVALID_HANDLE_VALUE)
   {
      printf ("CreateFile failed with error %d.\n", GetLastError());
      system("pause");
      return (1);
   }
   fSuccess = GetCommState(hCom, &dcb);
   
   if (!fSuccess)
   {
      printf ("GetCommState failed with error %d.\n", GetLastError());
      system("pause");
      return (2);
   }
   
   dcb.BaudRate = 9600; // set the baud rate
   dcb.ByteSize = 8; // data size, xmit, and rcv
   dcb.Parity = NOPARITY; // no parity bit
   dcb.StopBits = ONESTOPBIT; // one stop bit
   
   fSuccess = SetCommState(hCom, &dcb);
   
   if (!fSuccess)
      {
      printf ("SetCommState failed with error %d.\n", GetLastError());
      system("pause");
      return (3);
   }
   
   printf ("Serial port %s successfully reconfigured.\n", pcCommPort);
   
   CloseHandle(hCom);
   return (0);
}
