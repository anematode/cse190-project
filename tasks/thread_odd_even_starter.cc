// Task: Implement the print_even and print_odd functions to correctly print even and odd numbers using multithreading.
// Each function should increment a shared integer and print it when it's their turn (even or odd).
// The functions should stop when the shared integer exceeds a certain limit, e.g., 10.

#include "../vendor/std2.h"
#include "../vendor/io.h"
#include <chrono>
#include <unistd.h>

using namespace std2;

/**
 * Function: print_even
 * Description: This function should print even numbers starting from 0 up to a maximum value.
 * It must ensure that access to the shared integer is thread-safe.
 * 
 * Steps:
 * 1. Continuously loop until the shared number exceeds 10.
 * 2. Lock the shared resource to ensure exclusive access.
 * 3. Check if the current number is even:
 *      a. If yes, print the number.
 *      b. Increment the number.
 * 4. Release the lock.
 * 5. Pause the thread briefly to simulate processing time (e.g., sleep for 10 milliseconds).
 */
void print_even(arc<mutex<int>> current_number) safe {
    // Implement this function following the steps above.
}

/**
 * Function: print_odd
 * Description: This function should print odd numbers starting from 1 up to a maximum value.
 * It needs to synchronize access to a shared integer to prevent data races.
 * 
 * Steps:
 * 1. Continuously loop until the shared number exceeds 10.
 * 2. Acquire a lock on the shared resource for safe access.
 * 3. Check if the current number is odd:
 *      a. If yes, print the number.
 *      b. Increment the number.
 * 4. Release the lock.
 * 5. Introduce a brief pause to lower CPU usage (e.g., sleep for 10 milliseconds).
 */
void print_odd(arc<mutex<int>> current_number) safe {
    // Implement this function following the steps above.
}

/**
 * Main function to set up and execute the threads.
 * This function sets up the shared number and launches two threads: one for even numbers and one for odd numbers.
 * It waits for both threads to complete before exiting.
 */
int main() safe {
    arc<mutex<int>> shared_number(int(0)); // Initialize shared number protected by a mutex.

    // Create threads for printing even and odd numbers.
    thread even_thread(print_even, cpy shared_number);
    thread odd_thread(print_odd, cpy shared_number);

    // Wait for both threads to complete their execution.
    even_thread rel.join();
    odd_thread rel.join();
}
