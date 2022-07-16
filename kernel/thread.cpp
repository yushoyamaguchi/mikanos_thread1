#include "thread.hpp"

#include "task.hpp"
#include "asmfunc.h"
#include "segment.hpp"
#include "timer.hpp"

int num_of_thread=0;


void exec_thread_func(ThreadFunc* f,uint64_t task_id,int64_t data){
    /*ここでファイル関係の設定(ターミナルとの紐づけ)
    アプリのスタックを構築
    callAppでfunc呼び出す
    関数終わったらtaskも終わる設定
    親が終わったら子も終わる設定
    */
    //こっちだったらエラーは起こらない
    /*printk("exec_thread_func: data=%ld\n",data);
    task_manager->Sleep(task_id);*/
    Task* child=task_manager->GetTaskFromID(task_id);
    Task* parent=task_manager->GetTaskFromID(child->parent_id);
    const int stack_size = 16 * 4096;
    num_of_thread++;
    LinearAddress4Level stack_frame_addr{0xffff'ffff'ffff'f000 - (stack_size)*(num_of_thread+1)};
    // #@@range_end(increase_appstack)
    if (auto err = SetupPageMaps(stack_frame_addr, stack_size / 4096)) {
        printk("thread exec func : stack page map err\n");
        while(1) __asm__("hlt");
        return ;
    }
    /*while(1)__asm__("hlt");
    task_manager->Sleep(task_id); */
    for (int i = 0; i < parent->files_.size(); ++i) {
        child->Files().push_back(parent->files_[i]);
    }
    printk("child=%ld,parent=%ld\n",child->ID(),parent->ID());
    child->SetDPagingBegin(parent->DPagingBegin());
    child->SetDPagingEnd(parent->DPagingEnd());
    printk("thread exec func : before call app\n");
    int ret = CallAppforThread(data, 3 << 3 | 3, reinterpret_cast<uint64_t>(f),
                    stack_frame_addr.value + stack_size - 8,
                    &(child->OSStackPointer()));

    while(1)__asm__("hlt");
    task_manager->Sleep(task_id);             
    return;
}


void dummy_thread_func(int64_t i){
    printk("dummy thread func\n");
    __asm__("cli");
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



