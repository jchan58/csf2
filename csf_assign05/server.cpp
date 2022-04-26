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
Room * room_list;
int num_rooms;

//hold info about the connection object (from lecture 30, slide 24)
typedef struct ConnInfo {
  int clientfd;
  //const char *webroot; //what is webroot, are we not passing in the whole object like the instructions say?
  Server *server;
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
  struct ConnInfo *info = (ConnInfo *) arg;



  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  bool sender = false;
  bool receiver = false;
  Connection conn(info->clientfd);
  Message received = Message();
  conn.receive(received);

  if(strcmp(received.tag.c_str(), "slogin") == 0){
    sender = true; 
  } else if(strcmp(received.tag.c_str(), "rlogin") == 0){
    receiver = true; 
  } else {
    // TODO: send an error response back to the client
    Message error = Message("err", "error");
    bool err = conn.send(error);
  }

  std::string username = received.data;

  //"infinite loops"
  if(sender){
    chat_with_sender(&conn, info->server, username); 
    //check for join
    //make room
    //handle diff commands
  } else if(receiver){
    chat_with_receiver(&conn, info->server, username); 
    //make user
    //check if it exists if client socket still exists
    //handle diff commands
    //deque messages from correct message queue
  }

  return nullptr;
}

void chat_with_sender(*conn, Server server, string username){
  Message received = Message();
  bool joined = false; 
  conn.receive(received);
   if(strcmp(received.tag.c_str(), "join") == 0){
     //add them to a room that already exists or doesn't 
     joined = true; 
   } else {
    Message error = Message("err", "error");
    bool err = conn.send(error);
   }
  }
 }


 void chat_with_receiver(*conn, Server server, string username){
   Message received = Message();
   conn.receive(received);
   bool joined = false; 
   if(strcmp(received.tag.c_str(), "join") == 0){
     //add them to a room that already exists or doesn't depending on the case

     joined = true; 
   } else {
    Message error = Message("err", "error");
    bool err = conn.send(error);
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


bool Server::listen() {
  //use open_listenfd to create the server socket, return true
  //       if successful, false if not

 std::stringstream ss;
 ss << m_port;
 std::string str_port = ss.str();

 m_ssock = open_listenfd(str_port.c_str());
 
  if(m_ssock >= 0) {
   return true; 
  } else {
   return false; 
  }
}

//from lecture 30, slide 26 ???
void Server::handle_client_requests() {
  //infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while(true){
   int clientfd = Accept(serverfd, NULL, NULL);
   if (clientfd < 0) {
     std::cerr << "Error accepting client connection" << std::endl;
     return;
   } 

   ConnInfo *info = malloc(sizeof(ConnInfo));
   info->clientfd = clientfd;
  
   pthread_t thr_id;

   if(pthread_create(&thr_id, NULL, worker, info) != 0) {
     std::cerr << "pthread_create failed" << std::endl;
     return;
   }

   close(clientfd);
  }
}

//??very confused about how all these make server communicate with the clients
//how do we find existing rooms? is there a list of them somewhere? do we make a room list
Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  
}
