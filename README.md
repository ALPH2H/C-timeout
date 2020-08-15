Not deeply tested yet. Not ported to anything else other than windows.  
SetNoCancelTimeout works, tested a few times. Implemented SetCancelTimeout as I'm writing this, no idea if it is bulletproof, should be though, besides probably typos.  
If you only want to use a cancelable timeout from time to time in your code, use SetCancelTimeout function. Otherwise, first run ResizeTimeoutStates so that there is no performance loss from constantly resizing array of states, and then SetMultipleCancelTimeouts. If you are sure you've allocated enough space for the timeouts, for instance by running ResizeTimeoutStates at the very beginning of your code, no need to worry about running it again, and also, make sure to run SetMultipleCancelTimeouts for that neat performance gain from not executing one if statement.  
There is no way to decrease size of timeouts. You can implement your own simple function to do that.  
At first, cancelable timeouts are excluded from the code. You need to define _USE_CTIMEOUT and then use USE_CTIMEOUT function somewhere in ur .c file to make cancelable timeouts available.  
The code is not using atomic operations to sync data across threads, nor is it using a mutex. If you have a problem with it, just don't use. People seem to be afraid of going around atomics and mutexes.  
Code under Apache 2.0 license.
