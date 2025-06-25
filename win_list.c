/* win_list.c */

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x600
#endif

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

//probably not for this compiler since we are using 
//gcc in windows 
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

//we will need to link the libraries with
/* gcc win_list.c -o win_list.exe -liphlpapi -lws2_32 */

int main() {

    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)){
        printf("Failed to initialize. \n");
        return -1;
    }

    DWORD asize = 20;
    //changin asize to a really small number would tell you how much exact bytes we need
    PIP_ADAPTER_ADDRESSES adapters;
    do {
        //memory allocation for adapters
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);

        if (!adapters) {
            //error checking for malloc
            printf("Couldn't allocate %ld bytes for adapters.\n", asize);
            WSACleanup();
            return -1;
        }
        //using built in function to get addresses
        int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);
        
        if (r == ERROR_BUFFER_OVERFLOW) {
            //error checking if we have much memory
            printf("getAdapterAddresses wants %ld bytes.\n", asize);
            free(adapters);
        } else if (r == ERROR_SUCCESS) {
            break;
        } else {
            printf("Error from GetAdapterAddresses: %d\n", r);
            //ctrl + space to see all the available constants
            free(adapters);
            WSACleanup();
            return -1;
        }
    } while (!adapters);


    PIP_ADAPTER_ADDRESSES adapter = adapters;
    while (adapter) {
        printf("\nAdapter name: %S\n", adapter->FriendlyName);

        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
        while(address) {
            printf("\t%s", address->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");

            char ap[100];

            getnameinfo(address->Address.lpSockaddr, address->Address.iSockaddrLength, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);
            address = address->Next;
        }
        adapter = adapter->Next;
    }

    free(adapters);
    WSACleanup();
    return 0;
}

