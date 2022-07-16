#include <unistd.h>
#include <sys/time.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../syscall.h"


#define BIG_IN_APP 500

void test_func(int64_t arg1){
    SyscallLogString(kWarn,"from user thread\n");
    printf("test_from_thread:data=%ld\n",arg1);

    long int count=0;
    while(1){
        /*count++;
        if(count%500000000==0){
            SyscallLogString(kWarn,"from user thread:loop\n");
        }*/
    }
}

extern "C" void main(){
    void (*f)(int64_t);
    f=&test_func;
    int64_t data =5;
    printf("uthread1\n");
    SyscallLogString(kWarn,"from app\n");
    auto [ret, err] =SyscallThreadCreate((void *)f,77);
    //auto [ret2, err2]=SyscallCR3toApp();
    //printf("cr3=%lx\n",ret2);
    while(1);
    exit(0);
} 