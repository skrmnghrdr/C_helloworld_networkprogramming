/* unix_list.c*/

#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>


/* unix_list c begin */

int main() {
    
    struct ifaddrs *addresses;

    //man 3 get
    if (getifaddrs(&addresses) == -1 ) {
        printf("getifaddrs call failed!\n");
        return -1;
    }

    // we need to call getifaddrs so we can populate the memory and fill linked list of addresses
    /*cont*/

    struct ifaddrs *address = addresses;
    while(address) {
        int family = address->ifa_addr->sa_family;
        //maybe someday, you'll comeback here and actually teach someone 
        //how to print a subnet mask from this, until 
        //then, keep learning, -Yourself from 6/12/25
        if(family == AF_INET || family == AF_INET6){
            printf("%s\t", address->ifa_name);
            printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");

            char ap[100];

            //condition ? expression_if_true : expression if false
            const int family_size = family == AF_INET ?
                sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            getnameinfo(address->ifa_addr,
                family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);
        }
        address = address->ifa_next;
    }
    freeifaddrs(addresses);
    return 0;
}