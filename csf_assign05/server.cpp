#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"


////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions
int server_fd, new_socket, valread;
struct sockaddr_in address;


//hold info about the connection object (from lecture 30, slide 24)
typedef struct ConnInfo {
  int clientfd;
  const char *webroot; //what is webroot, are we not passing in the whole object like the instructions say?
} ConnInfo;

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

  //use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  struct ConnInfo *info = arg;

  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  
  
  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)

  return nullptr;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
}

Server::~Server() {
  // TODO: destroy mutex
}

//??
server_fd = Socket(AF_INET, SOCK_STREAM, 0);
setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
bind(server_fd, (struct sockaddr*) &address, sizeof(address));

bool Server::listen() {
  //use open_listenfd to create the server socket, return true
  //       if successful, false if not

 std::stringstream ss;
 ss << m_port;
 std::string str_port = ss.str();

  if(open_listenfd(str_port.c_str())) {
   return true; 
  } else {
   return false; 
  }
}

//from lecture 30, slide 26 ???
void Server::handle_client_requests() {
  //infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  int clientfd = Accept(serverfd, NULL, NULL);
  if (clientfd < 0) {
    fatal("Error accepting client connection");
  }

  ConnInfo *info = malloc(sizeof(ConnInfo));
  info->clientfd = clientfd;
  //where is this from?????????????????
  info->webroot = webroot;

  pthread_t thr id;

  if(pthread_create(&thr_id, NULL, worker, info) != 0) {
    fatal("pthread_create failed");
  }
}

//??very confused about how all these make server communicate with the clients
Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
}
