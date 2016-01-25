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

  /* ソケットの作成 */
  printf("socket \n");
  sock0 = socket(AF_INET, SOCK_STREAM, 0);
  if( sock0<0 ){
  	perror("socket");
  	return 0;
  }

  /* ソケットの設定 */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;
  // addr.sin_len = sizeof(addr);

  // portがTIME_WAIT状態でも接続する
  const int one = 1;
  setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

  printf("bind \n");
  if (  bind(sock0, (struct sockaddr *)&addr, sizeof(addr))<0 ){
  	perror("bind");
  	return 0;
  }

  /* TCPクライアントからの接続要求を待てる状態にする */
  printf("listen \n");
  if(   listen(sock0, 5)<0 ){
  	perror("listen");
  	return 0;
  }


  while (1) {

	len = sizeof(client);
	printf("accept \n");

	// クライアントから通信があるまで待機
	sock = accept(sock0, (struct sockaddr *)&client, &len);
	if(	sock<0 ){
	  perror("accept");
	  return 0;
	}

	printf("accepted connection from %s, port=%d\n",
		   inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	
	/* write(ソケット,"文字",文字数) */
	//  write(sock, "HELLO from mac01", 16);
	send(sock, "HELLO", 5, 0);
	
	/* TCPセッションの終了 */
	printf("close \n");
	close(sock);

  }

  /* listen するsocketの終了 */
  printf("close \n");
  close(sock0);
	
  return 0;

}
