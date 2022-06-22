#include "thread.hpp"

#include "task.hpp"
#include "asmfunc.h"
#include "segment.hpp"
#include "timer.hpp"


void thread_create(TaskFunc* f){
    task_manager->NewTask()
          .InitContext(f, 77)
          .Wakeup();
}