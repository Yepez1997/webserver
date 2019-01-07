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

 /* The Client wants to read what the server sends
 * 1. Create a buffer to read the bytes and
 * 2. Write the bytes to local machine
 * */

 #include "serverclient.h"

 // communicate with server -> get info back
 void talk_with_server(int fd){
     char buf[1000];
     int n;
     n=read(fd,buf,LEN);
     write(1,buf,n);
 }

// connect client to server
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



 int main() {
    int fd;
    int portnumber = 8888;
     // this will error -> review hostname
    fd =  connect_to_server('TestHost',portnumber);
    if (fd == -1)  {
        exit(1);
    }
    talk_with_server(fd);
    close(fd);


}
