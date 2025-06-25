/*thousandbind.c*/

//an experimental program to just listen on 1000 ports for no reason at all because she said I was not a good listener.


#include "chap04.h"
#include "stdbool.h"



void local_address(struct addrinfo *hint, int){
    printf("Zeroing\n");
    memset(hint, 0, sizeof(struct addrinfo));
    hint->ai_family = AF_INET;
    hint->ai_socktype = SOCK_DGRAM;
    hint->ai_flags = AI_PASSIVE;
    
    struct addrinfo *bind_address;
    getaddrinfo(0, "")
}

bool is_struct_empty(struct addrinfo *ptr){
    struct addrinfo zeroed;
    memset(&zeroed, 0, sizeof(zeroed));
    return memcmp(ptr, &zeroed, sizeof(*ptr)) == 0;
}
int main() {
    //how this would go is we would probably make a loop, or 
    //another function that would probably open a socket?
    //then just run a for loop over it like 1000 times,
    //so we would have a function
    //that takes in 
    //typical configure the local

    struct addrinfo dirty;
    printf("Is empty? %s\n", is_struct_empty(&dirty) ? "true" : "false" );
    local_address(&dirty);
    printf("Is empty? %s\n", is_struct_empty(&dirty) ? "true" : "false");

    struct addrinfo hints;
    local_address(&hints);
    //now we got the sanity check zeroed out, we can proceed to allocate it with the hints

    

    return 0;
}

