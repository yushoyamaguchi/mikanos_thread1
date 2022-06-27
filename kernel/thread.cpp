#include "thread.hpp"

#include "task.hpp"
#include "asmfunc.h"
#include "segment.hpp"
#include "timer.hpp"


void exec_thread_func(ThreadFunc* f,uint64_t task_id,int64_t data){
    //ここでタイマー関係とか権限関係とかやったあとにfunc呼び出す
}


void thread_create(ThreadFunc* f,int64_t data){
    Task* current=&(task_manager->CurrentTask());
    __asm__("cli");
    Task* new_task=&(task_manager->NewTask());
    new_task->is_thread=true;
    new_task->parent_id=current->ID();
    //ここでいろいろコピー
    const size_t stack_size = new_task->kDefaultStackBytes / sizeof(new_task->stack_[0]);
    new_task->stack_.resize(stack_size);
    uint64_t stack_end = reinterpret_cast<uint64_t>(&new_task->stack_[stack_size]);

    new_task->context_.cr3 = GetCR3();
    new_task->context_.rflags = 0x202;
    new_task->context_.cs = kKernelCS;
    new_task->context_.ss = kKernelSS;
    new_task->context_.rsp = (stack_end & ~0xflu) - 8;

    void (*etfunc)(ThreadFunc* ,u_int64_t,int64_t)=exec_thread_func;
    new_task->context_.rip = reinterpret_cast<uint64_t>(etfunc); 
    new_task->context_.rdi = reinterpret_cast<uint64_t>(f); 
    new_task->context_.rsi = new_task->ID();
    new_task->context_.rdx = data;

    __asm__("sti");
    /*task_manager->NewTask()
          .InitContext(f, 77)
          .Wakeup();*/
}