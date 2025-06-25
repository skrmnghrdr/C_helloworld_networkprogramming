#include "chap03.h"

int main(int argc, char *argv[]){
    printf("First hello world in network programming trying to write something on my own\n");

    struct addrinfo hints;
    struct addrinfo *result;
    //we forgot to clean out the hints
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int getaddr_result = getaddrinfo(argv[1], argv[2], &hints, &result);
    switch (getaddr_result)
    {
    case -8:
        fprintf(stderr, "Something went wrong with getaddrinfo (%d) (%d)", GETSOCKETERRNO(), getaddr_result);
        printf("Name or service not known");
        return -1;
    default:{}
    } 
        
    //we need to have get name info to print out since the getaddrinfo, was just to translate or format the ip or the hostname to a nice uniform way so we could do something with teh information

    //hostname /ip and port buffer
    char address_buffer[100];
    char service_buffer[100];

    if (getnameinfo(result->ai_addr, result->ai_addrlen,
                address_buffer, sizeof(address_buffer),
                service_buffer, sizeof(service_buffer), NI_NUMERICHOST)){
        
        printf("Something went wrong with getnameinfo()");
        return -1;
                }
    printf("The address is %s\n The port/service is %s", address_buffer, service_buffer);

    return 0;
}
//continue to page 102 to continue thy quest :))0
