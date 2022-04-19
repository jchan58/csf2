#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

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
  
  // send (is recieve?) rlogin and join messages (expect a response from
  //       the server for each one)
  Message rlogin = Message("rlogin", username);

 //check if send worked
  bool login = conn.send(rlogin);
  //check if server accepted send
  Message fromServer = Message();
  
  conn.recieve(fromServer);

  if (conn.send(rlogin) == false || strcmp(fromServer.tag, "err") == 0) {
    //so print the server payload
    std::cerr << fromServer.data;
    //exit non-zero
    return 2;
  }

  Message join = Message("join", room_name);
  bool join = conn.send(join);
  conn.receive(fromServer)
  

  //join resulted in error
  if(conn.send(join) == false || strcmp(fromServer.tag, "err") == 0) {
    std::cerr << fromServer.data;
    return 3;
  }

  // loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  std::string input;
  cin >> input;
  while(true) {
    //is empty at first
    Message received = Message();
    conn.receive(received);

    //print username: message
    int colon_two = 0;

    //get the index of the second colon
    for(int i = 5; i < sizeof(msg.data); i++) {
      if(msg.data[i] == ':') {
        colon_two = i;
        break;
      }
    }

    std::string username = received.data.substr(5, colon_two);
    std:: string message = received.data.substr(colon_two + 1, sizeof(received.data));

    std::cout << username ": " message;

  }


  return 0;
}
