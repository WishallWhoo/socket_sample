#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>

int main()
{
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  socklen_t len;
  int sock;

  /* �����åȤκ��� */
  printf("socket \n");
  sock0 = socket(AF_INET, SOCK_STREAM, 0);
  if( sock0<0 ){
  	perror("socket");
  	return 0;
  }

  /* �����åȤ����� */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;
  // addr.sin_len = sizeof(addr);

  // port��TIME_WAIT���֤Ǥ���³����
  const int one = 1;
  setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

  printf("bind \n");
  if (  bind(sock0, (struct sockaddr *)&addr, sizeof(addr))<0 ){
  	perror("bind");
  	return 0;
  }

  /* TCP���饤����Ȥ������³�׵���ԤƤ���֤ˤ��� */
  printf("listen \n");
  if(   listen(sock0, 5)<0 ){
  	perror("listen");
  	return 0;
  }


  while (1) {

	len = sizeof(client);
	printf("accept \n");

	// ���饤����Ȥ����̿�������ޤ��Ե�
	sock = accept(sock0, (struct sockaddr *)&client, &len);
	if(	sock<0 ){
	  perror("accept");
	  return 0;
	}

	printf("accepted connection from %s, port=%d\n",
		   inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	
	/* write(�����å�,"ʸ��",ʸ����) */
	//  write(sock, "HELLO from mac01", 16);
	send(sock, "HELLO", 5, 0);
	
	/* TCP���å����ν�λ */
	printf("close \n");
	close(sock);

  }

  /* listen ����socket�ν�λ */
  printf("close \n");
  close(sock0);
	
  return 0;

}
