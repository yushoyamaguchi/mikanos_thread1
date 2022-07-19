#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "app_thread.hpp"



int app_thread_join(app_thread_t *thread){
    int64_t task_id=thread->task_id;
    while(1){
        auto [ret, err]=SyscallTaskExist(task_id);
        if(ret==0){
            break;
        }
    }
    return 0;
}

int app_thread_create(app_thread_t* t, void* f, int64_t data){
    //SyscallThreadCreateを呼ぶ
    auto [ret, err] =SyscallThreadCreate((void *)f,77);
    t->task_id=ret;
    return 0;
}