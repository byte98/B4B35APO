#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

char* getIP()
{
	struct ifaddrs *addrs;
	getifaddrs(&addrs);
	struct ifaddrs *tmp = addrs;
	char* reti = calloc(32, sizeof(char));

	while (tmp)
	{
		if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
		{
			struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
			char bfr[32];
			sprintf(bfr, "%s", tmp->ifa_name);

			if (strcmp(bfr, "eth0") == 0)
			{
				sprintf(reti, "%s", inet_ntoa(pAddr->sin_addr));
			}
		}

		tmp = tmp->ifa_next;
	}

	freeifaddrs(addrs);
	return reti;
}

void connect_remote()
{
	RC_STATUS = "UP";
	struct addrinfo hints, *results;
	//struct sockaddr_in *ip_access_temp;
	int rv;
	int sockfd;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(RC_SERVER, "80", &hints, &results)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	}

	sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);



	connect(sockfd, results->ai_addr, results->ai_addrlen);

	char* request = calloc(2048, sizeof(char));
	strcpy(request, "HEAD ");
	strcat(request, RC_SERVER_CONNECT);
	strcat(request, "?ip=");
	strcat(request, getIP());
	strcat(request, " HTTP/1.1\r\n");
	strcat(request, "Host: ");
	strcat(request, RC_SERVER);
	strcat(request, "\r\n");
	strcat(request, "Connection: close\r\n\r\n");


	send(sockfd, request, strlen(request), 0);



	freeaddrinfo(results);
	RC_STATUS = " ";
}

void update_remote_status()
{
	RC_STATUS = "UP";
	struct addrinfo hints, *results;
	//struct sockaddr_in *ip_access_temp;
	int rv;
	int sockfd;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(RC_SERVER, "80", &hints, &results)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	}

	sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);



	connect(sockfd, results->ai_addr, results->ai_addrlen);

	char* request = calloc(2048, sizeof(char));
	strcpy(request, "HEAD ");
	strcat(request, RC_SERVER_UPDATE_REMOTE);
	strcat(request, "?state=");
	strcat(request, (remote_allowed == 0 ? "false" : "true"));
	strcat(request, " HTTP/1.1\r\n");
	strcat(request, "Host: ");
	strcat(request, RC_SERVER);
	strcat(request, "\r\n");
	strcat(request, "Connection: close\r\n\r\n");


	send(sockfd, request, strlen(request), 0);



	freeaddrinfo(results);
	RC_STATUS = " ";
}

void get_remote_config()
{
	RC_STATUS = "DOWN";
	struct addrinfo hints, *results;
	//struct sockaddr_in *ip_access_temp;
	int rv;
	int sockfd;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(RC_SERVER, "80", &hints, &results)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	}

	sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);



	connect(sockfd, results->ai_addr, results->ai_addrlen);

	char* request = calloc(2048, sizeof(char));
	strcpy(request, "GET ");
	strcat(request, RC_SERVER_GET_CONFIG);
	strcat(request, " HTTP/1.1\r\n");
	strcat(request, "Host: ");
	strcat(request, RC_SERVER);
	strcat(request, "\r\n");
	strcat(request, "Connection: close\r\n\r\n");


	send(sockfd, request, strlen(request), 0);



	freeaddrinfo(results);
	RC_STATUS = " ";
}