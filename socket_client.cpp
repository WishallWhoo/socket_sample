#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>

int main()
{
  struct sockaddr_in server;
  int sock;
  char buf[32];
  int n;

  /* �����åȤκ��� */
  printf("socket \n");
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if( sock<0 ){
	perror("socket");
	return 0;
	
  }

  /* ��³������ѹ�¤�Τν��� */
  server.sin_family = AF_INET;
  server.sin_port = htons(12345);
  server.sin_addr.s_addr = inet_addr("192.168.1.70"); // IP���ɥ쥹������

  /* �����Ф���³ */
  printf("connect \n");
  if( connect(sock, (struct sockaddr *)&server, sizeof(server))<0 ){
  	perror("connect");
  	return 0;
  }

  memset(buf, 0, sizeof(buf));

  /* �����Ф���ǡ�������� */
  printf("read \n");
  n = read(sock, buf, sizeof(buf));
  if( n <0 ){
  	perror("read");
  	return 0;
  }

  printf("%d, %s\n", n, buf);

  /* socket�ν�λ */
  close(sock);

  return 0;


}

 
