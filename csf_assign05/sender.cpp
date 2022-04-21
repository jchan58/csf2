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
using std::cout; 

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username = argv[3]; 
  char * host = argv[1];
  char * port = argv[2]; 

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);

  Connection conn;

  // connect to server
  conn.connect(server_hostname, server_port);

  //check if connection opened
  if (!conn.is_open()) {
    std::cerr << "Could not connect to server" << std::endl;
    return 5;
  }

  Message slogin =  Message("slogin", username);
  bool sloginSent = conn.send(slogin); 
  Message received = Message(); 
  conn.receive(received); 

   if (sloginSent == false || strcmp(received.tag.c_str(), "err") == 0) {
    //so print the payload
    std::cerr << received.data << std::endl;
    //exit non-zero
    return 2;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  string room; 
  string input = "";
  bool joined = false; 
  char command = '/';

  while(true) {
    std::getline(cin, input); 
    //checks if the input has the join line 
    if(command == input.at(0)) {
      if(input.at(1) == 'j'){
      Message join = Message("join", input.substr(6, input.length()));
      bool sentMessage = conn.send(join);
      Message received = Message();
      conn.receive(received);
      //check if message format was formatted correctly and if the recieve worked 

      if(sentMessage == false || strcmp(received.tag.c_str(), "err") == 0){
        std::cerr << received.data << std::endl;
        continue;
      } else {
        joined = true;
      }
      //if the input is leave then we leave the room
    } else if(input == "/leave"){
      Message leave = Message("leave", "");
      bool sentMessage = conn.send(leave); 
      Message received = Message();
      conn.receive(received);
       if(sentMessage == false || strcmp(received.tag.c_str(), "err") == 0){
         std::cerr << received.data << std::endl;
         continue; 
      } 
    } else if (input.compare("/quit") == 0) {
      Message quit = Message("quit", "");
      bool sentMessage = conn.send(quit); 
      Message received = Message();
      conn.receive(received);
     if(strcmp(received.tag.c_str(),"ok") == 0){
        return 0; 
      } else if(strcmp(received.tag.c_str(),"err") == 0){
        std::cerr << received.data << std::endl;
        continue; 
      }
     }
    } else if(command != input.at(0)) {
      //sends a message to all the rooms 
      Message send_all = Message("sendall", input);
      bool sentMessage = conn.send(send_all); 
      Message received = Message();
      conn.receive(received);
      if(sentMessage == false || strcmp(received.tag.c_str(), "err") == 0){
         std::cerr << received.data << std::endl;
         continue; 
      }
    }
  }


  return 0;
}
