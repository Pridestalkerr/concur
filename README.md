# thread
A C++17 library providing asynchronous execution and persistent threading.

## Installation (optional)
```bash
$ make install
```

## Usage
Include the header.
```cpp
#include <concur>                // if you've installed the library
#include "./include/concur.hpp"  // if you haven't
```
Initiate an async instance.
```cpp
con::Async async;
```
Run async functions, wait for their result whenever.
```cpp
async([](int x){ return x * 2; }, 44);
async([](int x){ return x * 2; }, 44).get();
```
