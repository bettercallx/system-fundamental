#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//epfd: instance of epoll
//listen_fd: listen socket
//client_fd: connected socket
//socket+bind+listen -> epoll_create1 -> epoll_ctl -> epoll_wait
// epoll_create1 (set control room) int epfd = epoll_create1(0);
// epoll_ctl (add camera)
// epoll_wait (wait for notification) listen_fd(add new fd) / client_fd(read/recv data)

#define PORT 8080
#define MAX_EVENTS 64
#define BUF_SIZE 1024

// TODO 1: 实现 set_nonblocking
// set fd as non-blocking
void set_nonblocking(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

// TODO 2: 实现 add_to_epoll
// add fd epoll to epfd，ET mode
// 读事件EPOLL IN + 边缘触发 EPOLL ET
void add_to_epoll(int epfd, int fd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

// TODO 3: 实现 handle_new_connection
// accept新连接，设为非阻塞，加入epoll
// 提示：accept返回-1且errno==EAGAIN时停止（listen fd也是非阻塞ET）
/* The accept() system call extracts the first
 * connection request on the queue of pending connections for the
 * listening socket, sockfd, creates a new connected socket, and
 * returns a new file descriptor referring to that socket.
 */
void handle_new_connection(int epfd, int listen_fd) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    while(1){
        int new_fd = accept(listen_fd,(struct sockaddr*)&client_addr, &addr_len);//?? addr怎么传入？
        if (new_fd>0) {
            set_nonblocking(new_fd);
            add_to_epoll(epfd, new_fd);
        } else if (new_fd == -1 && errno==EAGAIN) {
            break;
        } else{
            return;
        }
    }
}

// TODO 4: 实现 handle_client_data
// ET模式下必须循环read直到EAGAIN
// echo回去，对端关闭时从epoll删除并close
// 提示：read返回0表示对端关闭，返回-1且errno==EAGAIN表示读完了
void handle_client_data(int epfd, int fd) {
    char buffer[1024];

    ssize_t read_data = read(fd, buffer,sizeof(buffer));
    if(read_data > 0){
        continue;
    } else if (read_data == -1 && errno==EAGAIN) {
        break;
    } else if (read_data == 0){
        printf("client socket closed");
    }
}

//双层while(1), 

int main() {
    // create listen fd
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };
    bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(listen_fd, 128);

    // TODO 5: listen_fd也要设为非阻塞，加入epoll
    int epfd = epoll_create1(0);
    set_nonblocking(listen_fd);
    add_to_epoll(epfd, listen_fd);

    struct epoll_event events[MAX_EVENTS];

    printf("server listening on port %d\n", PORT);

    while (1) {
        // TODO 6: epoll_wait，根据返回的事件fd分发到handle_new_connection 或 handle_client_data
        // 每次epoll_wait处理已ready的事件，处理完返回，服务器持续监听，循环回去再调epoll_wait等下一批事件
        int n = epoll_wait(epfd, events, MAX_EVENTS,-1);
        for (int i = 0; i <n;i++) {
            int fd = events[i].data.fd;
            uint32_t revent = events[i].events;
            if (fd == listen_fd){
                handle_new_connection(epfd, fd);
            } else if (revent & EPOLLIN) {
                handle_client_data(epfd, fd);
            }
        }
    }

    close(listen_fd);
    close(epfd);
    return 0;
}