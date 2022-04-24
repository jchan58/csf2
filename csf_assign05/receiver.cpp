#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

#include <sstream>

using std::cin; 

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // connect to server
  conn.connect(server_hostname, server_port);

  //check if connection opened, print error and fail if not
  if (!conn.is_open()) {
    std::cerr << "Could not connect to server" << std::endl;
    return 5;
  }
  
  //rlogin and join messages (expect a response from
  //       the server for each one)
  Message rlogin = Message("rlogin", username);
  bool login = conn.send(rlogin);

  Message fromServer = Message();
  conn.receive(fromServer);


  //if an error is reported for rlogin
  if (login == false || strcmp(fromServer.tag.c_str(), "err") == 0) {
    //print the server response payload
    std::cerr << fromServer.data << std::endl;
    //exit non-zero
    return 2;
  }

  Message join = Message("join", room_name);
  bool joined = conn.send(join);
  conn.receive(fromServer);
  

  //if an error is reported for join
  if(joined == false || strcmp(fromServer.tag.c_str(), "err") == 0) {
    //print the server response payload and exit
    std::cerr << fromServer.data << std::endl;
    return 3;
  }

  // loop waiting for messages from server
  while(true) {
    //is empty at first
    Message received = Message();
    conn.receive(received);

    //break the received message up and print the username and message
    std::stringstream ss;
    ss << received.data;

    std::vector<std::string> split_msg;
    std::string word;
    while(std::getline(ss, word, ':')){
      split_msg.push_back(word);
    }
    
    std::cout << split_msg[1] << ": " << split_msg[2] << std::endl;

  }

  return 0;
}
