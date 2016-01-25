#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define EVAL_NUM 10000
#define SERVER_IP "192.168.1.70"

int main()
{
  struct sockaddr_in server;
  int sock;
  char buf[1];
  int n;

  FILE *fp;						// for file io
  int i;

  struct timespec tp1;			// for clock
  int counter = 0;
  double write_time[EVAL_NUM];
  double read_time[EVAL_NUM];
  double transport_time[EVAL_NUM];

  /* ソケットの作成 */
  printf("socket \n");
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if( sock<0 ){
	perror("socket");
	return 0;
	
  }

  /* 接続先指定用構造体の準備 */
  server.sin_family = AF_INET;
  server.sin_port = htons(12345);
  server.sin_addr.s_addr = inet_addr(SERVER_IP); // IPアドレスを設定

  /* サーバに接続 */
  printf("connect \n");
  if( connect(sock, (struct sockaddr *)&server, sizeof(server))<0 ){
  	perror("connect");
  	return 0;
  }

  memset(buf, 0, sizeof(buf));

  printf("start eval as a client\n");
  //  printf("==================\n");
  while( counter < EVAL_NUM ){

	// 時刻の記録
	if(clock_gettime(CLOCK_REALTIME,&tp1) < 0){
	  perror("clock_gettime begin");
	  return 0;
	}
	write_time[counter] = (double)tp1.tv_sec + (double)tp1.tv_nsec/ (double)1000000000L;

	/* write(ソケット,"文字",文字数) */
	//	printf("write \n");
	write(sock, "1", 1);
	
	/* サーバからデータを受信 */
	//	printf("read \n");
	n = read(sock, buf, sizeof(buf));
	if( n <= 0 ){
	  perror("read");
	  return 0;
	}
	//	printf("counter: %d, %s\n", counter, buf);
	// 時刻の記録
	if(clock_gettime(CLOCK_REALTIME,&tp1) < 0){
	  perror("clock_gettime begin");
	  return 0;
	}
	read_time[counter] = (double)tp1.tv_sec + (double)tp1.tv_nsec/ (double)1000000000L;
	
	//	printf("==================\n");
	counter++;

  }

  if((fp = fopen("transport_time_socket.txt", "w")) != NULL){
	for(i=0; i<=EVAL_NUM; i++){
	  transport_time[i] = read_time[i] - write_time[i];
	  if(fprintf(fp, "%1.9lf\n", transport_time[i]) < 0){
		//書き込みエラー
		break;
	  }
	}
	fclose(fp);
  }else{
	printf("error : can't output file \n");
  }

  /* socketの終了 */
  close(sock);

  printf("==end evaluation==\n");

  return 0;


}

 
