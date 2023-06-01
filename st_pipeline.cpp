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
void isPrime1(void* task){
    //first print the value
    unsigned int prt = reinterpret_cast<intptr_t>(task);
    cout << prt << endl;
    if(isPrime(prt) == 0){
        cout << "false"<<endl; 
    }
    else cout << "true" <<endl;
    prt+=11;
    task = reinterpret_cast<void*>(prt);
    Task* to3 = (Task*)getQueue(ao3);
    to3->enqueue(task);
}

void isPrime2(void* task){
    //first print the value
    unsigned int prt = reinterpret_cast<intptr_t>(task);
    cout << prt << endl;
    if(isPrime(prt) == 0){
        cout << "false"<<endl; 
    }
    else cout << "true" <<endl;
    prt-=13;
    task = reinterpret_cast<void*>(prt);
    Task* to4 = (Task*)getQueue(ao4);
    to4->enqueue(task);
}

void lastFunc(void* task){
    unsigned int prt = (unsigned int)(intptr_t)task;
    cout << prt << endl;
    prt+=2;
    cout << prt << endl;
}

void* CreateActiveObject(ffunc func){
    ActiveObject* ao = new ActiveObject();
    ao->func = (ffunc)func;
    ao->exac = thread(task_handler,ao,func);
    // ao->exac.join();
    return ao;

}

void task_handler(ActiveObject* ao, ffunc func) {
    void* task;
    while ((task = ao->que->dequeue())) {
        func(task);  // Call the provided function with the task
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}
void* getQueue(void* ao){
    ActiveObject * ans = (ActiveObject*) ao;
    return ans->que;
}

void stop(void* ao){
    ActiveObject * ans = (ActiveObject*) ao;
    while(!(ans->que->numQueue.empty()))
        ans->que->numQueue.pop();
    ans->exac.detach();

}


void generatenums(void* task){

    mt19937_64 generator((unsigned int)(intptr_t)task);  // Use the seed as the generator's seed
    uniform_int_distribution<int> distribution(100000, 999999);
    int num = distribution(generator);
    cout << num << endl;
    task = &num;
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
    for(int i=0;i<numT;++i){
        ao1->que->enqueue(&numS);
    }
}