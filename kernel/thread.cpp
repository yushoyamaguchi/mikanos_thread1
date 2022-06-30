#include "thread.hpp"

#include "task.hpp"
#include "asmfunc.h"
#include "segment.hpp"
#include "timer.hpp"


void exec_thread_func(ThreadFunc* f,uint64_t task_id,int64_t data){
    //ここでターミナル紐づけ関係とか権限関係とかやったあとにfunc呼び出す
    /*ToDo
    親taskのfile関係とデマンドページング関係の変数をコピーしてくる
    呼び出し元terminalのlayer_id取得してlayer_task_mapに追加
    callAppでアプリケーション側の関数fを呼び出し
    親が終わったら子も終わる設定
    */
   printk("exec_thread_func: data=%ld\n",data);
   task_manager->Sleep(task_id);
   return;
}


void dummy_thread_func(int64_t i){
    printk("dummy thread func\n");
    Task& task = task_manager->CurrentTask();
    __asm__("sti");
    task.Sleep();
    return;
}



void thread_create(ThreadFunc* f,int64_t data){
    __asm__("cli");
    Task* current=&(task_manager->CurrentTask());
    Task* new_task=&(task_manager->NewTask());
    __asm__("sti");
    new_task->is_thread=true;
    new_task->parent_id=current->ID();
    //ここでいろいろコピー
    const size_t stack_size = new_task->kDefaultStackBytes / sizeof(new_task->stack_[0]);
    new_task->stack_.resize(stack_size);
    uint64_t stack_end = reinterpret_cast<uint64_t>(&new_task->stack_[stack_size]);

    memset(&(new_task->context_), 0, sizeof(new_task->context_));
    new_task->context_.cr3 = GetCR3();
    printk("thread_create : cr3=%lx\n",new_task->context_.cr3);
    new_task->context_.rflags = 0x202;
    new_task->context_.cs = kKernelCS;
    new_task->context_.ss = kKernelSS;
    new_task->context_.rsp = (stack_end & ~0xflu) - 8;

    void (*etfunc)(ThreadFunc* ,u_int64_t,int64_t)=exec_thread_func;
    new_task->context_.rip = reinterpret_cast<uint64_t>(etfunc);
    new_task->context_.rdi = reinterpret_cast<uint64_t>(f); 
    new_task->context_.rsi = new_task->ID();
    new_task->context_.rdx = data;

    task_manager->Wakeup(new_task);
    
}



