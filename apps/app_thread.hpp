#pragma once

#include "syscall.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


struct app_thread_t {
  uint64_t task_id;
};

int app_thread_join(app_thread_t *thread);

int app_thread_create(app_thread_t* t, void* f, int64_t data);

#ifdef __cplusplus
}
#endif