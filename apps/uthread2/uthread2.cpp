#include <unistd.h>
#include <sys/time.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../app_thread.hpp"


int num = 0;


void input(int64_t arg){
    char buffer[32];
    for(;;){
        SyscallIntrLock();
        if (fgets(buffer, sizeof(buffer), stdin) == NULL || buffer[0] == '\n') {
            SyscallIntrUnlock();
            SyscallExit(-1);
        }
        num=atoi(buffer);
        SyscallIntrUnlock();
        if (num == -1) {
            break;
        }
    }
    SyscallExit(0);
}


void output(int64_t arg){
    for(;;){
        if (num == -1) {
            break;
        }
        printf("num = %d\n", num);
    }
    SyscallExit(0);
}


extern "C" void main(){
    void (*f)(int64_t);
    f=&input;
    void (*f2)(int64_t);
    f2=&output;
    printf("uthread2\n");
    app_thread_t t,t2;
    app_thread_create(&t,(void*)f,1);
    app_thread_create(&t2,(void*)f2,1);
    app_thread_join(&t);
    app_thread_join(&t2);
    exit(0);
} 