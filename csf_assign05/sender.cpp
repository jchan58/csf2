#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"


using std::cin; 
using std::string;

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;
  char * host = argv[1];
  char * port = argv[2]; 

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);

  Connection conn;

  // connect to server
  conn.connect(server_hostname, server_port);

  Message slogin =  Message("slogin", username);

   if (conn.send(slogin) == false) {
    //so print the payload
    std::cerr << slogin.data;
    //exit non-zero
    return 2;
  }

 //   Message join = Message("join", room_name);

 //   Message leave = Message("leave");

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  string room; 
  string input = "";
  bool joined = false; 


  while(true) {
    std::getline (std::cin, input);
    //checks if the input has the join line 
    if(input.find("/join")) {
      Message join = Message("join", input);
      conn.send(join);
      //check if message format was formatted correctly and if the recieve worked 
      if(conn.send(join) == false && conn.receive()){
         std::cerr << "connection error from server";
         joined = false; 
         //if the connection was not estabilished then break out of the loop 
         break;
      } else {
        joined = true;
      }
      //if the input is leave then we estabilish the room. 
    } else if(input == "/leave"){
      Message leave = Message("leave", "need to leave");
      
    }
  }

  //wait for server response 
 // if(!conn.receive()){

 // } 

  return 0;
}
