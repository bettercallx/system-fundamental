#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
//编译 -O2 opitimized, -O0 unoptimized
long counter = 0;        // shared memory: threads communicate through this same variable
mutex m;

// WITHOUT lock: both threads do counter++ at the same time -> race
void add_unsafe(int times) {
    for (int i = 0; i < times; i++) {
        counter++;       // read, +1, write  (three steps, can interleave!)
    }
}

// WITH lock: only one thread touches counter at a time
void add_safe(int times) {
    for (int i = 0; i < times; i++) {
        lock_guard<mutex> lock(m);
        counter++;
    }
}
//lixiang@lixiangdeMacBook-Pro pixelcanvas % ./studyC++\&golang/output/bin/shared_debug 
//unsafe result: 1000138  (expected 2000000)
//safe result:   2000000  (expected 2000000)

//Thread A: read counter (=5)
//Thread B: read counter (=5)      ← both read 5
//Thread A: write 6
//Thread B: write 6                ← B overwrites A! Two ++ but counter only went up by 1
int main() {
    const int N = 1000000;

    // --- unsafe ---
    counter = 0;
    { thread a(add_unsafe, N), b(add_unsafe, N); a.join(); b.join(); }
    cout << "unsafe result: " << counter << "  (expected " << 2 * N << ")\n";

    // --- safe ---
    counter = 0;
    { thread a(add_safe, N), b(add_safe, N); a.join(); b.join(); }
    cout << "safe result:   " << counter << "  (expected " << 2 * N << ")\n";
    return 0;
}
