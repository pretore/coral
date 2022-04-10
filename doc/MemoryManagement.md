##Memory Management
We use reference counting to manage the lifetime of every *initialized* 
`coral_object`.

Each `coral_object` will have `coral_*_retain(void*)`, `coral_*_release
(void*)` and `coral_*_autorelease(void*)` methods.

###`coral_*_retain(void*)`
Extend the lifetime of the instance by increasing the reference count.

###`coral_*_release(void*)`
Decrease the reference count and once it reaches zero the instance becomes 
*unowned* and is destroyed.

###`coral_*_autorelease(void*)` 
Extend the lifetime of the instance so that it can safely be returned from the 
**current** function without being at risk of being destroyed.

###`coral_autorelease_pool`
Any thread, where `coral_object` functions are called, will have an implicit 
`coral_autorelease_pool` created. Upon returning from those methods, all the 
*unowned* `coral_object` instances created in the returning method call 
will be destroyed.

It is expected that one of the final functions invoked before exiting the 
thread will be `coral_autorelease_pool_drain()`. This will give the 
framework an opportunity to destroy any *unowned* `coral_object` instances 
which is only visible to the exiting thread and thereby prevent a memory leak.