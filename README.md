# cotask
A task based framework that uses co-routines for managing blocking operations within a task based paradigm.  Tasks can be recurring or one off.  When a blocking operation occurs, the current task should co_return and allow the executing thread to process another task in the queue.
