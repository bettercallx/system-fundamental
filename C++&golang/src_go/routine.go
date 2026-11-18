//go routine is not a thread, it is a lightweight thread managed by go runtime
// Goroutines run in the same address space, so access to shared memory must be synchronized.同步的
// goroutines from 1 program run inside that 1 process, communicate via channels
func task() {

}