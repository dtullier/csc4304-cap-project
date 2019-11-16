#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <ctype.h>
#include <ctime>
#include <errno.h>
#include "CapParser.hpp"
#include <poll.h>

#define SERVER_PORT 8000

using namespace rapidxml;
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	int len, sock_ret, ioctl_ret, bind_ret, listen_ret, poll_ret, receive, trysend, on = 1;
	int sock = -1, new_sock = -1;
	bool end_server = false, remove_conn = false;
	bool close_conn = false;
	char buffer[2048];
	struct sockaddr_in6 addr;
	int timeout;
	struct pollfd poll_file_des[200];
	int nfds = 1, current_poll_size = 0;
        FILE *fp, *fp2;
	int ch = 0;
	int words;
	
	// create stream socket (for TCP, IP)
	sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock < 0)
	{
		printf("Error opening socket");
		return 1;
	}
	
	// socket should be reusable
	sock_ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if (sock_ret < 0) {
		printf("Error with setsockopt");
		close(sock);
		return 1;
	}
	
	// make socket nonblocking
	ioctl_ret = ioctl(sock, FIONBIO, (char *)&on);
	if (ioctl_ret < 0) {
		printf("Error making socket nonblocking in ioctl()");
		close(sock);
		return 1;
	}
	
	// bind socket
	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	addr.sin6_port = htons(SERVER_PORT);
	bind_ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (bind_ret < 0) {
		printf("Error binding socket in bind()");
		close(sock);
		return 1;
	}
	
	// create listening queue
	listen_ret = listen(sock, 32);
	if (listen_ret < 0) {
		printf("Error creating listen queue");
		close(sock);
		return 1;
	}
	
	// initialize pollfd struct
	memset(poll_file_des, 0, sizeof(poll_file_des));
	
	// set up listening sock
	poll_file_des[0].fd = sock;
	poll_file_des[0].events = POLLIN;
	
	// 1 hour timeout
	timeout = (60 * 60 * 1000);
	
	// check for incoming connections or data in loop
	do {
		// poll
		printf("Waiting...\n");
		poll_ret = poll(poll_file_des, nfds, timeout);
		
		// check if poll failed
		if (poll_ret < 0) {
			printf("poll() failed\n");
			return 1;
		}
		
		// check for timeout
		if(poll_ret == 0) {
			printf("Server Timeout\n");
			return 0;
		}
		
		// check which sockets are readable
		current_poll_size = nfds;
		for (int i = 0; i < current_poll_size; i++) {
		        cout << i;	
			// find those that returned POLLIN
			if(poll_file_des[i].revents == 0)
				continue;
			
			// if event not POLLIN, unexpected error
			if(poll_file_des[i].revents != POLLIN) {
				printf("Error %d\n", poll_file_des[i].revents);
				end_server = true;
				break;
			}
			
			if(poll_file_des[i].fd == sock) {
				// Accept incoming connections
			    do {
			        new_sock = accept(sock, NULL, NULL);
				//printf("%d",new_sock);
			        if (new_sock < 0) {
			        	// if EWOULDBLOCK, all have been accepted
			        	if (errno != EWOULDBLOCK) {
			        		printf("accept failed\n");
			        		end_server = true;
			        	}
			        	break;
			        }
			        
			        // Add new connection to poll struct
			        printf("Adding new connection - %d\n", new_sock);
			        poll_file_des[nfds].fd = new_sock;
			        poll_file_des[nfds].events = POLLIN;
			        nfds++;
				break;
			        
			        // Accept another incoming connection
			    } while(new_sock != -1);
			    break;
			}
			
			
			else {
				close_conn = false;
				// Socket had POLLIN but is not socket for incoming connections.
				// Receive data from socket
				printf("pll_file_des[i].fd is %d", poll_file_des[i].fd);

				do {
				    receive = read(poll_file_des[i].fd, buffer, 2048);
				    printf("%s\n", buffer);
				    if (receive < 0) {
				         	if (errno != EWOULDBLOCK) {
					        	printf("recv()");
						        close_conn = true;
					        }
					        break;
				    }
				
				    // check if connection closed by client
				    if (receive == 0) {
					    printf("Connection closed\n");
					    close_conn = true;
				    } 
				
				// if not connection closed, data received
				    len = receive;
				
				// echo data; change this part to parse and store data
				    trysend = send(poll_file_des[i].fd, buffer, len, 0);
				    cout << buffer;
				    cout << len;
				    if (trysend < 0) {
					    printf("failed to send\n");
					    close_conn = true;
					    break;
				    }
				    printf("sent data back\n");
				} while(true);
		 
		                printf("%d\n",close);	
			        // close connection
			        if (close_conn){
				        printf("closing connection %d/n", poll_file_des[i].fd);
				        close(poll_file_des[i].fd);
				        poll_file_des[i].fd = -1;
				        remove_conn = true;
			        }
			}
			printf("finished loop\n");
			      
		}
		
	} while(true);
	
	// need to clean up poll array to remove descriptors of sockets for closed connections
	if (remove_conn) {
		remove_conn = false;
		for (int i = 0; i < nfds; ++i) {
			if (poll_file_des[i].fd == -1){
				for(int j = i; j < nfds; ++j) {
					poll_file_des[j].fd = poll_file_des[j+1].fd;
				}
				i--;
				nfds--;
			}
		}
	} while (end_server == false);
	
	
	// server ending, close all sockets
	for (int i = 0; i < nfds; ++i) {
		if(poll_file_des[i].fd >= 0) close(poll_file_des[i].fd);
	}
}

