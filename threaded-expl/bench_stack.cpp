#include "stack_lib.h"
#include <chrono>
#include <iostream>
#include <thread>

void push_numbers(stacks::Stack *stack, int16_t offset, int16_t step) {
    for (int i = offset; i < stacks::N; i += step) {
        stacks::push(stack, i);
    }
}

void bench_single(uint16_t num_benches) {
    size_t avrg_duration = 0;
    for (uint16_t i = 0; i < num_benches; i++) {
        auto stack = stacks::new_stack();
        auto start = std::chrono::high_resolution_clock::now();
        push_numbers(&stack, 0, 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << (stacks::verify_stack(&stack) ? "Correct" : "Smth is wrong") << std::endl;

        avrg_duration += duration.count();
    }
    std::cout << "SINGLE THREAD Avrg duration: \t" << avrg_duration / num_benches << std::endl;
}

void bench_thread(uint16_t num_benches) {
    size_t avrg_duration = 0;
    for (uint16_t i = 0; i < num_benches; i++) {
        auto stack = stacks::new_stack();
        auto start = std::chrono::high_resolution_clock::now();

        std::thread th1(push_numbers, &stack, 0, 2);
        std::thread th2(push_numbers, &stack, 1, 2);
        th1.join();
        th2.join();

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << (stacks::verify_stack(&stack) ? "Correct" : "Smth is wrong") << std::endl;
        avrg_duration += duration.count();
    }
    std::cout << "DOUBLE THREAD Avrg duration: \t" << avrg_duration / num_benches << std::endl;
}

void push_mutex_numbers(stacks::MutexStack *stack, int16_t offset, int16_t step) {
    for (int i = offset; i < stacks::N; i += step) {
        stacks::mutex_stack_push(stack, i);
    }
}

void bench_mutex_single(uint16_t num_benches) {
    size_t avrg_duration = 0;
    for (uint16_t i = 0; i < num_benches; i++) {
        auto stack = stacks::mutex_stack_new();
        auto start = std::chrono::high_resolution_clock::now();
        push_mutex_numbers(stack, 0, 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << (stacks::verify_mutex_stack(stack) ? "Correct" : "Smth is wrong") << std::endl;
        avrg_duration += duration.count();

        mutex_stack_drop(stack);
    }
    std::cout << "SINGLE THREAD Avrg duration: \t" << avrg_duration / num_benches << std::endl;
}

void bench_mutex_thread(uint16_t num_benches) {
    size_t avrg_duration = 0;
    for (uint16_t i = 0; i < num_benches; i++) {
        auto stack = stacks::mutex_stack_new();
        auto start = std::chrono::high_resolution_clock::now();

        std::thread th1(push_mutex_numbers, stack, 0, 2);
        std::thread th2(push_mutex_numbers, stack, 1, 2);
        th1.join();
        th2.join();

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << (stacks::verify_mutex_stack(stack) ? "Correct" : "Smth is wrong") << std::endl;
        avrg_duration += duration.count();

        mutex_stack_drop(stack);
    }
    std::cout << "DOUBLE THREAD Avrg duration: \t" << avrg_duration / num_benches << std::endl;
}

void push_atomic_numbers(stacks::AtomicStack *stack, int16_t offset, int16_t step) {
    for (int i = offset; i < stacks::N; i += step) {
        stacks::atomic_stack_push(stack, i);
    }
}

void bench_atomic_single(uint16_t num_benches) {
    size_t avrg_duration = 0;
    for (uint16_t i = 0; i < num_benches; i++) {
        auto stack = stacks::atomic_stack_new();
        auto start = std::chrono::high_resolution_clock::now();
        push_atomic_numbers(stack, 0, 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << (stacks::verify_atomic_stack(stack) ? "Correct" : "Smth is wrong") << std::endl;

        avrg_duration += duration.count();
    }
    std::cout << "SINGLE THREAD Avrg duration: \t" << avrg_duration / num_benches << std::endl;
}

void bench_atomic_thread(uint16_t num_benches) {
    size_t avrg_duration = 0;
    for (uint16_t i = 0; i < num_benches; i++) {
        auto stack = stacks::atomic_stack_new();
        auto start = std::chrono::high_resolution_clock::now();

        std::thread th1(push_atomic_numbers, stack, 0, 2);
        std::thread th2(push_atomic_numbers, stack, 1, 2);
        th1.join();
        th2.join();

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << (stacks::verify_atomic_stack(stack) ? "Correct" : "Smth is wrong") << std::endl;
        avrg_duration += duration.count();
    }
    std::cout << "DOUBLE THREAD Avrg duration: \t" << avrg_duration / num_benches << std::endl;
}

int main() {
    uint16_t num_benches;
    std::cin >> num_benches;
    std::cout << "Simple" << std::endl;
    bench_single(num_benches);
    bench_thread(num_benches);
    std::cout << "Mutex" << std::endl;
    bench_mutex_single(num_benches);
    bench_mutex_thread(num_benches);
    std::cout << "Atomic" << std::endl;
    bench_atomic_single(num_benches);
    bench_atomic_thread(num_benches);

    return 0;
}