#ifndef mozilla_wr_bindings_h
#define mozilla_wr_bindings_h

#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

namespace queues {

constexpr static const uintptr_t N = 1000;

constexpr static const uintptr_t N2 = (N + 100);

struct Stack {
  uintptr_t head;
  int32_t data[N2];
};

extern "C" {

Stack new_stack();

int32_t peak(const Stack *self);

void push(Stack *self, int32_t el);

int32_t pop(Stack *self);

bool verify(const Stack *self);

} // extern "C"

} // namespace queues

#endif // mozilla_wr_bindings_h
