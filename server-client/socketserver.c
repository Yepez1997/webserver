/* Socket-Client Server */

/*  Basic Client Server Model
 *  ----------           ----------
 * | Client   |         | Server   |
 *  ----------           ----------
 *      ||                   ||
 *  ____________________________________
 *
 *   Client:                            Server:
 *                                      set up the service  <--------+
 *   connect to Server ---------------> accept server call          ||
 *   obtain service   <---------------> provide service             ||
 *   hang up                            hang up ---------------------+
 *
 *   Main Ops:
 *      1. Server sets up a service
 *      2. Client connects to Server
 *      3. Server and Client communicate
 * */

#include "serverclient.h"

#define HOSTLEN 256
#define BACKLOG 1

int make_server_socket_q(int port_number, int back_log) {
    /* Required Components of Socket */
    struct sockaddr_in socketaddr;
    // struct address we want to get
    struct hostent *hp; // refer to hostent struct in netbd.h
    char hostname[HOSTLEN];
    int sock_id;

    /* Create the socket */
    sock_id = socket(PF_INET, SOCK_STREAM, 0);
    if (sock_id == -1) {
        return -1;
    }

    /* build adress and bind to socket */
    bzero((void*) &socketaddr, sizeof(socketaddr));

    /* gets the host name*/
    gethostname(hostname, HOSTLEN);


    /* copy the bytes */
    /* copy host name to socket adress */
    bcopy((void*)hp->h_addr,(void*)&socketaddr.sin_addr, hp->h_length);

    socketaddr.sin_port = htons(port_number); /* SOCKET PORT */
    socketaddr.sin_family = AF_INET; /* ADDR FAMILY */

    // bind function goes as follows:  bind(int, struct sockaddr *, socklen)
    if (bind(sock_id, (struct sockaddr *)&socketaddr, sizeof(socketaddr) != 0)) {
        return -1;
    }

    /* incomming calls */
    if (listen(sock_id, BACKLOG) != 0) {
        return -1;
    }

    return sock_id;
}

int make_server_socket(int port_number) {
    return make_server_socket_q(port_number, BACKLOG);
}

/*Process Request: return the date to client */
void process_request(int fd) {
    int pid = fork(); // process id
    switch(pid){
        case -1: return;
        case 0: dup2(fd,1); // works and child runs
                close(fd);
                execl("/bin/date","date", NULL); // exec the date command
                // oops("execlp");
        default: wait(NULL);
    }
}

/* must use sigchild to prevent zombies
* i.e server should not wait for child to finish handeling the request
* sigchild when process dies or gets killed -> sends process to parent
* */
void child_waiter(int signum){
    // if we encounter multiple process exiting at once
    // some can get lost
   while(waitpid(-1,NULL, WNOHANG) > 0);
}


/* This is only for server side */
int main() {
    int sock, fd; // socket and connection
    int port_number = 8888;
    void process_request(int);
    void child_waiter(int);
    signal(SIGCHLD, child_waiter);
    // make server socket
    if(sock= make_server_socket(port_number) == -1) {
        exit(1);
    }
    while(1) {
        fd = accept(sock,NULL, NULL);
        if (fd == -1) {
            break;
        }
        process_request(fd);
        close(fd);
    }
}



// C++, Java, Python, C, SQL, Go, JavaScript, Swift, Ruby