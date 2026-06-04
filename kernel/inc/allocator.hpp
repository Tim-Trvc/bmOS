// Copyright 2026 Tim Travica
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

#pragma once

#include "concepts.hpp"

namespace Kernel {

template <uint8_t allignment, size_t pool_size>
struct ArenaAllocator {
  static void* allocate(std::size_t size) {
    alignas(allignment) static uint8_t pool[pool_size];
    static size_t offset = 0;

    offset = (offset + (allignment - 1)) & ~(allignment - 1);

    void* ptr = &pool[offset];
    offset = offset + size;

    if (offset > size) {
      return nullptr;
    }

    return ptr;
  }

  static void deallocate(void* ptr, std::size_t size) {
    // currently ArenaAllocator does not free memory
  }
};

}  // namespace Kernel
