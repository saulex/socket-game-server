#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <iostream>
#include <syslog.h>
#include <list>
#include <thread>

#define BUFSIZE 34
#define TRUE 1
#define FALSE 0
#define PORT 8888

void init_log()
{
	setlogmask(LOG_UPTO(LOG_NOTICE));
	openlog("testd", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

/*
 * * error - wrapper for perror
 * */

void error(char *msg)
{
	perror(msg);
	exit(1);
}
size_t FindIndex(const int a[], size_t size, int value)
{
	size_t index = 0;

	while (index < size && a[index] != value)
		++index;

	return (index == size ? -1 : index);
}

int main(int argc, char **argv)
{
	init_log();

	while (1)
	{

		int portno = 0;
		if (argc != 2)
		{
			fprintf(stderr, "usage: %s <port>\n", argv[0]);
			exit(1);
		}
		portno = atoi(argv[1]);

		int opt = TRUE;
		int master_socket, addrlen, new_socket, client_socket[30],
		max_clients = 30, activity, i, sd;
		int max_sd;
		struct sockaddr_in address;

		char buffer[1025]; 


		fd_set readfds;


		char const *message = "ECHO Daemon v1.0 \r\n";


		for (i = 0; i < max_clients; i++)
		{
			client_socket[i] = 0;
		}


		if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		{
			perror("socket failed");
			exit(EXIT_FAILURE);
		}

		if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
					        sizeof(opt)) < 0)
		{
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}


		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(portno + 1);


		if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
		{
			perror("bind failed");
			exit(EXIT_FAILURE);
		}
		printf("Listener on port %d \n", portno + 1);


		if (listen(master_socket, 3) < 0)
		{
			perror("listen");
			exit(EXIT_FAILURE);
		}


		addrlen = sizeof(address);
		puts("Waiting for connections ...");


		std::list<sockaddr_in> addres_list; 
		struct sockaddr_in my_addr, cli_addr[30], cli_temp, cli_addr_temp[30];
		int sockfd;
		socklen_t slen[30], slen_temp;

		slen_temp = sizeof(cli_temp);
		uint8_t buf[BUFSIZE];
		int clients = 0;
		int client_port[30];
		int client_out = 0;
		int continue_next_socket = 1;
		std::cout << "Server : Socket() successful\n";

		if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		{
			printf("test\n");

		}
		else
		{
			printf("Server : Socket() successful\n");
		}
		bzero(&my_addr, sizeof(my_addr));
		my_addr.sin_family = AF_INET;
		my_addr.sin_port = htons(portno);
		my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
		{

		}
		else
		{
			printf("Server : bind() successful\n");
		}

		std::thread t([&]()
				{
				while (1)
				{


				FD_ZERO(&readfds);


				FD_SET(master_socket, &readfds);
				max_sd = master_socket;

				for (i = 0; i < max_clients; i++)
				{

				  sd = client_socket[i];


				    if (sd > 0)
				        FD_SET(sd, &readfds);

				      if (sd > max_sd)
					          max_sd = sd;
				}


				activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

				if ((activity < 0) && (errno != EINTR))
				{
					  printf("select error");
				}


				if (FD_ISSET(master_socket, &readfds))
				{
					  if ((new_socket = accept(master_socket,
									                              (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
						    {
							        perror("accept");
								    exit(EXIT_FAILURE);
								      }


					    printf("New connection , socket fd is %d , ip is : %s , port : %d ", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));


					      if (send(new_socket, message, strlen(message), 0) != strlen(message))
						        {
								    perror("send");
								      }

					        puts("Welcome message sent successfully");


						  for (i = 0; i < max_clients; i++)
							    {

								        if (client_socket[i] == 0)
										    {
											          client_socket[i] = new_socket;

												        printf("Adding to list of sockets as %d\n", i);

													      break;
													          }
									  }
				}
				printf("check while");

				for (i = 0; i < max_clients; i++)
				{
					  sd = client_socket[i];

					    if (FD_ISSET(sd, &readfds))
						      {
							          printf("check disconnected , i %i \n", 1);

								      ssize_t valread;
								          printf("check disconnected , i %i \n", 2);
									      if ((valread = read(sd, buffer, 1024)) == 0)
										          {
												        printf("check disconnected , i %i \n", 3);

													      getpeername(sd, (struct sockaddr *)&address,
															                        (socklen_t *)&addrlen);
													            printf("Host disconnected , ip %s , port %d \n",
																                  inet_ntoa(address.sin_addr), ntohs(address.sin_port));

														          int newIndex = 0;


															        for (auto it = addres_list.begin(); it != addres_list.end(); ++it) {

																	          const auto& cli_addr_element = *it;

																		            printf("Host disconnected for , ip %s , port %d \n",
																					                  inet_ntoa(cli_addr_element.sin_addr), ntohs(cli_addr_element.sin_port));

																			              printf("iteranti disconnected , i %i \n", 1);
																				       
																				                if (strcmp(inet_ntoa(address.sin_addr), inet_ntoa(cli_addr_element.sin_addr))== 0) {
																							              printf("eraseee disconnected , i %i \n", 2);
																								                    it = addres_list.erase(it); 
																										              }
																						      }
																    
																      close(sd);
																            client_socket[i] = 0;
																	        }


									          else
											      {
												            printf("check disconnected , i %i \n", 4);
													        }
										    }
				}


				}
				});

		while (continue_next_socket == 1)
		{


			if (recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&cli_temp, &slen_temp) == -1)
				printf("buf \n");
			if (1 == 1)
			{
				int result = buf[0];
				if (result == 0)
				{
					cli_addr[clients] = cli_temp;

					addres_list.push_front(cli_temp);
					client_port[clients] = ntohs(cli_addr[clients].sin_port);
					printf("isdigit \n");
					printf("clients :%i \n", clients);
					sendto(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&cli_temp, sizeof(cli_temp));

					printf("New connection from utp, socket fd is %d , ip is : %s , port : %d  \n", new_socket, inet_ntoa(cli_temp.sin_addr), ntohs(cli_temp.sin_port));
					clients = clients + 1;
				}
				else if (result == 9)
				{

				}
				else if (result == 8)
				{
					printf("isdigit \n");
					printf("restart all \n");
					continue_next_socket = 0;
				}
				else if (result == 10 || result == 11)
				{

					int size = addres_list.size();
					printf(" size list : %i \n",  size);
					for (auto const& i : addres_list) {

						sendto(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&i, sizeof(i));
						    
					}


				}
			}
			else
			{
			}
		}

		close(sockfd);
	}
	return 0;
}

int main2()
{

	while (TRUE)
	{
	}

	return 0;
}



