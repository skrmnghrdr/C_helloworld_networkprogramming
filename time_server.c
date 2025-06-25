/*time_server.c*/

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif


#if defined(_WIN32)
#define ISVALIDSOCKET(s)    ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKERRNO() (WSAGetLastError())

#else 
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKERRNO() (errno)
#endif
// std c lihbraries to do our bidding
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(){

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)){
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif
    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 
    //AI_PASSIVE is the wildcard address 
    //0.0.0.0 in a sense

    //was curious on waht those codes would mean since they're int looking at the structs.
    printf("Codes are the following:\nai_fam:%d\n"
        "ai_socktype:%d\nai_flags:%d", AF_INET, SOCK_STREAM, AI_PASSIVE);

    /*
    book says some programmers dont use getaddrinfo() 
    they fill a struct addrinfo directly (ouch)
    this is pretty convenient I would suppose
    */
    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating socket...\n");
    SOCKET socket_listen;
    //we first make a socket_listen variable with the struct SOCKET
    //next we create a socket using the socket function from the header we imported by supplying the parameters
    //"the reason we used getaddrinfo before calling socket is that we can now pass inparts of bind_address as the arguments to socket() "
    //I'm thinking it formats it nicely, and we can just disect the bind_address parts to supply params on socket()
    socket_listen = socket(bind_address->ai_family,
    bind_address->ai_socktype, bind_address->ai_protocol);
    //1.bsically create a socket type first above
    //error handling over here
    if (!ISVALIDSOCKET(socket_listen)){
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKERRNO());
        return 1;
    }
    printf("Binding socket to local addresses...\n");
    if (bind(socket_listen,
        bind_address->ai_addr, bind_address->ai_addrlen)){
            fprintf(stderr, "bind() failed. (%d)\n", GETSOCKERRNO());
            return 1;
        }
        freeaddrinfo(bind_address);
    //return 0;
    //why was there a return 0 here, this is probably why the program stops

    //continue on page 58
    //make the socket listen here
    printf("Listening..\n");
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed! (%d)\n", GETSOCKERRNO());
        //error handling for listen is the same for bind()
        //and socket() as well too!
        return 1;
    }


    //start accepting 
    printf("Waiting for connection...\n");
    //then start accepting
    struct sockaddr_storage client_address; 
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr*) &client_address, &client_len);

    if (!ISVALIDSOCKET(socket_client)){
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKERRNO());
        return 1;
    }
    
    //yeaaaah finally we can replyyyyy
    printf("Client is connected!!!");\
    char address_buffer[100];
    getnameinfo((struct sockaddr*)&client_address,
    client_len, address_buffer, sizeof(address_buffer), 0,0,NI_NUMERICHOST);
    printf("%s\n", address_buffer);

    printf("Reading request...\n");
    char request[1024];
    int bytes_recieved = recv(socket_client, request, 1024, 0);
    printf("Received %d bytes. \n", bytes_recieved);

    //just like in python, always check the recv len,
    //well we could try except in python, but in C,
    //we need to check if recv is more than 0
    //which means it ran clean, no errors

    //printinf the browsers request to the console
    printf("%.*s", bytes_recieved, request);

    printf("Sending response...\n");
    const char *response = 
    "HTTP/1.1 200 OK\r\n"
    "Connection: close\r\n"
    "Content-Type: text/plain\r\n\r\n"
    "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    time_t timer;
    time(&timer);
    char *timer_msg = ctime(&timer);
    bytes_sent = send(socket_client, timer_msg, strlen(timer_msg), 0);
    printf("Sent %d of %d bytes. \n", bytes_sent, (int)strlen(timer_msg));

    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);


    printf("Closing listening socket...\n");

    #if defined(_WIN32)
        WSACleanup();
    #endif

        printf("Finished.\n");
        return 0;
    //HEAD BACK TO 59 AND REVIEW THIS ONCE AGAINI

    
    /*
    1. what is a socket;
        just a basic point of computer communication
        built with an ip and a port
    2. contents of a udp protocol
        datagram, and where to reply back for the sender
    3. tcp would have a handshake anmd like syna ck as well
    5. udp for gaming and video, something real time
        or speed as well
    6. tcp: reliable, something sensitive whre Ineed all data to be downloaded'
    7. noo, but it can guarantee if data was recieved
    9. bind. binding a service to a port, then from there on, the server would have to open a higher ephemeral port to it
    10: accept
     
    */
}
