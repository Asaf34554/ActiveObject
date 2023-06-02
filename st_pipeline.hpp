#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>

typedef void(*ffunc)(void *&);
typedef struct _Task
{
    
    std::queue<void*> numQueue;
    std::mutex mtx;
    std::condition_variable cv;
    void enqueue(void* item) {
        std::lock_guard<std::mutex> lock(mtx);
        numQueue.push(item);
        cv.notify_one();
    }

    void* dequeue() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !numQueue.empty(); });
        if(numQueue.front() == nullptr) return nullptr;
        void* item = numQueue.front();
        numQueue.pop();
        return item;
    }
    
}Task;
class ActiveObject{
    
    public:
        Task* que;
        ffunc func;
        std::thread exac;

};
void* CreateActiveObject(ffunc func);
void* getQueue(void*);
void stop(void*);
unsigned int GetNum();
void task_handler(ActiveObject*,ffunc);
int isPrime(unsigned int); 


void generatenums(void*&); //ao1
void isPrime1(void*&); //ao2
void isPrime2(void*&); //ao3
void lastFunc(void*&); //ao4