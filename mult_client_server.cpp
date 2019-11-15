#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <ctime>
#include "CapParser.hpp"
#include <poll.h>
#include <ctpl.h>

#define SERVER_PORT 55555;

using namespace rapidxml;
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	int len, sock_ret, ioctl_ret, bind_ret, listen_ret, poll_ret, receive, on = 1;
	int sock = -1, new_sock = -1;
	bool end_server = false, remove_conn = false;
	int close_connection;
	char buffer[80];
	struct sockadder_in addr;
	int timeout;
	struct pollfd poll_file_des[200];
	int nfds = 1, current_size = 0, i, j;
	
	// create stream socket (for TCP, IPv4)
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		fprintf(stderr, "Error opening socket");
		return 1;
	}
	
	// socket should be reusable
	sock_ret = setsockopt(socket_description, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if (sock_ret < 0) {
		fprintf(stderr, "Error with setsockopt");
		close(sock);
		return 1;
	}
	
	// make socket nonblocking
	ioctl_ret = ioctl(listen_sd, FIONBIO, (char *)&on);
	if (ioctl_ret < 0) {
		fprintf(stderr, "Error making socket nonblocking in ioctl()");
		close(sock);
		return 1;
	}
	
	// bind socket
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	memcpy(&addr.sin_addr, &inadder_any, sizeof(inaddr_any));
	addr.sin_port = htons(SERVER_PORT);
	bind_ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (bind_ret < 0) {
		fprintf(stderr, "Error binding socket in bind()")
		close(sock);
		return 1;
	}
	
	// create listening queue
	listen_ret = listen(sock, 32);
	if (listen_ret < 0) {
		fprintf(stderr, "Error creating listen queue");
		close(sock);
		return 1;
	}
	
	// initialize pollfd struct
	memset(poll_file_des, 0, sizeof(fds));
	
	// set up listening sock
	poll_file_des[0].fd = sock;
	poll_file_des[0].events = POLLIN;
	
	// 1 hour timeout
	timeout = (60 * 60 * 1000);
	
	// check for incoming connections or data in loop
	do {
		// poll
		printf("Waiting...");
		poll_ret = poll(poll_file_des, nfds, timeout);
		
		// check if poll failed
		if (poll_ret < 0) {
			fprintf(stderr, "poll() failed");
			return 1;
		}
		
		// check for timeout
		if(poll_ret == 0) {
			fprintf("Server Timeout");
			return 0;
		}
		
		// check which sockets are readable
		poll_size = nfds;
		for (i = 0; i < poll_size; i++) {
			
			// find those that returned POLLIN
			if(poll_file_des[i].revents == 0)
				continue;
			
			// if event not POLLIN, unexpected error
			if(poll_file_des[i].revents != POLLIN) {
				fprintf(stderr, "Error %d\n", poll_file_des[i].revents);
				end_server = TRUE;
				break;
			}
			
			if(poll_file_des[i].fd == sock) {
				// Accept incoming connections
			    do {
			        new_sock = accept(sock, NULL, NULL);
			        if (new_sock) < 0 {
			        	// if EWOULDBLOCK, all have been accepted
			        	if (errno != EWOULDBLOCK) {
			        		fprintf(stderr, "accept failed");
			        		end_server = Truel
			        	}
			        	break;
			        }
			        
			        // Add new connection to poll struct
			        printf("New connection - %d\n", new_sock);
			        poll_file_des[nfds].fd = new_sock;
			        poll_file_des[nfds].events = POLLIN;
			        nfds++;
			        
			        // Accept another incoming connection
			    } while(new_sock != -1);
			}
			
			
			else {
				close_conn = FALSE;
				
				// Socket had POLLIN but is not socket for incoming connections.
				// Receive data from socket
				receive = recv(poll_file_des[i].fd, buffer, sizeof(buffer), 0);
				if (receive < 0) {
					if (errno != EWOULDBLOCK) {
						fprintf(stderr, "recv()");
						close_conn = TRUE;
					}
					break
				}
				
				// check if connection closed by client
				if (receive == 0) {
					fprintf(stderr, "Connection closed\n");
					close_conn = TRUE;
					break;
				}
				
				// if not connection closed, data received
				len = receive;
				
				// echo data; change this part to parse and store data
				trysend = send(poll_file_des[i].fd, buffer, len, 0);
				if (trysend < 0) {
					fprintf(stderr, "failed to send");
					close_conn = TRUE;
					break;
				}
			} while(true);
			
			// close connection
			if (close_conn){
				close(poll_file_des[i].fd);
				poll_file_des[i].fd = -1;
				remove_conn = true;
			}
			
			      
		}
		
	}
	
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
