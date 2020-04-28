/*
  BlueZ example code to build an rfcomm client.
  This code just creates a socket and connects
  to a remote bluetooth device and sends a string.
  Programmed by Bastian Ballmann
  http://www.geektown.de
  Compile with gcc -lbluetooth <executable> <source>
*/

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>

int main(int argc, char *argv[])
{
  //structre variable for bluetooth address of server
  struct sockaddr_rc addrress = { 0 };
  int s, status;

  // Already known address and name of server
  char dest[18]=argv[1];
  char namelaptop[20]="ubuntu";

  // allocate a socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  // set the connection parameters (who to connect to)
  // AF_BLUETOOTH is a bluetooth family, that is generally used.
  addrress.rc_family = AF_BLUETOOTH;
  addrress.rc_channel = (uint8_t) 1;
  // Convert the string "48:51:B7:85:2B:62" into an bluetooth address.
  str2ba( dest, &addrress.rc_bdaddr );

  // Initializtion of local bluetooth device
  int dev_id, sock;
  dev_id = hci_get_route(NULL);
  sock = hci_open_dev( dev_id );
  if (dev_id < 0 || sock < 0) {
      perror("Error while opening socket");
      exit(1);
  }

  status = connect(s, (struct sockaddr *)&addrress, sizeof(addrress));
  //successful, connect() returns 0.

  printf("connection status: %d\n\n",status);
  //0 show OK

  // send contents of file "test.txt" to server
  FILE *fp = fopen(argv[2], "r");    
  // Buffer array for string
  char li[100];

  while(fscanf(fp, "%s", li) != EOF){
      printf("%s", li);
      write(s, li, 6);    
  }
  // Instade of fscanf, scanf can also be used to take input from terminal

  // Ending the connection, letting the server know it by sending "END"
  write(s, "END", 6);

  printf("Closing socket\n");
  close(s);
  close( sock );
  return 0;
}