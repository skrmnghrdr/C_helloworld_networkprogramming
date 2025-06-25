/*udp_serve_toupper*/

#include "chap04.h"
#include <ctype.h>

int main() {
        printf("Configuring local address...\n");
        struct addrinfo hints;
        memset(hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;

        struct addrinfo *bind_address;
        getaddrinfo(0, "8080", &hints, &bind_address);
        print("Creating socket...\n");
        SOCKET socket_listen;
        socket_listen = socket(bind_address->ai_family,
                            bind_address->ai_socktype,
                            bind_address->ai_protocol);
        if (!ISVALIDSOCKET(socket_listen)){
            fprintf(stderr, "Socket() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }

        printf("Binding socket to local address...\n");
        if (bind(socket_listen,
                bind_address->ai_addr,
                bind_address->ai_addrlen)){
                    fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
                    return 1;
                }
                freeaddrinfo(bind_address);

        fd_set master;
        FD_ZERO(&master);
        FD_SET(socket_listen, &master);
        SOCKET max_socket = socket_listen;

        printf("Waiting for connections...\n");
        
    }
