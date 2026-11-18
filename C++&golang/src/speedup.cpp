#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

// Pretend this is slow work: a network call, a big computation, reading a file.
void slow_task(int id) {
    this_thread::sleep_for(chrono::seconds(1));
    cout << "task " << id << " done\n";
}

int main() {
    auto start = chrono::steady_clock::now();

    // --- Sequential: do 3 tasks one after another ---顺序完成
    slow_task(1);
    slow_task(2);
    slow_task(3);

    auto mid = chrono::steady_clock::now();

    // --- Concurrent: do 3 tasks at the same time --- 并发完成  
    // 适用Waiting on slow things (network, disk, sleep) — a thread that's blocked waiting lets other threads use the CPU
    // heacy computation
    thread a(slow_task, 4);
    thread b(slow_task, 5);
    thread c(slow_task, 6);
    a.join();
    b.join();
    c.join();

    auto end = chrono::steady_clock::now();

    auto seq  = chrono::duration_cast<chrono::milliseconds>(mid - start).count();
    auto conc = chrono::duration_cast<chrono::milliseconds>(end - mid).count();
    cout << "sequential took: " << seq  << " ms\n";
    cout << "concurrent took: " << conc << " ms\n";
    return 0;
}
