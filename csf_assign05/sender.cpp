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
  std::string username;
  char * host = argv[1];
  char * port = argv[2]; 

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);

  Connection conn;

  // connect to server
  conn.connect(server_hostname, server_port);

  Message slogin =  Message("slogin", username);
  Message received = Message(); 
  bool sloginSent = conn.send(slogin); 
  conn.receive(received); 

   if (sloginSent == false || strcmp(received.tag.c_str(), "err") == 0) {
    //so print the payload
    std::cerr << slogin.data;
    //exit non-zero
    return 2;
  } else if(strcmp(received.tag.c_str(), "ok") == 0){
    cout << "this works"; 
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  string room; 
  string input = "";
  bool joined = false; 

  while(true) {
    std::getline (cin, input);
    //checks if the input has the join line 
    if(input.at(0) == '/') {
      if(input.substr(0, 4).compare("/join") == 0) {
      Message join = Message("join", input.substr(6, input.length()));
      bool sentMessage = conn.send(join);
      Message received = Message();
      conn.receive(received);
      //check if message format was formatted correctly and if the recieve worked 

      if(sentMessage == false || strcmp(received.tag.c_str(), "err") == 0){
         joined = false;
         std::cerr << received.data;
         continue; 
      } else {
        joined = true;
      }
      //if the input is leave then we leave the room
    } else if(input == "/leave" && joined == true){
      Message leave = Message("leave", "");
      bool sentMessage = conn.send(leave); 
      Message received = Message();
      conn.receive(received);
       if(sentMessage == false || strcmp(received.tag.c_str(), "err") == 0){
         std::cerr << received.data;
         continue; 
      } 
    } else if (input.compare("/quit") == 0) {
      Message quit = Message("quit", "");
      bool sentMessage = conn.send(quit); 
      Message received = Message();
      conn.receive(received);
      if(sentMessage == false || strcmp(received.tag.c_str(), "err") == 0){
         std::cerr << received.data;
         continue; 
      } else if(strcmp(received.tag.c_str(), "ok") == 0){
        //exits 
        return 0; 
      }
     } else {
       //prints out error for not a correct command 
       std::cerr << "not a correct command";
       continue; 
     }
    } else {
      //sends a message to all the rooms 
      Message send_all = Message("sendall", input.substr(8, input.length()));
      bool sentMessage = conn.send(send_all); 
      Message received = Message();
      conn.receive(received);
      if(sentMessage == false || strcmp(received.tag.c_str(), "err") == 0){
         std::cerr << received.data;
         continue; 
      }
    }
  }


  return 0;
}
