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

  //response was error (is this how?)
  if (conn.send(rlogin) == false) {
    //so print the payload
    std::cerr << rlogin.data;
    //exit non-zero
    return 2;
  }

  Message join = Message("join", room_name);

  //join resulted in error
  if(conn.send(join) == false) {
    //server error will check bytes back to determine error
    std::cerr << join.data;
    return 3;
  }

  // loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  std::string input;
  cin >> input;
  while(true) {
    //check tag?
    conn.receive();
  }


  return 0;
}
