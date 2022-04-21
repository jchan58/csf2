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

  //check if connection opened
  if (!conn.is_open) {
    std::cerr << "Could not connect to server" << std::endl;
    return 5;
  }
  
  // send (is receive?) rlogin and join messages (expect a response from
  //       the server for each one)
  Message rlogin = Message("rlogin", username);

 //check if send worked
  bool login = conn.send(rlogin);
  //check if server accepted send
  Message fromServer = Message();
  
  conn.receive(fromServer);

  if (login == false || strcmp(fromServer.tag.c_str(), "err") == 0) {
    //so print the server payload
    std::cerr << fromServer.data << std::endl;
    //exit non-zero
    return 2;
  }

  Message join = Message("join", room_name);
  bool joined = conn.send(join);
  conn.receive(fromServer);
  

  //join resulted in error
  if(joined == false || strcmp(fromServer.tag.c_str(), "err") == 0) {
    std::cerr << fromServer.data << std::endl;
    return 3;
  }

  // loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  
  while(true) {
    //is empty at first
    Message received = Message();
    conn.receive(received);

    //sender is 1st word and message is second
    std::stringstream ss;
    ss << received.data;
    //std::getline(ss, word, ':');


    std::vector<std::string> split_msg;
    std::string word;
    while(std::getline(ss, word, ':')){
      split_msg.push_back(word);
    }
    
    std::cout << split_msg[1] << ": " << split_msg[2] << std::endl;

  }


  return 0;
}
