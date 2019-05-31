#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

char* getIP(char* interface_name)
{
    int fd;
 struct ifreq ifr;

 fd = socket(AF_INET, SOCK_DGRAM, 0);

 /* I want to get an IPv4 IP address */
 ifr.ifr_addr.sa_family = AF_INET;

 strncpy(ifr.ifr_name, interface_name, IFNAMSIZ-1);

 ioctl(fd, SIOCGIFADDR, &ifr);

 close(fd);

 char* reti = calloc(32, sizeof(char));
 sprintf(reti, "%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
 return reti;
}

int main(void) {
    printf("IP: %s\n", getIP("wifi0"));

    struct addrinfo hints, *results;
    //struct sockaddr_in *ip_access_temp;
    int rv;
    int sockfd;
	char inputVal[100];
	unsigned char buffer[4096];
	printf("Enter a Domain Name: \n");
	scanf("%s",inputVal);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ( (rv = getaddrinfo( inputVal , "80" , &hints , &results)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

 

    connect(sockfd, results->ai_addr, results->ai_addrlen);

    char* request = calloc(2048, sizeof(char));
    strcpy(request, "HEAD /apo/api/connect.php?ip=10.10.10.10 HTTP/1.1\r\n");
    strcat(request, "Host: cvut.skodaj.cz\r\n");
    strcat(request, "Connection: close\r\n\r\n");

    printf("%s\n", request);

    send(sockfd, request, strlen(request), 0);
        int recv_length = 1;
        recv_length = recv(sockfd, &buffer, 1024, 0);
        while(recv_length > 0) {
          printf("The web server is %s\n", buffer+8);
          freeaddrinfo(results);
         return 0;
         }

        printf("Server line not found\n");



    freeaddrinfo(results);

    printf("\n");


    return 1;

}

