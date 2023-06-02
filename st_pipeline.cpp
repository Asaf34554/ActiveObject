#include <cmath>
#include <pthread.h>
#include <queue>
#include <iostream>
#include <random>
#include <time.h>
#include "st_pipeline.hpp"


using namespace std;

ActiveObject* ao1,*ao2,*ao3,*ao4;


int isPrime(unsigned int val){
    if(val % 2 == 0 || val < 2){
        return 0;
    }
    for (unsigned int i = 2; i <= sqrt(val); ++i) {
        if (val % i == 0) {
            return 0;
        }
    }
    return 1;
}
void isPrime1(void*& task){
    //first print the value
    if(task == nullptr){
        Task* to3 = (Task*)getQueue(ao3);
        to3->enqueue(task);
        return;
    }
    int* prt = reinterpret_cast<int*>(task);
    cout << *prt<< endl;
    if(isPrime((unsigned int)*prt) == 0){
        cout << "false"<<endl; 
    }
    else cout << "true" <<endl;
    *prt+=11;
    task = prt;
    Task* to3 = (Task*)getQueue(ao3);
    to3->enqueue(task);
}

void isPrime2(void*& task){
    //first print the value
    if(task == nullptr){
        Task* to4 = (Task*)getQueue(ao4);
        to4->enqueue(task);
        return;
    }
    int* prt = reinterpret_cast<int*>(task);
    cout << *prt << endl;
    if(isPrime((unsigned int)*prt) == 0){
        cout << "false"<<endl; 
    }
    else cout << "true" <<endl;
    *prt-=13;
    task = reinterpret_cast<void*>(prt);
    Task* to4 = (Task*)getQueue(ao4);
    to4->enqueue(task);
}

void lastFunc(void*& task){
    if(task == nullptr){
        return;
    }
    int* prt = reinterpret_cast<int*>(task);
    cout << *prt << endl;
    *prt+=2;
    cout << *prt << endl;
}

void* CreateActiveObject(ffunc func){
    ActiveObject* ao = new ActiveObject();
    ao->func = (ffunc)func;
    ao->que = new Task(); // Allocate memory for the Task structure
    ao->exac = thread(task_handler,ao,func);
    return ao;

}

void task_handler(ActiveObject* ao, ffunc func) {
    void* task;
    while ((task = ao->que->dequeue())) {
        
        func(task);  // Call the provided function with the task
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    func(task);
    stop(ao);
    
}

void* getQueue(void* ao){
    ActiveObject * ans = (ActiveObject*) ao;
    return ans->que;
}

void stop(void* ao) {
    ActiveObject* ans = (ActiveObject*)ao;
    ans->que->enqueue(nullptr);  // Enqueue a nullptr task
    delete ans->que;
}



void generatenums(void*& task){
    if(task == nullptr){
        Task* to2 = (Task*)getQueue(ao2);
        to2->enqueue(task);
        return;
    }
    unsigned int seed = (unsigned int)(intptr_t)task;
    mt19937_64 generator(seed);  // Use the seed as the generator's seed
    uniform_int_distribution<int> distribution(100000, 999999);
    int* num = new int(distribution(generator));
    task = num; // Assign the pointer to num
    Task* to2 = (Task*)getQueue(ao2);
    to2->enqueue(task);                   
}

int main(int argc,char* argv[]){
    if(argc <=1 || argc > 3){
        perror("Usage:./st_pipeline <Num of Tasks> (optional)<Seed>\n");
        exit(1);
    } 
    int numT = stoi(argv[1]),numS = time(NULL);
    if(argc == 3){
        numS = stoi(argv[2]);
    }
    ao1 = (ActiveObject*)CreateActiveObject(generatenums);
    ao2 = (ActiveObject*)CreateActiveObject(isPrime1);
    ao3 = (ActiveObject*)CreateActiveObject(isPrime2);
    ao4 = (ActiveObject*)CreateActiveObject(lastFunc);
    int* taskSeed = new int(numS);
    if (ao1 != nullptr) {  // Check if ao1 is not nullptr before accessing its members
        for (int i = 0; i < numT; ++i) {
            
            ao1->que->enqueue(&(*taskSeed++));

            
        }
        ao1->que->numQueue.push(nullptr);
    }

    // Wait for threads to finish
    if (ao1 != nullptr) {
        ao1->exac.join();
    }
    if (ao2 != nullptr) {
        ao2->exac.join();
    }
    if (ao3 != nullptr) {
        ao3->exac.join();
    }
    if (ao4 != nullptr) {
        ao4->exac.join();
    }
    delete ao1;
    delete ao2;
    delete ao3;
    delete ao4;

    return 0;
}