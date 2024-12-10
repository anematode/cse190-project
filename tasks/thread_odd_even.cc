#feature on safety
#include "../vendor/std2.h"
#include "../vendor/io.h"
#include <chrono>
#include <unistd.h>

using namespace std2;

void print_even(arc<mutex<int>> current_number) safe {
  while (true) {
    auto lock_guard = current_number->lock();
    int^ num = mut lock_guard.borrow();

    // might need to unlock thread after?
    if (*num > 10) break; // Stop after reaching 100

    if (*num % 2 == 0) { // Check if the number is even
      println(*num);
      ++(*num); // Increment to the next number
    }

    unsafe { std::this_thread::sleep_for(std::chrono::milliseconds(10)); } // Add delay to reduce CPU usage
  }
}

void print_odd(arc<mutex<int>> current_number) safe {
  while (true) {
    auto lock_guard = current_number->lock();
    int^ num = mut lock_guard.borrow();

    // might need to unlock thread after?
    if (*num > 10) break; // Stop after reaching 100

    if (*num % 2 != 0) { // Check if the number is even
      println(*num);
      ++(*num); // Increment to the next number
    }

    unsafe { std::this_thread::sleep_for(std::chrono::milliseconds(10)); } // Add delay to reduce CPU usage
  }
}

int main() safe {
  // Shared counter protected by a mutex, start number at 0
  // arc<mutex<int>> shared_number(new mutex<int>(0));
  arc<mutex<int>> shared_number(int(0));

  // thread(entry_point, cpy shared_data, i)
  thread even_thread(print_even, cpy shared_number);
  thread odd_thread(print_odd, cpy shared_number);

  even_thread rel.join();
  odd_thread rel.join();
}