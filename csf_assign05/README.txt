README.txt Milestone 1

Joey Chan

I worked on message.h, sender.cpp, server.cpp, message_queue.cpp, and room.cpp. 


Sola Oladosu

I worked on connection.cpp and reciever.cpp, server.cpp, message_queue.cpp, and room.cpp.


--Synchronization Report--
In server.cpp, we made the body of find_or_create_room() a critical section. This is done because if we remove a room while looking for it, it could get the room before it is removed and return a room that no longer exists. Here, we used a guarded mutex because it will automatically unlock the section after the critical section passes, preventing deadlocks.
In room.cpp, we made the section of broadcast_message() that adds a new message to a message queue a critical section. We did this to prevent threads from editing the queue at the same time. Attempting to add multiple elements to the same spot could create issues.  We choose to use a guarded mutex again to automatically unlock after the critical section and prevent deadlocks.
In the enqueue section, we used a mutex to lock the enqueue function when it is pushing a new message into the queue. We did this to prevent multiple threads from adding to a message queue at the same spot at once, which would create a writing race condition.
We then used a semaphore to notify the threads that there is a message that can be dequeued. In messagequeue.cpp()’s dequeue() function, we used semaphore to wait for this notification before dequeuing. This prevents us dequeueing from an empty queue, which would cause problems. Then, we use a lock to create a critical section when removing a message from the queue. This prevents a thread from trying to read a message while it is getting removed, which would cause problems. 
