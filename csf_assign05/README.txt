README.txt Milestone 1

Joey Chan

I worked on message.h, sender.cpp, server.cpp, message_queue.cpp, and room.cpp. 


Sola Oladosu

I worked on connection.cpp and reciever.cpp, server.cpp, message_queue.cpp, and room.cpp.


--Synchronization Report--

We decided to create critical sections with mutexes where multiple threads could edit data at once or where multiple threads could read and edit data at once. So, we made the body of find_or_create_room() , the body of broadcast_message(), the body of enqueue(), the section of dequeue() that removes a message, the body of remove_member(), and the body of add_member() critical sections. At the end of enqueue(), we then use a semaphore to notify threads that there is a message that can be dequeued. So, in dequeue(), we use a semaphore to wait for this notification before dequeing. This prevents dequeueing from an empty queue, which would be problematic. To prevent deadlocks, we used Guard’s, as they automatically unlock at the end of the scope.




