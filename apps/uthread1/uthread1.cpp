#include <unistd.h>
#include <sys/time.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../syscall.h"


#define BIG_IN_APP 500

void test_func(u_int64_t arg1,int64_t arg2){
    printf("data=%ld",arg2);
    while(1){
        
    }
}

extern "C" void main(){
    void (*f)(u_int64_t,int64_t);
    f=&test_func;
    printf("uthread1\n");
    auto [ret, err] =SyscallThreadCreate((void *)f,77);
    //auto [ret2, err2]=SyscallCR3toApp();
    //printf("cr3=%lx\n",ret2);
    exit(0);
} 