#include <iostream>
#include <thread>

int main() {
    // may return 0 when not able to detect
    const auto processor_count = std::thread::hardware_concurrency();
    std::cout << processor_count << std::endl;

    return 0;
}

