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
  // add User to the room
  members.insert(user); 
}

void Room::remove_member(User *user) {
  // remove user from collection
  members.erase(user); 
}

// send a message to every (receiver) User in the room
void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  std::set<User *>::iterator it;
  for(it = members.begin(); it != members.end(); ++it){
    if((*it)->sender == false){

      Guard g(lock);
      Message* message = new Message(TAG_DELIVERY, this->room_name + ":" + sender_username + ":" + message_text);
      // add in the message into the queue 
      (*it)->mqueue.enqueue(message); 
    }
  }
}


