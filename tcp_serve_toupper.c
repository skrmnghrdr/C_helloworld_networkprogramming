#include "chap03.h"
#include <ctype.h>

int main(){
    //must put the windows wsadata here but we'll focus on linux for now
    //get local address, create socket and bind 3 easy steps

    printf("Configuring local address..\n");
    //we first get a hint of what ip we would like to be extracted s
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating socket...\n");
    SOCKET socket_listen;

    //we make a listening socket via the getaddrinfo
    //so instead of cinfiguring it manually, we use the library to nicely format our things to make it friendly when jumping on different OS
    //socket (family, socket_type, protocol)
    socket_listen = socket(bind_address->ai_family,
        bind_address->ai_socktype, bind_address->ai_protocol);

    /* in a nutshell,
    we configured the local address, by first making hints, and getting the addr info of it.
    putting the result on bindaddress

    theh we configure the listening socket with the address that we have to bind
    */

    if (!ISVALIDSOCKET(socket_listen)){
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    /*typical error check if the socket was valid or not*/

    printf("Binding socket to local address...\n");
    //bind is different from listening, as what you have learned
    /*
    we manually have to bind it first, (telling the OS like hey, this is my socket now)
    */
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)){
        //incase bind throws an error, you need to handle it
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
    }
    freeaddrinfo(bind_address);

    //basically like a house and lot, we first get the lot
    //then we build the house // or our satelite to listen
    printf("Listenting...\n");
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    //no we have the house and lot of the listener.



    // we should store our sockets somewhere in memory or they get loset
    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master)                          ;
    SOCKET max_socket = socket_listen;

    printf("Waiting for connections...\n");
    
    while(1) {
        //entering main loop
        fd_set reads;
        //do not forget the cruicial step of copying reads to master since select would modify it, (only leaving the sockets that are ready)
        //so if you don't copy it, you lose all of your legit sockets
        reads = master;
        if (select(max_socket+1, &reads, 0,0,0) < 0){
            
            fprintf(stderr, "Select() failed. (%d) \n", GETSOCKETERRNO());
            return 1;
        }
    
        SOCKET i;
        for(i = 1; i <= max_socket; i++){
            if (FD_ISSET(i, &reads)){
                if (i == socket_listen){
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    SOCKET socket_client = accept(socket_listen,
                    (struct sockaddr *) &client_address, &client_len);

                    if (!ISVALIDSOCKET(socket_client)){
                        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
                        return 1;
                    }

                    FD_SET(socket_client, &master);
                    if (socket_client > max_socket){
                    max_socket = socket_client;
                    }

                    char address_buffer[100];
                    getnameinfo((struct sockaddr*) &client_address, client_len, address_buffer, sizeof(address_buffer), 0,0, NI_NUMERICHOST);
                    printf("new connection from %s\n", address_buffer);
                }else {
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);
                    if(bytes_received < 1){
                        FD_CLR(i, &master);
                        CLOSESOCKET(i);  
                        continue;
                    }
                    
                    int j;
                    for (j = 0; j < bytes_received; ++j){
                        read[j] = toupper(read[j]);
                    }
                    send(i, read, bytes_received, 0);
                }


            }//fd_isset
        }//for i to max_socket
    }//big mother while true loop[]

    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);

    printf("Fin");
    return 0;
}

//next project is to create a chatroom as what is being said in the book
//next iteration of that project is to connect more people in the chatroom as a server and then each parties would be able to read both of the inputs of each other

//continue at 116