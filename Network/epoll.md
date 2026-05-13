https://man7.org/linux/man-pages/man7/epoll.7.html  


cache I/O: 

I/O multiplexing:


POSIX(portable OS interface) I/O multiplexing: `select & polling` all client fd. 



Linux I/O event notification: `epoll`, RB tree storage `fd`, add/delete with O(log n). 
- epoll instance 
can be considered as a container for 2 lists:
interest list - the set of fd has registered in monitoring

ready list - the set of fd are ready for I/O, subset of interest list

`epoll_interest` create a new epoll instance and return a fd referring to the instance

`epoll_ctl` register fd in interest list

`epoll_wait` waits for I/O events, blocking the calling thread if no events are currently available 

- Edge Triggered/ Level Triggered






- protocol / IPC(pipe| ,shared memory)
ls | grep "file1"
grep: global regular expression print

ps aux | grep -i "Manager"
ps aux: list all the process; grep filter for processes with "manager" in the name; -i: ignore case
ps aux | grep xxxx(pid) : list process with pid
ps -ef | grep Manager


lsof -i :8080
check whether the 8080 socket is open

curl (Client URL) multiple protocols:HTTP, HTTPS, FTP...
support GET, POST

DNS find IP -> construct HTTP package, TLS -> verify encryption version and algo, server send certificate chain
, verify signature/domain/expire date, key exchange, encrypt and transmit data