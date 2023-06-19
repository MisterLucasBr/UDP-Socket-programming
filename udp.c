#include <unistd.h> // for close
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#define MAXBUF 1024

#define LISTEN_PORT 50001
#define REMOTE_PORT 50001
#define SEND_PORT 50002


// function PROTOTYPES avoid warning: implicit declaration of function
int server();
int client();


int main(int argc, char* argv[])
{
/* 
// check for the right number of arguments
if (argc < 2)
{
fprintf(stderr, "Usage: %s <port>\n", argv[0]);
return 1; // exit(1);
}

if (argc < 3)
{
fprintf(stderr, "Usage: %s <ip address> <port>\n", argv[0]);
exit(1);
}
*/
int returnStatus = client();
}

int server()
{
int udpSocket;
int returnStatus = 0;
int addrlen = 0;
struct sockaddr_in udpServer, udpClient;
char buf[MAXBUF];


/* create a socket */
udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
if (udpSocket == -1)
{
fprintf(stderr, "Could not create a socket!\n");
return 1; // exit(1);
}
else {
printf("Socket created.\n");
}

/* set up the server address and port */
/* use INADDR_ANY to bind to all local addresses */
udpServer.sin_family = AF_INET;
udpServer.sin_addr.s_addr = htonl(INADDR_ANY);
/* use the port passed as argument */
//udpServer.sin_port = htons(atoi(argv[1]));
udpServer.sin_port = htons(LISTEN_PORT);

/* bind to the socket */
returnStatus = bind(udpSocket, (struct sockaddr*)&udpServer, sizeof(udpServer));

if (returnStatus == 0) {
fprintf(stderr, "Bind completed!\n");
}
else {
fprintf(stderr, "Could not bind to address!\n");
close(udpSocket);
return 1; // exit(1);
}

while (1)
{
addrlen = sizeof(udpClient);
returnStatus = recvfrom(udpSocket, buf, MAXBUF, 0,
(struct sockaddr*)&udpClient, &addrlen);

if (returnStatus == -1) {
fprintf(stderr, "Could not receive message!\n");
}
else {
printf("Received: %s\n", buf);
/* a message was received so send a confirmation */
strcpy(buf, "OK");
returnStatus = sendto(udpSocket, buf, strlen(buf)+1, 0, (struct sockaddr*)&udpClient, sizeof(udpClient));

if (returnStatus == -1) {
fprintf(stderr, "Could not send confirmation!\n");
}
else {
printf("Confirmation sent.\n");
}
}
}
}

int client()
{
int udpSocket;
int returnStatus;
int addrlen;
struct sockaddr_in udpClient, udpServer;
char buf[MAXBUF];

/* create a socket */
udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
if (udpSocket == -1)
{
fprintf(stderr, "Could not create a socket!\n");
return 1; // exit(1);
}
else {
printf("Socket created.\n");
}
/* client address */
/* use INADDR_ANY to use all local addresses */
udpClient.sin_family = AF_INET;
udpClient.sin_addr.s_addr = INADDR_ANY;
udpClient.sin_port = htons(SEND_PORT);

returnStatus = bind(udpSocket, (struct sockaddr*)&udpClient, sizeof(udpClient));

if (returnStatus == 0) {
fprintf(stderr, "Bind completed!\n");
}
else {
fprintf(stderr, "Could not bind to address!\n");
close(udpSocket);
return 1; // exit(1);
}

/* set up the message to be sent to the server */
strcpy(buf, "For Professionals, By Professionals.\n");
/* server address */
/* use the command-line arguments */
udpServer.sin_family = AF_INET;
//udpServer.sin_addr.s_addr = inet_addr(argv[1]); // argv[1] does not work
udpServer.sin_addr.s_addr = inet_addr("192.168.1.4");
//udpServer.sin_port = htons(atoi(argv[2]));
udpServer.sin_port = htons(REMOTE_PORT);

returnStatus = sendto(udpSocket, buf, strlen(buf)+1, 0, (struct sockaddr*)&udpServer, sizeof(udpServer));

if (returnStatus == -1) {
fprintf(stderr, "Could not send message!\n");
}
else {
printf("Message sent.\n");

/* message sent: look for confirmation */
addrlen = sizeof(udpServer);
returnStatus = recvfrom(udpSocket, buf, MAXBUF, 0, (struct sockaddr*)&udpServer, &addrlen);

if (returnStatus == -1) {
fprintf(stderr, "Did not receive confirmation!\n");
}
else {
buf[returnStatus] = 0;
printf("Received: %s\n", buf);
}
}
/* cleanup */
close(udpSocket);
return 0;
}