
#include <sys/socket.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#define MAX 512
int main(int argc , char *argv[]){

	int newSocket, ret;
	struct sockaddr_in server_addr;
	char input[MAX];

	newSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(newSocket < 0){
		printf("Connection Failed\n");
		exit(1);
	}

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[1]));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = connect(newSocket, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(ret < 0){
		printf("Connection Failed!\n");
		exit(1);
	}
	printf("Welcome to command server.\n");

	while(1){
		printf("Command: ");
		gets(input);
		send(newSocket, input, strlen(input), 0);
    
		if(strcmp(input, "QUIT") == 0){
			close(newSocket);
			printf("Goodbye!\n");
			exit(1);
		}

		if(recv(newSocket, input, MAX, 0) < 0){
			printf("something went wrong.\n");
		}else{
			printf("%s\n", input);
		}
	}

	return 0;
}
