//go routine is not a thread, it is a lightweight thread managed by go runtime
// Goroutines run in the same address space, so access to shared memory must be synchronized.同步的
┌───────────────────┬───────────────────────┬──────────────────────────────┐
│                   │    C++ std::thread    │         Go goroutine         │
├───────────────────┼───────────────────────┼──────────────────────────────┤
│ Who manages it    │ OS kernel             │ Go runtime (in your program) │
├───────────────────┼───────────────────────┼──────────────────────────────┤
│ Starting stack    │ ~1–8 MB fixed         │ ~2 KB, grows as needed       │
├───────────────────┼───────────────────────┼──────────────────────────────┤
│ How many feasible │ thousands             │ hundreds of thousands+       │
├───────────────────┼───────────────────────┼──────────────────────────────┤
│ Cost to create    │ syscall (slow)        │ function call (cheap)        │
├───────────────────┼───────────────────────┼──────────────────────────────┤
│ Switching         │ kernel context switch │ user-space, cheaper          │
├───────────────────┼───────────────────────┼──────────────────────────────┤
│ Start syntax      │ std::thread t(f)      │ go f()                       │
├───────────────────┼───────────────────────┼──────────────────────────────┤
