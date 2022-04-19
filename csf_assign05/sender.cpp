#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

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

  int clientfd = open_clientfd(argv[1], argv[2]);

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

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  while()

  return 0;
}
