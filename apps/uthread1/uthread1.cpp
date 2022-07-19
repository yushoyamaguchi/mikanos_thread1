#include <unistd.h>
#include <sys/time.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>


#include "../app_thread.hpp"


#define BIG_IN_APP 500

void test_func(int64_t arg1){
    SyscallLogString(kWarn,"from user thread\n");
    printf("test_from_thread:data=%ld\n",arg1);

    SyscallExit(0);
}

extern "C" void main(){
    void (*f)(int64_t);
    f=&test_func;
    printf("uthread1\n");
    app_thread_t t;
    app_thread_create(&t,(void*)f,77);
    printf("task_id=%ld\n",t.task_id);
    app_thread_join(&t);
    exit(0);
} 