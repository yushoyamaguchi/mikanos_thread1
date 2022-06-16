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


void thread_create(TaskFunc* f);