cmake_minimum_required(VERSION 3.5)
project(ProxyOnThreads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(include)
set(SOURCE_FILES src/main.cpp src/CacheEntry.cpp  src/MutexGuard.cpp src/Mutex.cpp src/CacheManager.cpp src/CondVar.cpp src/HTTPProxy.cpp src/Client.cpp src/Request.cpp src/Downloader.cpp src/Response.cpp)
add_executable(ProxyOnThreads ${SOURCE_FILES})
