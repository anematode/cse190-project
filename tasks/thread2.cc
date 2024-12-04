#feature on safety
#include "../vendor/std2.h"
#include <chrono>

using namespace std2;

void entry_point_(shared_ptr<string> data, int tid) safe {
    //s^->append("More text");
    println(*data);
}

int main() safe {
    vector<thread> threads { };
    
    auto data = shared_ptr<string>::make("Hello threads");

    // launch all the threads
    const int num_threads = 15;
    for (int i : num_threads)
        threads^.push_back(thread(&entry_point_, copy data, i));

    // mutable must borrow

    // join all the threads
    for(thread^ t: ^threads)
        t^->join();
}