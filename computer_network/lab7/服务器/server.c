#include<stdio.h> 
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h> 
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<time.h>

#define PORT 1331
#define BUFFER_SIZE 4096
#define MAX_CONNECTION 100 
//Limit the maximum number of connections to 100
struct client {
	int isAlive;
	pthread_t pt;
	int fd;
	char* ip;
	uint64_t port;
	int id;			//from 0-99
};

//save clients
struct client clients[MAX_CONNECTION];
int clients_number = 0;
int live_number = 0;

void acceptHandler(void *socket_fd);
void receiveHandler(void *void_client);

int main() {
	//create a new socket
	int socket_fd = 0; 
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(socket_fd < 0) {
		printf("fail to create a socket\n");
		exit(1);
	} 
	
	//initialize the addresss
	struct sockaddr_in addr_local = {0};
	addr_local.sin_family = AF_INET;
	addr_local.sin_port = htons(PORT);
	addr_local.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//bind the socket
	if(bind(socket_fd, (void*)(&addr_local), sizeof(addr_local)) < 0) {
		printf("fail to bind\n");
		close(socket_fd);
		exit(1);
	}
	
	if(listen(socket_fd, MAX_CONNECTION) < 0) {
		printf("fail to listen\n'");
		exit(1);
	}

	//create a new thread to handle accept
	pthread_t thread_id;
	if (pthread_create(&thread_id, NULL, (void*)(&acceptHandler), &socket_fd) < 0) {
		printf("fail to create a pthread\n'");
	}
	
	//the main thread read the input from user
	while(1) {
		char op[30];
		scanf("%s",op);
		if(strcmp(op,"Quit") == 0) {
			break;
		} 
	}
	
	//close all sockets
	printf("Waiting for stop...\n",live_number,clients_number);
	char send_buf[50];
	pthread_kill(thread_id, 0);
	for(int i = 0; i < clients_number; i++) {
		if(clients[i].isAlive) {
			memset(send_buf, 0, sizeof(send_buf));
			//ask the client to quit
			strcpy(send_buf, "QuitS");
			if(send(clients[i].fd, send_buf, strlen(send_buf), 0) < 0) {
				printf("fail to send\n");
			}
			sleep(1);
		}
		pthread_kill(clients[i].pt, 0);
	}
	sleep(1);
	return 0;
}

//handle the accept
void acceptHandler(void *socket_fd) {
	while(1) {
		printf("Waiting for connection...\n");
		int fd = *((int*)socket_fd);
		int socklen = sizeof(struct sockaddr_in);
		struct sockaddr_in addr_remote;
		int client_fd = accept(fd,(struct sockaddr *)(&addr_remote), (socklen_t *)(&socklen));
		if(client_fd < 0) {
			printf("fail to accept\n'");
			exit(1);
		}
		
		//tell client to start to read message from the server
		char send_buf[50];
		strcpy(send_buf, "connection accpeted");
		if(send(client_fd, send_buf, strlen(send_buf), 0) < 0) {
			printf("fail to send!\n");
			exit(1);
		}
				
		printf("accpet a new connection!\n");
		printf("ip: %s:%d\n", inet_ntoa(addr_remote.sin_addr), addr_remote.sin_port);

		//save the new client in the clients list
		clients[clients_number].isAlive = 1;
		clients[clients_number].fd = client_fd;
		clients[clients_number].ip = inet_ntoa(addr_remote.sin_addr);
		clients[clients_number].port = addr_remote.sin_port;
		clients[clients_number].id = clients_number;
		printf("Now, there are %d clients alive\n",live_number+1);
		
		//create a new thread to receive client's data
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, (void*)(&receiveHandler),&clients[clients_number]); 
		clients[clients_number].pt = thread_id;
		
		//increase the number of clients
		clients_number += 1;
		live_number += 1;		
	} 

	if(close(socket_fd) < 0) {
		printf("fail to close the server\n");
		exit(1);
	}
	printf("The server has been closed now!\n");
	pthread_exit(NULL);
}

void receiveHandler(void *void_client) {
	struct client client = *((struct client*) void_client);
	
	char recv_buf[BUFFER_SIZE];
	char send_buf[BUFFER_SIZE];
	int recv_length;
	while(1) {
		//clear the buffer
		memset(recv_buf, 0, sizeof(recv_buf));
		recv_length = recv(client.fd, recv_buf, sizeof(recv_buf), 0);
		
		if(recv_length < 0) {
			printf("fail to receive data\n");
			break;
		}
		
		if(recv_length == 0) {
			printf("No data received!\n");
			break;		//break the while loop for safety
		}
		
		//receive QUIT, so close the connection between client and server
		printf("receive data from client%02d:%s\n", client.id, recv_buf);
		if(strcmp(recv_buf, "Quit") == 0) {
			strcpy(send_buf, "Quit");
			if(send(client.fd, send_buf, strlen(send_buf), 0) < 0) {
				printf("fail to send\n");
				break;
			}
			break;
		}
		
		else if(strcmp(recv_buf, "get time") == 0) {
			time_t nowtime = time(NULL);
			strftime(send_buf,sizeof(send_buf),"%Y-%m-%d %H:%M:%S",localtime(&nowtime));
		}
		
		else if(strcmp(recv_buf, "get name") == 0) {
			gethostname(send_buf, sizeof(send_buf));
		}
		
		else if(strcmp(recv_buf, "get clts") == 0){
			//head of the list
			char head[50] = "\nid    ip              port     alive";
			strcpy(send_buf, head);
			for(int i = 0; i < clients_number; i++) {
				char row[60] = {0};
				char alive[4];
				if(clients[i].isAlive == 0) {
					strcpy(alive,"No");
				} else {
					strcpy(alive,"Yes");
				}
				sprintf(row, "\n%-6d%-16s%-8lu %s",clients[i].id,clients[i].ip,clients[i].port,alive);
				strcat(send_buf, row);
				
				//mark the client who sent the request
				if(clients[i].id == client.id) {
					strcat(send_buf,"  <-you\n");
				}					
			}
		}
		
		else {
			int des, src;
			sscanf(recv_buf,"%d",&des);
			src = client.id;
			char message[4000];
			sprintf(message,"To client%-2d From client%-2d:%s\nip:%s,port:%u\n",des,src,recv_buf+3,clients[des].ip,clients[des].port);
			//tell the client whether the message has been sent successfully
			int res = send(clients[des].fd, message, strlen(message), 0);
			if(res < 0) {
				strcpy(send_buf, "fail to send to the client\nThe client may not exist or not alive.");
			} else {
				strcpy(send_buf, "The server has sent the message.");
			}
		}
		
		if(send(client.fd, send_buf, strlen(send_buf), 0) < 0) {
			printf("fail to send\n");
			break;
		}
	}
	
	//set the state of the client
	((struct client*) void_client)->isAlive = 0;
	live_number -= 1;
	printf("close the connection of client%02d\n",client.id);
	//close the socket
	close(client.fd);
	//exit the thread
	pthread_exit(NULL);
}

