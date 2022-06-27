#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <map>
#include <optional>
#include <vector>

#include "error.hpp"
#include "message.hpp"
#include "paging.hpp"
#include "fat.hpp"
#include "debug_print.hpp"
#include "task.hpp"

using ThreadFunc = void ( int64_t);

void exec_thread_func(ThreadFunc* f,uint64_t task_id,int64_t data);

void thread_create(ThreadFunc* f,int64_t data);