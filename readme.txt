Genevieve Watkins
Steven Platt
William Newberry
Zhaoxin Deng

For the Client

To run the client, you need to use the following command-line parameters must be used
   <executable_name>
   <IP/host_name> //dotted decimal IP address or host name of the server
   <port> //the port that the server is running on
   <ID> //the ID of the robot that the client wishes to communicate with (serves as a password to be verified by the middleware
   <L> //the length of each side of the polygonal trabel path
   <N> //the shape of the polygonal travel path (hard coded into our code)

Given a particular L and N, the client will have to individually communicate the command necessary for the robot to complete its travel (waiting for each command to finish before sending the next).  The client will also receive messages from the middleware and take the output and place it in a new directory.

Command messages have the following format

   Robot ID (NULL-terminated string)
   Robot Command (NULL-sterminated string)

Example of a full message:

   MOVE 4\0 STOP\0 TURN <90 degrees>\0 <.....>

individual command would be:

   MOVE 4\0


For the Server

To run the server, you need to use the following command-line parameters:
    <executable_name>  
    <server port>  //port for the server to use
    <robot IP addr> //address of the robot you are connecting to
    <robot ID>  //ID of the robot you want to connect to
    <image ID>  //ID of the images for the robot

The middleware will open a connection from the client and receive messages that will be packaged as an HTTP message to be sent to the robot.  Responses from the robot are sent to the client.
