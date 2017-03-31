#include "Robot.h"

int sendRequest(char *command, char *http);
int getContent(char *http, char *content, int recvlen);

static const int BUFSIZE = 1000000;
char id[50];		//robot id
char imgId[50];			//image id
char robip[100];	//ip of the robot
char request1[1024];
char response1[1000];
char request2[1000];
char contenta[1000000];
char http1[1000000];

int main(int argc, char *argv[])
{
	int port = atoi(argv[1]);		//middleware port
	strcpy(robip, argv[2]);
	strcpy(id, argv[3]);
	strcpy(imgId, argv[4]);

	struct sockaddr_in midAddr;
	struct sockaddr_in clntAddr;
	unsigned int clntAddrLen;
	char sentId[50];
	char command[100];

	/* Create socket for sending/receiving datagrams */
	int sock;
   if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
      DieWithError("socket() failed");

   /* Construct local address structure */
   memset(&midAddr, 0, sizeof(midAddr));   /* Zero out structure */
   midAddr.sin_family = AF_INET;                /* Internet address family */
   midAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
   midAddr.sin_port = htons(port);      /* Local port */

   /* Bind to the local address */
   //printf("Binding to port %d\n", echoServPort);    
   if (bind(sock, (struct sockaddr *) &midAddr, sizeof(midAddr)) < 0)
      DieWithError("bind() failed");

	for(;;)
	{
		clntAddrLen = sizeof(clntAddr);

		memset(request1, 0, 1024);
		if(recvfrom(sock, request1, 1024, 0, (struct sockaddr*)&clntAddr, &clntAddrLen) < 0)
			DieWithError("recvfrom() failed");

		char *requestp = &request1[0];
		requestp = requestp + 4;

		strcpy(sentId, requestp);
		if(strcmp(sentId, id) != 0)
		{
			fprintf(stderr, "Incorrect Robot ID\n");
			continue;
		}

		requestp += strlen(requestp)+1;
		strcpy(command, requestp);
//fprintf(stderr, "%s\n", command);
		memset(http1, 0, BUFSIZE);
		char *httpp = &http1[0];
		int recvlen = sendRequest(command, httpp);
//fprintf(stderr, "%s\n", httpp);

		memset(contenta,0,BUFSIZE);
		char *content = &contenta[0];
		int len = getContent(httpp, content, recvlen);
//fprintf(stderr, "%s\n", content);

		int total = len / 988 + 1;
		if(total % len == 0)
			total--;
		int i;
		for(i = 0; i < total-1; i++)
		{
			memset(response1, 0, 1000);
			response1[0] = request1[0];
			response1[1] = request1[1];
			response1[2] = request1[2];
			response1[3] = request1[3];

			memcpy(response1+4, &total, 4);

			memcpy(response1+8, &i, 4);
			
			memcpy(response1+12, content, 988);
			content += 988;

			sendto(sock, response1, 1000, 0, (struct sockaddr *)&clntAddr, sizeof(clntAddr));
		}

		len -= 988*(total-1);
		response1[0] = request1[0];
		response1[1] = request1[1];
		response1[2] = request1[2];
		response1[3] = request1[3];

		memcpy(response1+4, &total, 4);

		memcpy(response1+8, &i, 4);

		memcpy(response1+12, content, len);
		sendto(sock, response1, len+12, 0, (struct sockaddr *)&clntAddr, sizeof(clntAddr));
	}
}

int sendRequest(char *command, char *http)
{
	int port;
	memset(request2, 0, 1000);
	strcpy(request2, "GET /");
	if(strncmp(command, "GET", 3) == 0)
	{
		if(strcmp(command+4, "GPS") == 0)
		{
			strcat(request2, "state?id=");
			strcat(request2, id);
			port = 8082;
		}

		if(strcmp(command+4, "DGPS") == 0)
		{
			strcat(request2, "state?id=");
			strcat(request2, id);
			port = 8084;
		}

		if(strcmp(command+4, "LASERS") == 0)
		{
			strcat(request2, "state?id=");
			strcat(request2, id);
			port = 8083;
		}

		if(strcmp(command+4, "IMAGE") == 0)
		{
			port = 8081;
			strcat(request2, "snapshot?topic=/robot_");
			strcat(request2, imgId);
			strcat(request2, "/image?width=600?height=500");
		}
	}

	else
	{
		port = 8082;
		strcat(request2, "twist?id=");
		strcat(request2, id);
		strcat(request2, "&");
		if(strncmp(command, "MOVE", 4) == 0)
		{
			strcat(request2, "lx=");
			strcat(request2, command+5);
		}

		if(strncmp(command, "TURN", 4) == 0)
		{
			strcat(request2, "az=");
			strcat(request2, command+5);
		}

		if(strncmp(command, "STOP", 4) == 0)
		{
			strcat(request2, "lx=0&az=0");
		}
	}

	strcat(request2, " HTTP/1.1\r\nHost: ");
	strcat(request2, robip);
	strcat(request2, ":");
	char ports[20];
	sprintf(ports, "%d", port);
	strcat(request2, ports);
	strcat(request2, "\r\nConnection: close\r\n\r\n");

	//create socket
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock < 0)
		DieWithError("socket() failed");

	//construct server address structure
	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	struct hostent *thehost = gethostbyname(robip);
	servAddr.sin_addr.s_addr = *((unsigned long *)thehost->h_addr_list[0]);

	//establish connection
	if(connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
		DieWithError("connect() failed");

	//send to robot
	size_t numBytesSent = send(sock, request2, strlen(request2), 0);
	if(numBytesSent < 0)
		DieWithError("send() failed");

	int total = 0;	
	int numBytesRcvd;
	char temp[BUFSIZE];
	while((numBytesRcvd = recv(sock, temp, BUFSIZE, 0)) > 0)
	{
		memcpy(http+total, temp, numBytesRcvd);
		total += numBytesRcvd;
	}
	if(numBytesRcvd < 0)
			DieWithError("recv() failed");

	return total;
}

int getContent(char *http, char *content, int revclen)
{
	if(strncmp(http, "HTTP/1.1", 8) == 0)
	{
		while(strncmp(http, "Content-Length", 14) != 0)
			http += 1;

		http += 16;
		int stop = 1;
		while(*(http+stop) != '\r')
			stop++;
		char hold[20];
		memcpy(hold, http, stop);
		int len = atoi(hold);
		memset(hold, 0, 20);

		while(strncmp(http, "\r\n\r\n", 4) != 0)
			http += 1;

		http += 4;
		memcpy(content, http, len);
		return len;
	}

	else
	{
		char *temp = http;
		while(strncmp(temp, "\r\n\r\n", 4) != 0)
			temp += 1;

		temp += 4;
		int len = revclen - (temp - http);
		memcpy(content, temp, len);
		memset(temp, 0, len);
		return len;
	}
}
