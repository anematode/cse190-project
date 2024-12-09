#feature on safety
#include "../vendor/std2.h"

using namespace std2;

void entry_point_(const string^ s, int tid) safe {
    //s^->append("More text");
    println(*s)
}

    string s {"Hello threads"};

int main() safe {
    vector<thread> threads { };
    

    int num = 6;

    // launch all the threads
    const int num_threads = 15;
    for (int i : num_threads)
        threads^.push_back(thread(&entry_point_, ^const s, i));

    // mutable must borrow

    // join all the threads
    for(thread^ t: ^threads)
        t^->join();
}