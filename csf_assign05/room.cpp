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
  pthread_mutex_init(&lock, nullptr);
}

Room::~Room() {
   //destroy mutex
   pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  Guard g(lock);
  members.insert(user); 
}

void Room::remove_member(User *user) {
  // remove user from collection
  Guard g(lock);
  members.erase(user); 
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // send a message to every (receiver) User in the room
  Guard g(lock);
  std::set<User *>::iterator it;
  for(it = members.begin(); it != members.end(); ++it){
    if((*it)->sender == false){
      Message* message = new Message(TAG_DELIVERY, this->room_name + ":" + sender_username + ":" + message_text);
      (*it)->mqueue.enqueue(message); 
    }
  }
}


