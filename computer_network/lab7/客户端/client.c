#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096
int connectServer();
void Handler(void *socket_fd);

//print the menu
void prompt() {
	printf("You can do following things.\n");
	printf("0-connect to the server\n");
	printf("1-get time now\n");
	printf("2-get name of the server\n"); 
	printf("3-get clients list\n");
	printf("4-send message to other client\n");
	printf("5-disconnect the server\n");
	printf("6-Quit\n");
}

int QUIT_FLAG = 0;
int hasConnected = 0;

int main() {
	int socket_fd = -1;
	prompt();
	
	//loop to read in choice
	char send_buf[BUFFER_SIZE];
	while(!QUIT_FLAG) {
		sleep(1);
		memset(send_buf, 0,sizeof(send_buf));
		printf("Please Enter your choice\n");
		int choice;
		scanf("%d",&choice);
		
		//determine what to do by the status of connection
		if(hasConnected == 0) {
			//connect to the server
			if(choice == 0) {
				socket_fd = connectServer();				
			}
			else if(choice == 6) {
				printf("You will quit the program!\n");
				return 0;
			} else if(1 <= choice && choice <= 5) {
				printf("You havn't connected to the server!\n");
			} else {
				printf("Your choice is not valid!\n");		
			}			
		}
		else {
			if(choice == 0) {
				printf("You've connected to the server!\n");
				continue;
			}
			else if(choice == 1) {
				strcpy(send_buf,"get time");
			}
			else if(choice == 2) {
				strcpy(send_buf,"get name");
			}
			else if(choice == 3) {
				strcpy(send_buf,"get clts");
			}
			else if(choice == 4) {
				printf("Who do you want to send message to? please enter the id:\n");
				int id;
				scanf("%d",&id);
				getchar();
				char message[4000];
				memset(message,0,sizeof(message));
				printf("Please enter the message:\n");
				fgets(message, sizeof(message), stdin);
				//write the client's id and message into the data
				sprintf(send_buf,"%02d %s",id,message);
			}
			//disconnect the server
			else if(choice == 5) {
				hasConnected = 0;
				strcpy(send_buf,"Quit");			
			}
			//quit the whole program
			else if(choice == 6) {
				QUIT_FLAG = 1;
				printf("You will quit the program!\n");
				strcpy(send_buf,"Quit");
			}
			else {
				printf("Your choice is not valid!\n");
				continue;
			}
			
			//send data to the server
			if(send(socket_fd, send_buf, strlen(send_buf), 0) < 0) {
				printf("fail to send to the server!\n");
				exit(1);
			}	
		}
	}
	//close the socket
	if(close(socket_fd) < 0) {
		printf("fail to close the client!");
	}
	return 0;
}

//connect to the server, return the socket fd
int connectServer() {
	char server_ip[20];
	char server_port[10];
	
	printf("Please enter the ip of the server:");
	scanf("%s",server_ip);	
	printf("Please enter the port of the server:");
	scanf("%s",server_port);
	
	//only port 1331 is acceptable
	if(strcmp(server_port,"1331") != 0) {
		printf("The port can't be connected!\n");
		return -1;
	}
	
	int socket_fd;
	struct sockaddr_in addr_remote;

	memset(&addr_remote, 0, sizeof(addr_remote));

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd < 0) {
		printf("fail to create socket!\n");
		exit(1);
	}
	
	pthread_t thread_id;
	if(pthread_create(&thread_id, NULL, (void*)(&Handler), (void*)(&socket_fd)) < 0) {
		printf("fail to create pthread!\n");
		exit(1);
	}
	
	addr_remote.sin_family = AF_INET;
	addr_remote.sin_addr.s_addr = inet_addr(server_ip);
	addr_remote.sin_port = htons(1331);
	
	if(connect(socket_fd, (struct sockaddr *)(&addr_remote), sizeof(struct sockaddr)) < 0) {
		printf("fail to connect!\n");
		exit(1);
	}	
	sleep(1);
	return socket_fd;
}

//handle the data the client received
void Handler(void *socket_fd) {
	char recv_buf[BUFFER_SIZE];
	int recv_length;
	int fd = *((int*)socket_fd);
	//loop unitl the connection is accepted
	while(hasConnected == 0) {
		memset(recv_buf,0,sizeof(recv_buf));
		recv_length = recv(fd, recv_buf, sizeof(recv_buf), 0);
		if(strcmp(recv_buf, "connection accpeted") == 0) {
			printf("%s\n",recv_buf);
			hasConnected = 1;
		}		
	} 

	int close_flag = 0;
	while(1) {
		memset(recv_buf,0,sizeof(recv_buf));
		int recv_length = recv(fd, recv_buf, sizeof(recv_buf), 0);
		if(strlen(recv_buf) == 0) {
			printf("Empty message received! Maybe the server has closed\n");
			break;		//break loop for safety
		} else {
			printf("You received a message:%s\n",recv_buf);
		}
		//the QUIT is entered at the client
		if(strcmp(recv_buf,"Quit") == 0) {
			close_flag = 1;
			break;
		} 
		//the QUIT is entered at the server
		else if(strcmp(recv_buf,"QuitS") == 0) {
			close_flag = 1;
			QUIT_FLAG = 1;
			break;
		}
	}
	
	//close the socket
	if(close_flag && close(fd) < 0) {
		printf("fail to close the receiveHandler!\n");
	}
	printf("The receiveHandler has been closed now!\n");
	//the QUIT is from the server, so exit the whole program
	if(QUIT_FLAG)
		exit(0);
	return;
}

