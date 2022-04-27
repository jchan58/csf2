#include <cassert>
#include <ctime>
#include "message_queue.h"



MessageQueue::MessageQueue() {
  //initialize the mutex and the semaphore
  //what is the other parameter?
  pthread_mutex_init(&m_lock, nullptr);
  //0 = private, 0 = starting value is 0 (what should it be?)
  sem_init(&m_avail, 0, 0);
  
}

MessageQueue::~MessageQueue() {
  //destroy the mutex and the semaphore
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  //lock/wait while the queue is being changed ?
  //lock_Guard(m_lock);
  //sem_wait(msg.size());
  //put the specified message on the queue
  m_messages.push_front(msg);
  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
  sem_post(&m_avail);
}

Message *MessageQueue::dequeue() {
  struct timespec ts;

  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;

  //call sem_timedwait to wait up to 1 second for a message
  //       to be available, return nullptr if no message is available
  sem_wait(&m_avail);

  if(m_messages.size() == 0){
    return nullptr;
  }
  
  //remove the next message from the queue, return it
  Message *msg = m_messages.back();
  m_messages.pop_back();
  return msg;
}
