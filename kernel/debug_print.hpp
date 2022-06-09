#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdio>

#include "console.hpp"

__attribute__((format(printf, 1, 2))) int printk(const char* format, ...) ;
