#include <cassert>
#include <ctime>
#include "message_queue.h"
#include "guard.h"


MessageQueue::MessageQueue() {
  //initialize the mutex and the semaphore
  //what is the other parameter?
  pthread_mutex_init(&m_lock, nullptr);
  //0 = private, 0 = starting value is 0
  sem_init(&m_avail, 0, 0);
  
}

MessageQueue::~MessageQueue() {
  //destroy the mutex and the semaphore
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  //lock/wait while the queue is being changed ?

  Guard g(m_lock);
  
  //put the specified message on the queue
  m_messages.push_front(msg);
  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
  sem_post(&m_avail);
}

Message *MessageQueue::dequeue() {
  sem_wait(&m_avail);

  if(m_messages.size() == 0){
    return nullptr;
  }
  
  Guard g(m_lock);
  //remove the next message from the queue, return it
  Message *msg = m_messages.back();
  m_messages.pop_back();
  return msg;
}
