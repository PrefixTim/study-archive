#include "../queue_lib/queue_lib.h"
#include <chrono>
#include <iostream>

size_t bench();
void push_numbers(queues::Stack *);


int main() {
    uint16_t num_benches;
    std::cin >> num_benches;
    std::cout << bench() << "cold" << std::endl;
    size_t avrg_duration = 0;
    for(uint16_t i = 0; i < num_benches; i++) {
        avrg_duration += bench();
    }
    std::cout << "Avrg duration: \t" << avrg_duration / num_benches << std::endl;
    return 0;
}

size_t bench() {
    auto stack = queues::new_stack();
    auto start = std::chrono::high_resolution_clock::now();
    push_numbers(&stack);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
    std::cout << (queues::verify(&stack) ? "Correct" : "Smth is wrong") << std::endl;
    
    return duration.count();
}

void push_numbers(queues::Stack *stack) {
    for (int i = 0; i < queues::N; ++i) {
        queues::push(stack, i);
    }
}