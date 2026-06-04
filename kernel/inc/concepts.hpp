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
#include <stddef.h>
#include <stdint.h>
#include <type_traits.h>

namespace Kernel::Concepts {

template <typename T>
concept Allocator = requires(size_t size, void* ptr) {
  { T::allocate(size) } -> std::same_as<void*>;
  { T::deallocate(ptr, size) } -> std::same_as<void>;
};

template <typename T>
concept SystemTimer = requires() {
  { T::get_ticks() } -> std::same_as<uint32_t>;
};

}  // namespace Kernel::Concepts
