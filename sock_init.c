/* sock_init.c */

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock.h>
#include <ws2tcpip.h>

//kinda useless but we'll put it here anayways since this is part of the source code
#pragma comment(lib, "ws2_32.lib")

#else
//basically, the if above would compile if it's a windows computer
//as you can see over there
//if otherwise, probably a linux or mac one, then here comes the else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
//berkley socket API headers and other headers we need would need to be included
//also not that the book says berkeley sockets, no special init is used, they're always read (thank you linux <3)
#endif 

#include <stdio.h>

int main(){

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprint(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    printf("Ready to use socket API.\n");

#if defined(_WIN32)
    WSACleanup();
#endif
    return 0;

    //quick notes on here
    /*
    udp is best when you don;'t need a reply from the client, all you do is just send: video games, videos, streaming, music, ip multicast, broadcast 

    if you're thinking of something like a custom protocol, you could just put a udp if you don't want the other end to reply, though, you might need to number the udp packets and ask for the missing ones to mitigate data loss

    some of the socket functions here are almost the same as the socket library in python, 
    socket()
    bind()
    listen()
    connect()
    accept()
    send()
    //and obviously, 
    recv()
    sendto() and recvfrom() are used to send and  recieve data from sockets without a bound remote address
    close(), closesocket()
    shutdown() //only used to close one side of the connection, #graceful and orderly connection teardown
    select() //used to wait for an event or one ormore sockets
    getnanmeinfo() and getaddrinfo() to provide a protocol independent manner
    setsockopt() // socket options
    fcntl() ,and ioctlsocket() used to get some socket options
    
    on the udp client, take note that the client must send the data first, as you remembered from the UDP listener exercise, you needed to "poke the server" first after listening because the UDP server would have no way of knowing where to send since UDP did not do any handshake
    */
}
