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
  //hold info about the connection object
  Connection *conn;
  Server *server;

  ConnInfo(Connection *conn, Server *server) : conn(conn), server(server) { }
  ~ConnInfo() {
    // destroy connection when ConnInfo object is destroyed
    delete conn;
  }

} ConnInfo;


////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////


Room *Server::find_or_create_room(const std::string &room_name) {
  //return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  //first find if the room exists already - if not create a room
  //create an iterator to iterate over the map


  // this function can be called from multiple threads, so
  // make sure the mutex is held while accessing the shared
  // data (the map of room names to room objects)
  Guard g(m_lock);

  Room *room;

  auto i = m_rooms.find(room_name);
  if (i == m_rooms.end()) {
    // room does not exist yet, so create it and add it to the map
    room = new Room(room_name);
    m_rooms[room_name] = room;
  } else {
    room = i->second;
  }

  return room;
  
}



void chat_with_sender(Connection *conn, std::string username, ConnInfo* info){
  User* user = new User(username); 
  user->username = username;
  user->sender = true;
  Room* room = nullptr;
  bool joined = false; 
  user->room = nullptr;
  
  while(true){
    Message received = Message();
    conn->receive(received);
    if(strcmp(received.tag.c_str(), TAG_JOIN) == 0 && user->room == nullptr){
      room = info->server->find_or_create_room(received.data);
     //have the user join the room
     room->add_member(user);
     user->room = room;
     joined = true; 
     Message ok = Message(TAG_OK, username);
     conn->send(ok);
     //broadcast the message to all the queues
   } else if(strcmp(received.tag.c_str(), TAG_SENDALL) == 0 && joined == true){
      user->room->broadcast_message(username, received.data.c_str());
      Message ok = Message(TAG_OK, username);
     conn->send(ok);
   } else if(strcmp(received.tag.c_str(), TAG_LEAVE) == 0 && joined == true){
     user->room->remove_member(user);
     user->room = nullptr;  
     Message ok = Message(TAG_OK, "ok");
     conn->send(ok);
     joined = false; 
     //in order to quit must leave the room first 
   } else if(strcmp(received.tag.c_str(), TAG_QUIT) == 0){
      if(user->room != nullptr){
	user->room->remove_member(user);
      }
      
      if(room != nullptr){
	delete(room);
      }
     user->room = nullptr;
     Message ok = Message(TAG_OK, "ok");
     conn->send(ok);
     joined = false; 
     delete(user);
     //delete(info);
     break; 
   }
  }
 }


void chat_with_receiver(Connection *conn,  std::string& username, std::string &room_name, ConnInfo* info){
  User* user = new User(username);
  //find room/create one if it does not exists
  Room* room = info->server->find_or_create_room(room_name);
  //join room
  room->add_member(user);
  //now user is in room

  Message ok = Message("ok", username);
  conn->send(ok);

  Message* msg = nullptr;
  
  //deliver messages
    while(true){
      msg = user->mqueue.dequeue();
      //Message& msg_ref = msg;
      bool sent = conn->send(*msg);
      if(sent){
	 Message ok = Message("ok", username);
	 conn->send(ok);
      } else {
	 delete(user);
	 delete(room);
	 //delete(info);
	 break;	 
      }
      
      if(msg != nullptr){
	delete(msg);
      }
    }
}
 

namespace {

void *worker(void *arg) {
  bool sender, receiver;

  pthread_detach(pthread_self());
  //use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  ConnInfo *info_ = static_cast<ConnInfo *>(arg);

  // use a std::unique_ptr to automatically destroy the ConnInfo object
  // when the worker function finishes; this will automatically ensure
  // that the Connection object is destroyed
  std::unique_ptr<ConnInfo> info(info_);


Message msg;

  if (!info->conn->receive(msg)) {
    if (info->conn->get_last_result() == Connection::INVALID_MSG) {
      info->conn->send(Message(TAG_ERR, "invalid message"));
    }
    return nullptr;
  }

  if (msg.tag != TAG_SLOGIN && msg.tag != TAG_RLOGIN) {
    info->conn->send(Message(TAG_ERR, "first message should be slogin or rlogin"));
    return nullptr;
  }

  std::string username = msg.data;
  if (!info->conn->send(Message(TAG_OK, "welcome " + username))) {
    return nullptr;
  }


  //ours
  if(strcmp(msg.tag.c_str(), TAG_SLOGIN) == 0){
    sender = true;
  } else if(strcmp(msg.tag.c_str(), TAG_RLOGIN) == 0){
    receiver = true; 
  }

  
  if(sender){
    chat_with_sender(&(*info->conn), username, &(*info)); 
    //check for join
    //make room
    //handle diff commands
  } else if(receiver){
    Message join = Message();
    info->conn->receive(join);
    //join.data is the room name
    chat_with_receiver(&(*info->conn), username, join.data, &(*info)); 
  }

  delete(info_);
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
  std::string port = std::to_string(m_port);
  m_ssock = open_listenfd(port.c_str());
  return m_ssock >= 0;  
 
}


void Server::handle_client_requests() {
  //infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client

  assert(m_ssock >= 0);
  while(true){
   int clientfd = Accept(m_ssock, nullptr, nullptr);
   if (clientfd < 0) {
     std::cerr << "Error accepting client connection" << std::endl;
     return;
   } 

   ConnInfo *info = new ConnInfo(new Connection(clientfd), this);
   
  
   pthread_t thr_id;

   if(pthread_create(&thr_id, nullptr, worker, static_cast<void *>(info)) != 0) {
     std::cerr << "pthread_create failed" << std::endl;
     return;
   }
  }
}
