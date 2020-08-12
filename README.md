# thread
A C++17 library providing asynchronous execution and persistent threading.

## USAGE
Initiate an async instance.
```cpp
thr::Async async;
```
Run async functions, wait for their result whenever.
```cpp
async([](int x){ return x * 2; }, 44);
async([](int x){ return x * 2; }, 44).get();
```
