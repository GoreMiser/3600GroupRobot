/* This is a client for the final project
 * This code is meant to connect to an accompanying
 *     server, issue commands to the server, and
 *     receive a response from the server
 * 
 */

// Group Members:
//    Genevieve Watkins
//    Steven Platt
//    William Newberry
//    Zhaoxin Deng

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <signal.h>
#include <netdb.h>


void DieWithMessage(char* msg); //in case it breaks
void printResponse(char* resp, char* cmd, int size); 
void ctrlCCode();
void CatchAlarm(int ignored);
void sendMessages(char *cmd, char *id, int count, int sock, struct addrinfo *servAddr);  //allows us to control messages better between server/client
void catch_alarm(int sig);

int numFile = 0;
int numStop = 0;


int main(int argc, char* argv[]) {
	if(argc < 6 || argc > 6) {
		fprintf(stderr, "Incorrect usage: Expected: %s <Server Hostname> <Server Port> <Robot ID> <Length> <Side Number>\n", argv[0]);
	}
	
	mkdir("./robotOutput", 0777);

	//save args to use
	char *server = argv[1];
	char *servPort = argv[2];
	char *robotID = argv[3];
	char *cmd = (char *)malloc(65536); //actual command
	char getStuff[] = {'G', 'E', 'T', ' ', 'I', 'M', 'A', 'G', 'E', '\n', 'G', 'E', 'T', ' ', 'G', 'P', 'S', '\n', 'G', 'E', 'T', ' ', 'D', 'G', 'P', 'S', '\n', 'G', 'E', 'T', ' ', 'L', 'A', 'S', 'E', 'R', 'S', '\0'};
	int l = (atoi(argv[4]))/6, n = atoi(argv[5]);
	int count = 0;

	struct sigaction myAction; //for timeout alarm

	//set up ability to connect to server
	struct addrinfo addrCriteria;
	memset(&addrCriteria, 0, sizeof(addrCriteria));
	addrCriteria.ai_family = AF_UNSPEC;
	addrCriteria.ai_socktype = SOCK_DGRAM;
	addrCriteria.ai_protocol = IPPROTO_UDP;

	struct addrinfo *servAddr;
	int rtnVal = getaddrinfo(server, servPort, &addrCriteria, &servAddr);
	if(rtnVal != 0)
		DieWithMessage("getaddrinfo() failed");

	// Create a datagram/UDP socket
	int sock = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol);
	if(sock < 0)
		DieWithMessage("socket() failed");

	myAction.sa_handler = catch_alarm;
	if(sigfillset(&myAction.sa_mask) < 0)
	    DieWithMessage("sigfillset() failed");
	myAction.sa_flags = 0;

	if(sigaction(SIGALRM, &myAction, 0) < 0)
	    DieWithMessage("sigactiou() failed for SIGALRM");

	/*handle N and L for the messages*/
	//get the string ready for concatinations	

	//determine the messages to send
	/* IMAGE: GET IMAGE, GPS: GET GPS, DGPS: GET DGPS, LASERS: GET LASERS, MOVE: MOVE [VELOCITY], 
		TURN: TURN [VELOCITY](Radians), STOP: STOP*/
	
	//NEED: null termination on the ROBOTID
	//Figure out how to send separate commands to the server(MOVE then STOP, etc.)
	//sprintf(msg, "%s", robotID);

	//all angles are divided by 6 to accomodate timeout

	int c = 0; //count for loops to control command flow
	if(n == 4){
	    for(c = 0; c < n; c++){
	        //use str len to send L for MOVE
	        sprintf(cmd + (strlen(cmd)), "MOVE ");
	        sprintf(cmd + (strlen(cmd)), "%d\n", l); //send length
	        sprintf(cmd + (strlen(cmd)), "STOP\n");
	        sprintf(cmd + (strlen(cmd)), "TURN ");
	        sprintf(cmd + (strlen(cmd)), "0.2618\n"); //turn 90 degrees
		sprintf(cmd + (strlen(cmd)), "STOP\n");
	        sprintf(cmd + (strlen(cmd)), "%s\n", getStuff);
	    }

	    //now the triangle
	    for(c = 0; c < (n-1); c++){
		sprintf(cmd + (strlen(cmd)), "MOVE ");
		sprintf(cmd + (strlen(cmd)), "%d\n", l); //send length
		sprintf(cmd + (strlen(cmd)), "STOP\n");
		sprintf(cmd + (strlen(cmd)), "TURN ");
		sprintf(cmd + (strlen(cmd)), "0.1745\n"); //turn 60 degrees
		sprintf(cmd + (strlen(cmd)), "STOP\n");
		sprintf(cmd + (strlen(cmd)), "%s\n", getStuff);
	    }
	}

	else if(n == 5){
	    for(c = 0; c < n; c++){
	        //use str len to send L for MOVE
	        sprintf(cmd + (strlen(cmd)), "MOVE ");
	        sprintf(cmd + (strlen(cmd)), "%d\n", l); //send length
	        sprintf(cmd + (strlen(cmd)), "STOP\n");
	        sprintf(cmd + (strlen(cmd)), "TURN ");
	        sprintf(cmd + (strlen(cmd)), "0.3142\n"); //turn 108 degrees
		sprintf(cmd + (strlen(cmd)), "STOP\n");
	        sprintf(cmd + (strlen(cmd)), "%s\n", getStuff);
	    }

	    for(c = 0; c < (n-1); c++){
		sprintf(cmd + (strlen(cmd)), "MOVE ");
		sprintf(cmd + (strlen(cmd)), "%d\n", l); //send length
		sprintf(cmd + (strlen(cmd)), "STOP\n");
		sprintf(cmd + (strlen(cmd)), "TURN ");
		sprintf(cmd + (strlen(cmd)), "0.2618\n"); //turn 90 degrees
		sprintf(cmd + (strlen(cmd)), "STOP\n");
		sprintf(cmd + (strlen(cmd)), "%s\n", getStuff);
	    }
	}

	else if(n == 6){
	    for(c = 0; c < n; c++){
	        //use str len to send L for MOVE
	        sprintf(cmd + (strlen(cmd)), "MOVE ");
	        sprintf(cmd + (strlen(cmd)), "%d\n", l); //send length
	        sprintf(cmd + (strlen(cmd)), "STOP\n");
	        sprintf(cmd + (strlen(cmd)), "TURN ");
	        sprintf(cmd + (strlen(cmd)), "0.3491\n"); //turn 120 degrees
		sprintf(cmd + (strlen(cmd)), "STOP\n");
	        sprintf(cmd + (strlen(cmd)), "%s\n", getStuff);
	    }

	    for(c = 0; c < (n-1); c++){
		sprintf(cmd + (strlen(cmd)), "MOVE ");
		sprintf(cmd + (strlen(cmd)), "%d\n", l); //send length
		sprintf(cmd + (strlen(cmd)), "STOP\n");
		sprintf(cmd + (strlen(cmd)), "TURN ");
		sprintf(cmd + (strlen(cmd)), "0.3142\n"); //turn 108 degrees
		sprintf(cmd + (strlen(cmd)), "STOP\n");
		sprintf(cmd + (strlen(cmd)), "%s\n", getStuff);
	    }
	}

	else if(n == 7){
	    for(c = 0; c < n; c++){
	        //use str len to send L for MOVE
	        sprintf(cmd + (strlen(cmd)), "MOVE ");
	        sprintf(cmd + (strlen(cmd)), "%d\n", l); //send length
	        sprintf(cmd + (strlen(cmd)), "STOP\n");
	        sprintf(cmd + (strlen(cmd)), "TURN ");
	        sprintf(cmd + (strlen(cmd)), "0.3740\n"); //turn 128.57 degrees *hopefully correct*
		sprintf(cmd + (strlen(cmd)), "STOP\n");
	        sprintf(cmd + (strlen(cmd)), "%s\n", getStuff);
	    }

	    for(c = 0; c < (n-1); c++){
		sprintf(cmd + (strlen(cmd)), "MOVE ");
		sprintf(cmd + (strlen(cmd)), "%d\n", l); //send length
		sprintf(cmd + (strlen(cmd)), "STOP\n");
		sprintf(cmd + (strlen(cmd)), "TURN ");
		sprintf(cmd + (strlen(cmd)), "0.3491\n"); //turn 120 degrees *hopefully correct*
		sprintf(cmd + (strlen(cmd)), "STOP\n");
		sprintf(cmd + (strlen(cmd)), "%s\n", getStuff);
	    }
	}

	else if(n == 8){
	    for(c = 0; c < n; c++){
	        //use str len to send L for MOVE
	        sprintf(cmd + (strlen(cmd)), "MOVE ");
	        sprintf(cmd + (strlen(cmd)), "%d\n", l); //send length
	        sprintf(cmd + (strlen(cmd)), "STOP\n");
	        sprintf(cmd + (strlen(cmd)), "TURN ");
	        sprintf(cmd + (strlen(cmd)), "0.3927\n"); //turn 135 degrees
		sprintf(cmd + (strlen(cmd)), "STOP\n");
	        sprintf(cmd + (strlen(cmd)), "%s\n", getStuff);
	    }

	    for(c = 0; c < (n-1); c++){
		sprintf(cmd + (strlen(cmd)), "MOVE ");
		sprintf(cmd + (strlen(cmd)), "%d\n", l); //send length
		sprintf(cmd + (strlen(cmd)), "STOP\n");
		sprintf(cmd + (strlen(cmd)), "TURN ");
		sprintf(cmd + (strlen(cmd)), "0.3740\n"); //turn 128.57 degrees
		sprintf(cmd + (strlen(cmd)), "STOP\n");
		sprintf(cmd + (strlen(cmd)), "%s\n", getStuff);
	    }
	}

   //--------do we need set a loop here to ask for the next message?-------

	//send the messages
	sendMessages(cmd, robotID, count, sock, servAddr);

   	fprintf(stderr, "Commands are sent.\n");

	return 0;
}

//set up function to send messages to the server
void sendMessages(char *cmd, char *id, int count, int sock, struct addrinfo *servAddr) {
	
	int i = 0; //controls where in the message string you are
	
	char *resp = (char *)malloc(65536); //response string
	char tok;
	char *nextCommand = (char *)malloc(1024);
	char full[65536];
	
	struct sockaddr_storage fromAddr; //where the response comes from
	socklen_t fromAddrLen = sizeof(fromAddr);
   
   tok=cmd[0];
      
   //-----command message-------
   int j = 0; //index for command message.

    while(tok != '\0'){
      //-------inner loop----each command-----------
	strcpy(nextCommand, "abcd");
	strcat(nextCommand, id);
      while(tok!='\n')
      {
         nextCommand[j + strlen(nextCommand) + 1]=cmd[i];
         j++; //after loop, j=length of command message
	 i++; //to keep track of where we are in the msg[]
      
         tok = cmd[i]; //tok equals message index + length of command message 
      } 
		//use this to delay functions
		//accompany using time() to delay
		//get next command(FIX)
		//send an actual command and receive response
		sendto(sock, nextCommand, 1024, 0, servAddr->ai_addr, servAddr->ai_addrlen);

		alarm(5);

		int numrec = recvfrom(sock, resp, 65536, 0, (struct sockaddr *) &fromAddr, &fromAddrLen);

		sleep(alarm(0)+1);

		memset(full, 0, 65536);

		int total, sec, size = 0;
		memcpy(&total, resp+4, 4);
		memcpy(&sec, resp+8, 4);
		memcpy(full, resp+12, numrec-12);
		size += numrec-12;
		int k;
		for(k = 0; k < total-1; k++)
		{
			numrec = recvfrom(sock, resp, 65536, 0, (struct sockaddr *) &fromAddr, &fromAddrLen);
			memcpy(full+size, resp+12, numrec-12);
			memcpy(&sec, resp+8, 4);
			size += numrec-12;
		}

		//print the response
		printResponse(full, nextCommand, size);


    	  //-------get next message---------
     	 tok = cmd[i + 1];
    	  j=0; // setting j = 0
	  i += 1;
	  count++;

	  memset(nextCommand, 0, strlen(nextCommand) + i + 1);
	}

}

//error messages
void DieWithMessage(char* msg) {
	printf("%s\n", msg);
	exit(1);
}

//print response from server
void printResponse(char* resp, char* command, int size) {
        char num[12];
	char num2[12];

	char dir[200];
	strcpy(dir,"./robotOutput/");

	if(strncmp(command+10, "MOVE", 4) == 0)
	{
		strcat(dir, "MOVE");
		snprintf(num, 12, "%d", numFile);
		strcat(dir, num);
		strcat(dir, ".txt");
		FILE *file = fopen(dir, "w+");
		fwrite(resp, sizeof(char), size, file);
		fclose(file);
	}

	else if(strncmp(command+10, "STOP", 4) == 0)
	{
		strcat(dir, "STOP");
		snprintf(num, 12, "%d", numFile);
		strcat(dir, num);
		snprintf(num2, 12, "%d", numStop%2);
		strcat(dir, num2);
		strcat(dir, ".txt");
		FILE *file = fopen(dir, "w+");
		fwrite(resp, sizeof(char), size, file);
		fclose(file);
		numStop++;
	}

	else if(strncmp(command+10, "TURN", 4) == 0)
	{
		strcat(dir, "TURN");
		snprintf(num, 12, "%d", numFile);
		strcat(dir, num);
		strcat(dir, ".txt");
		FILE *file = fopen(dir, "w+");
		fwrite(resp, sizeof(char), size, file);
		fclose(file);
	}

	else if(strncmp(command+10, "GET IMAGE", 9) == 0)
	{
		strcat(dir, "IMAGE");
		snprintf(num, 12, "%d", numFile);
		strcat(dir, num);
		strcat(dir, ".jpeg");
		FILE *file = fopen(dir, "w+");
		fwrite(resp, sizeof(char), size, file);
		fclose(file);
	}

	else if(strncmp(command+10, "GET GPS", 7) == 0)
	{
		strcat(dir, "GPS");
		snprintf(num, 12, "%d", numFile);
		strcat(dir, num);
		strcat(dir, ".txt");
		FILE *file = fopen(dir, "w+");
		fwrite(resp, sizeof(char), size, file);
		fclose(file);
	}

	else if(strncmp(command+10, "GET DGPS", 8) == 0)
	{
		strcat(dir, "DGPS");
		snprintf(num, 12, "%d", numFile);
		strcat(dir, num);
		strcat(dir, ".txt");
		FILE *file = fopen(dir, "w+");
		fwrite(resp, sizeof(char), size, file);
		fclose(file);
	}

	else if(strncmp(command+10, "GET LASERS", 10) == 0)
	{
		strcat(dir, "LASERS");
		snprintf(num, 12, "%d", numFile);
		strcat(dir, num);
		strcat(dir, ".txt");
		FILE *file = fopen(dir, "w+");
		fwrite(resp, sizeof(char), size, file);
		fclose(file);
		numFile++;
	}


}

void catch_alarm(int sig){
	fprintf(stderr, "Message Timed out\n");
	exit(1);
}
