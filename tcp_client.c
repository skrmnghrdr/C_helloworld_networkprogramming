/*tcp_client*/

#include "chap03.h"
//just programming for linux as of now

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: tcp_client hostname port\n");
        return 1;
    }
    //basic argc contraining the nunmber of arguments values that will be available to us. like in python as well
    //the first argument would always be the programs name

    //argc is the argument count and argv is the arguemnt value

    printf("Configuring remote address...\n");
    //first thing's first, we put a varibale on the stack to ahve memory allocated to the struct data type
    struct addrinfo hints;
    //we need to zero it out for convention since it might contian some garbage 
    memset(&hints, 0, sizeof(hints));
    //next up we set the socket type of the hint to be stream (tcp)
    hints.ai_socktype = SOCK_STREAM;

    //then we proceed to make a peer adress on the stack
    struct addrinfo *peer_address;
    
    //we then try to format whatever address we got into a format that we can later on use
    //though this might sound irrelevant, formatting is a good way to write secure code and 
    //a more portable software. getaddrinfo has the format
    //getaddrinfo(char nodename, char servername, hitns, result);

    //plugging it in the function and checking to see the if it was valid,
    //typical C style approach
    if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)){
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    /* tcp client on page 82 by the book*/

    printf("Remote address is: ");
    //populate the buffer on the stack
    char address_buffer[100];
    char service_buffer[100];
    //make a buffer to store the name information
    //getnameinfo is formatted like so.
    //getnameinfo(socket_address, socketlength, host, hostlength, address_buffer, service_buffer, flags);

    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, 
        address_buffer, sizeof(address_buffer),
        service_buffer, sizeof(service_buffer),
        NI_NUMERICHOST);
    //remember that all strings are just arrays so you need the size since C does not konw when to sop

    printf("Creating socket...\n");
    //put socket variable on the stack
    SOCKET socket_peer;
    //socket() returns a SOCKET datatype, hence we declared the SOCKET up top to store it.
    socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);

    //no exceptions in this language so we have to check the socket if it was valid manually like so
    if (!ISVALIDSOCKET(socket_peer)) {
        fprintf(stderr, "socket() failed. (%d) \n", GETSOCKETERRNO());
        return 1;
    }

    printf("Connecting ...\n");
    //if the connect() did not return 0, (error) then we print it out
    if (connect(socket_peer,
        peer_address->ai_addr, peer_address->ai_addrlen)) {
            fprintf(stderr, "Connect failed, (%d)\n", GETSOCKETERRNO());
            return 1;
        }
        freeaddrinfo(peer_address);
    
    printf("Connected!\n");
    printf("To send data, enter text followed by enter.\n");
    
    //convenient way of printing the filedescriptor of such
    printf("File descriptor for stdin is:%d\n", fileno(stdin) );
    printf("For stdout is stdout:%d\n", fileno(stdout));

    while(1) {
        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(socket_peer, &reads);
        
        //linux only    
        FD_SET(0, &reads);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

        if (select(socket_peer+1, &reads, 0,0, &timeout) < 0){
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        //reason why we cannot jsut directly check with select is windows, so we have to put a timeout and check the input manually
        }

        if (FD_ISSET(socket_peer, &reads)) {
            //if the socket peer was ready, 
            char read[4096];
            int bytes_received = recv(socket_peer, read, 4096, 0);
            if (bytes_received < 1) {
                printf("Connection closed by peer.\n");
                break;
            }
            printf("Received (%d bytes): %.*s",bytes_received, bytes_received, read);
            //just we have to put the *s so we only print 4096 chars and not some garbage ones, also this would lead to some vulnerability liekt the canary overflow if you exploit it hard enough
        }

        if (FD_ISSET(0, &reads)) {
            //modifty this chunk to loop over the stdin if you don't see a newline or like a null character, so that we may send if the character ismore thatn 1024
        char read[1024];

        //actually, after dissecting the code, fgets leaves the things on where it was reading from
        if (!fgets(read, 1024, stdin)) break;
        printf("Sending: %s", read);
        int bytes_sent = send(socket_peer, read, strlen(read), 0);
        printf("Sent %d bytes.\n", bytes_sent);
            }   
        } 

        printf("Closing socket...\n");
        CLOSESOCKET(socket_peer);

        printf("Fin :)))) \n");
        return 0;

        //learned something new today,
        /*
        so if you wanna manually do an http request, you ned to type
        GET / HTTP/1.1
        Host: www.google.com
        (then send a blank line)
        **see results :)))))))
        */

        // continue on page 103 for the book on TCP server
}