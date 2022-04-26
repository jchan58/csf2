#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include <iterator>
#include <set>
#include <iostream>
#include <cstring>

using std::iterator;
using std::set; 

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  //initialize the mutex
  pthread_mutex_init(&lock, NULL);
}

Room::~Room() {
   //destroy mutex
   pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  members.insert(user); 
}

void Room::remove_member(User *user) {
  // remove user from collection
  members.erase(user); 
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
  Message message = Message("sendall", message_text);
  std::set<User *>::iterator it;
  for(it = members.begin(); it != members.end(); ++it){
    if((*it)->sender == false){
      (*it)->mqueue.enqueue(&message); 
    }
  }
}

Message& Room::take_message() {
  Message msg;
  // take the message out of every reciever's message queue in the room
  std::set<User *>::iterator it;
  for(it = members.begin(); it != members.end(); ++it){
    if( (*it)->sender == false){
      msg = (*it)->mqueue.dequeue();
    }
  }
  return msg;
}
