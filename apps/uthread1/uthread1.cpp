#include <unistd.h>
#include <sys/time.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../syscall.h"


#define BIG_IN_APP 500

void test_func(u_int64_t arg1,int64_t arg2){
    SyscallLogString(kWarn,"from user thread\n");
    printf("data=%ld",arg2);
    long int count=0;
    while(1){
        /*count++;
        if(count%500000000==0){
            SyscallLogString(kWarn,"from user thread:loop\n");
        }*/
    }
}

extern "C" void main(){
    void (*f)(u_int64_t,int64_t);
    f=&test_func;
    printf("uthread1\n");
    SyscallLogString(kWarn,"from app\n");
    auto [ret, err] =SyscallThreadCreate((void *)f,77);
    //auto [ret2, err2]=SyscallCR3toApp();
    //printf("cr3=%lx\n",ret2);
    while(1);
    exit(0);
} 