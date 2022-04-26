#include <pthread.h>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
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
#include <map>
#include <iterator>

using std::map;
using std::iterator;


////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

//hold info about the connection object (from lecture 30, slide 24)
typedef struct ConnInfo {
  int clientfd;
  //const char *webroot; //what is webroot, are we not passing in the whole object like the instructions say?
  Server *server;
} ConnInfo;

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////


Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  //first find if the room exists already - if not create a room
  //create an iterator to iterate over the map
  std::map<std::string, Room *>::iterator it;
  it = m_rooms.find(room_name);
  if(it != m_rooms.end()){
    return it->second;
  } else {
    //create a new room and add it to the map
      Room* new_room = new Room(room_name);
      m_rooms.emplace(room_name, new_room);
      return new_room;
  }
}



void chat_with_sender(Connection *conn, Server server, std::string username){
  Message received = Message();
  bool joined = false;
  conn->receive(received);
  while(true){
   if(strcmp(received.tag.c_str(), "join") == 0){
     Room* room = find_or_create_room(received.data.c_str());
     //create the user and then have them join the room
     User user;
     user.username = username;
     user.sender = true; 
     joined = true;
   } else {
    Message error = Message("err", "error");
    bool err = conn->send(error);
   }
  }
 }


void chat_with_receiver(Connection *conn,  std::string username, std::string &room_name){
  //find room/create one if it does not exists
  Room* room = find_or_create_room(room);
  //join room
  room.add_member(room.user);
  //now user is in room

  Message ok = Message("ok", username);
  conn.send(ok);

  
  //deliver messages
    while(true){
      Message msg = room.user.deque();
      bool sent = conn.send(msg);
      if(sent){
	 Message ok = Message("ok", username);
	 conn.send(ok);
      } else {
	 Message error = Message("err", "error");
	 conn.send(error);
      }
      delete(msg);
    }
}


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
    //send an error response back to the client
    Message error = Message("err", "error");
    conn.send(error);
  }

  std::string username = received.data;
  Message ok = Message("ok", username);
  conn.send(ok);

  if(sender){
    chat_with_sender(&conn, info->server, username); 
    //check for join
    //make room
    //handle diff commands
  } else if(receiver){
    Message join = Message();
    conn.receive(join);
    //join.data is the room name
    chat_with_receiver(&conn, username, join.data); 
  }

  return nullptr;
 }
}
////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  //initialize mutex
  //what is the other parameter?
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server() {
  //destroy mutex
  pthread_mutex_destroy(&m_lock);
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
   int clientfd = Accept(m_ssock, NULL, NULL);
   if (clientfd < 0) {
     std::cerr << "Error accepting client connection" << std::endl;
     return;
   } 

   ConnInfo *info = (ConnInfo *) malloc(sizeof(ConnInfo));
   info->clientfd = clientfd;
  
   pthread_t thr_id;

   if(pthread_create(&thr_id, NULL, worker, info) != 0) {
     std::cerr << "pthread_create failed" << std::endl;
     return;
   }

   close(clientfd);
  }
}
