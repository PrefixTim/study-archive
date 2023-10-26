#include "stack_lib.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

uintptr_t global_limit;

void push_numbers(stacks::Stack *stack, int16_t offset, int16_t step) {
    for (int i = offset; i < global_limit; i += step) {
        stacks::push(stack, i);
    }
}

size_t bench_single(size_t num_benches, bool verbose) {
    size_t avrg_duration = 0;
    for (uintptr_t i = 0; i < num_benches; i++) {
        auto stack = stacks::new_stack();
        auto start = std::chrono::high_resolution_clock::now();
        push_numbers(&stack, 0, 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        if (verbose) {
            std::cout << (stacks::verify_stack(&stack, global_limit) ? "Correct" : "Smth is wrong") << std::endl;
        }
        avrg_duration += duration.count();
    }
    return avrg_duration / num_benches;
}

size_t bench_thread(size_t num_benches, bool verbose) {
    size_t avrg_duration = 0;
    for (uintptr_t i = 0; i < num_benches; i++) {
        auto stack = stacks::new_stack();
        auto start = std::chrono::high_resolution_clock::now();

        std::thread th1(push_numbers, &stack, 0, 2);
        std::thread th2(push_numbers, &stack, 1, 2);
        th1.join();
        th2.join();

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        if (verbose) {
            std::cout << (stacks::verify_stack(&stack, global_limit) ? "Correct" : "Smth is wrong") << std::endl;
        }
        avrg_duration += duration.count();
    }
    return avrg_duration / num_benches;
}

void push_mutex_numbers(stacks::MutexStack *stack, int16_t offset, int16_t step) {
    for (int i = offset; i < global_limit; i += step) {
        stacks::mutex_stack_push(stack, i);
    }
}

size_t bench_mutex_single(size_t num_benches, bool verbose) {
    size_t avrg_duration = 0;
    for (uintptr_t i = 0; i < num_benches; i++) {
        auto stack = stacks::mutex_stack_new();
        auto start = std::chrono::high_resolution_clock::now();
        push_mutex_numbers(stack, 0, 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        if (verbose) {
            std::cout << (stacks::verify_mutex_stack(stack, global_limit) ? "Correct" : "Smth is wrong") << std::endl;
        }
        avrg_duration += duration.count();

        mutex_stack_drop(stack);
    }
    return avrg_duration / num_benches;
}

size_t bench_mutex_thread(size_t num_benches, bool verbose) {
    size_t avrg_duration = 0;
    for (uintptr_t i = 0; i < num_benches; i++) {
        auto stack = stacks::mutex_stack_new();
        auto start = std::chrono::high_resolution_clock::now();

        std::thread th1(push_mutex_numbers, stack, 0, 2);
        std::thread th2(push_mutex_numbers, stack, 1, 2);
        th1.join();
        th2.join();

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        if (verbose) {
            std::cout << (stacks::verify_mutex_stack(stack, global_limit) ? "Correct" : "Smth is wrong") << std::endl;
        }
        avrg_duration += duration.count();

        mutex_stack_drop(stack);
    }
    return avrg_duration / num_benches;
}

void push_atomic_numbers(stacks::AtomicStack *stack, int16_t offset, int16_t step) {
    for (int i = offset; i < global_limit; i += step) {
        stacks::atomic_stack_push(stack, i);
    }
}

size_t bench_atomic_single(size_t num_benches, bool verbose) {
    size_t avrg_duration = 0;
    for (uintptr_t i = 0; i < num_benches; i++) {
        auto stack = stacks::atomic_stack_new();
        auto start = std::chrono::high_resolution_clock::now();
        push_atomic_numbers(stack, 0, 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        if (verbose) {
            std::cout << (stacks::verify_atomic_stack(stack, global_limit) ? "Correct" : "Smth is wrong") << std::endl;
        }
        avrg_duration += duration.count();
    }
    return avrg_duration / num_benches;
}

size_t bench_atomic_thread(size_t num_benches, bool verbose) {
    size_t avrg_duration = 0;
    for (uintptr_t i = 0; i < num_benches; i++) {
        auto stack = stacks::atomic_stack_new();
        auto start = std::chrono::high_resolution_clock::now();

        std::thread th1(push_atomic_numbers, stack, 0, 2);
        std::thread th2(push_atomic_numbers, stack, 1, 2);
        th1.join();
        th2.join();

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        if (verbose) {
            std::cout << (stacks::verify_atomic_stack(stack, global_limit) ? "Correct" : "Smth is wrong") << std::endl;
        }
        avrg_duration += duration.count();
    }
    return avrg_duration / num_benches;
}

int main(int argc, char *argv[]) {
    size_t num_benches;
    bool verbose = true;
    
    if (argc >= 3) {
        num_benches = (size_t)atoi(argv[1]);
        global_limit = (uintptr_t)atoi(argv[2]);
        verbose = false;
    } else {
        std::cin >> num_benches;
        std::cin >> global_limit;
    }

    global_limit = stacks::N < global_limit ? stacks::N : global_limit;

    auto single = bench_single(num_benches, verbose);
    auto thread = bench_thread(num_benches, verbose);
    auto mutex_single = bench_mutex_single(num_benches, verbose);
    auto mutex_thread = bench_mutex_thread(num_benches, verbose);
    auto atomic_single = bench_atomic_single(num_benches, verbose);
    auto atomic_thread = bench_atomic_thread(num_benches, verbose);

    if (verbose) {
        std::cout << "Simple" << std::endl;
        std::cout << "SINGLE THREAD Avrg duration: \t" << single << std::endl;
        std::cout << "DOUBLE THREAD Avrg duration: \t" << thread << std::endl;
        std::cout << "Mutex" << std::endl;
        std::cout << "SINGLE THREAD Avrg duration: \t" << mutex_single << std::endl;
        std::cout << "DOUBLE THREAD Avrg duration: \t" << mutex_thread << std::endl;
        std::cout << "Atomic" << std::endl;
        std::cout << "SINGLE THREAD Avrg duration: \t" << atomic_single << std::endl;
        std::cout << "DOUBLE THREAD Avrg duration: \t" << atomic_thread << std::endl;
    } else {
        std::cout << single << " " << thread << " " << mutex_single << " " << mutex_thread << " " << atomic_single << " " << atomic_thread << std::endl;
    }
    return 0;
}