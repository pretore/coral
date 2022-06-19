## Lifecycle
There are objects that are used by the framework that do not use 
reference counting, but combined with `coral_object` have established the 
following vocabulary:
* `*_alloc` - heap allocated instance.
* `*_destroy` - invalidate instance and deallocate from heap.
* `*_init` - initialize the instance before use.
* `*_invalidate` - invalidate object instance only.

Heap only instances of `coral_object` and its subtypes will not have the 
`*_invalidate` method as there is no use case where you will invalidate the 
instance and not deallocate it from the heap.

### `coral_object`
The following process outlines how to create and dispose of `coral_object` 
instances.
1. `coral_*_alloc(void**)` allocate space on the heap.
   * **on failure**: do nothing as there is nothing allocated.
2. `coral_*_init(void*,...)` initialize the instance before use.
   * **on failure**: You must call `coral_*_destroy(void*)` to release the  
     possibly _partially initialized_ instance.
   * **on success**: You must only use the reference counting
     methods of `coral_*_retain(void*)`, `coral_*_release(void*)` and
     `coral_*_autorelease(void*)` to manage the lifetime of the instance.