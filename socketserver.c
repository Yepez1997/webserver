/* Socket Server */

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
 * */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <netdb.h>

#define HOSTLEN 256
#define BACKLOG 1

int make_server_socket_q(int port_number, int back_log) {
    /* Required Components of Socket */
    struct sockaddr_in socketaddr;
    struct hostent *hp;
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

    hp = gethostbyname(hostname);

    /* copy the bytes */
    /* copy host name to socket adress */
    bcopy((void*)hp->h_addr,(void*)&socketaddr.sin_addr, hp->h_length);

    socketaddr.sin_port = htons(port_number); /* SOCKET PORT */
    socketaddr.sin_family = AF_INET; /* ADDR FAMILY */

    if (bind(sock_id, (struct sockaddr *)&socketaddr, sizeof(socketaddr) != 0)) {
        return -1;
    }

    /* incomming calls */
    if (listen(sock_id, BACKLOG) != 0) {
        return -1;
    }

    return sock_id;
}

int connect_to_server(char *host, int port_number) {
    int sock;
    struct sockaddr_in servadd;
    struct hostent *hp;

    /* get a socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        return -1;
    }

    /* connect to the server */
    bzero(&servadd, sizeof(servadd)); // zero the addr
    hp = gethostbyname(host);

    if (hp == NULL) {
        return -1;
    }
    
    // source, dest, size
    bcopy(hp->h_addr,(struct sockaddr *)&servadd.sin_addr, hp->h_length);
    servadd.sin_port = htons(port_number);
    servadd.sin_family = AF_INET;

    if ( connect(sock, (struct sockaddr *)&servadd, sizeof(servadd) != 0)) {
        return -1;
    }
    return sock;

}

int make_server_socket(int port_number) {
    return make_server_socket_q(port_number, BACKLOG);
}

