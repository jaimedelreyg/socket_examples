#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <error.h>
int main(int argc, char *argv[]){

struct addrinfo hints, *res;
struct sockaddr_storage cli;
char buf[80];
socklen_t clen;
char host[NI_MAXHOST], serv[NI_MAXSERV];
fd_set set;
struct timeval tout;

printf("Direccion:  %s \n", argv[1] );
printf("Puerto: %s \n", argv[2]);
fflush(stdout);

hints.ai_flags = AI_PASSIVE;
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_DGRAM;
hints.ai_protocol = 0;


getaddrinfo(argv[1], argv[2], &hints, &res);

int sd = socket (res->ai_family, res->ai_socktype, 0);
printf("Socket:  %d",sd);
fflush(stdout);
//creacion de descriptores para la multiplexacion
FD_ZERO(&set);
FD_SET(0,&set);
FD_SET(sd,&set);

tout.tv_sec = 2;
tout.tv_sec = 0;

bind (sd, (struct sockaddr *)res->ai_addr, res->ai_addrlen);

while(1){

if(select (FD_SETSIZE, &set, NULL, NULL, &tout) == -1){

        perror("Select Error");

}

if(select (sd + 1, &set, NULL, NULL, &tout) == 0){
	printf("Waiting for input");
	fflush(stdout);
}


clen = sizeof(cli);

int c = recvfrom (sd, buf, 80, 0,
(struct sockaddr *) &cli, &clen);

getnameinfo ((struct sockaddr *) &cli, clen, host, 
NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST);

buf[c] = '\0';

if(c > 0){
printf("Received %sAddress: %s Port: %s \n", buf, host, serv);
}

sendto(sd, buf, c, 0, (struct sockaddr *) &cli, clen);
}
}
