
#ifndef __NETWORKING_H__
#define __NETWORKING_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#define RC_SERVER "http://wa.toad.cz"
#define RC_BUFFER_LEN 2048

struct Array{
    char* data;
    int length;
};
typedef struct Array array_t;

int socket_connect(char *host, in_port_t port){
	struct hostent *hp;
	struct sockaddr_in addr;
	int on = 1, sock;     

	if((hp = gethostbyname(host)) == NULL){
		herror("gethostbyname");
		exit(1);
	}
	copy(hp->h_addr, &addr.sin_addr, hp->h_length);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));

	if(sock == -1){
		perror("setsockopt");
		exit(1);
	}
	
	if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){
		perror("connect");
		exit(1);

	}
	return sock;
}

array_t send(char* script, char* data)
{
    char* reti_data = calloc(RC_BUFFER_LEN, sizeof(char));
    array_t reti;
    reti.data = reti_data;
    reti.length = 0;

    char* script_name = calloc(256, sizeof(char));
    strcpy(script_name, RC_SERVER);
    strcat(script_name, script);



    char* request = 
    "GET / HTTP/1.1 \r\n"
    "Host: " RC_SERVER "\r\n"
    ;

    return reti;
}
#endif