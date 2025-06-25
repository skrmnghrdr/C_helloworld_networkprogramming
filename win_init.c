/* win_init.c*/

#include <stdio.h>
#include <winsock2.h>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif


#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    WSADATA d;

    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        printf("Failed to initialize! \n");
        return -1;
    }

    /*win_list.c continued */

    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters;
    do 
    {
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);
        if (!adapters) {
            //malloc returns 0 if failed, so we check
            printf("Couldn't allocate %ld bytes for adapters!. \n", asize);
            WSACleanup();
            return -1;
        }
        
        int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);
        //passing AF_INET, or AF_INET6 would make windwos return only said ip.

        if (r == ERROR_BUFFER_OVERFLOW) {
            printf("GetAdapterAddressess wants %ld bytes.\n", asize);
            free(adapters);
        } else if (r == ERROR_SUCCESS) {
            break;
        } else {
            printf("Error from GetAdapterAddresses: %d\n", r);
            free(adapters);
            WSACleanup();
            return -1;
        }
    } while (!adapters);

    /* page 35 continuation*/

    PIP_ADAPTER_ADDRESSES adapter = adapters;
    while (adapter) {
        printf("\nAdapter name: %S\n", adapter->FriendlyName);

        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
        while (address) {
            printf("\t%s",
            address->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");

            char ap[100];
            
            getnameinfo(address->Address.lpSockaddr,
            address->Address.iSockaddrLength,
            ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);
            address = address->Next;
        }
        adapter = adapter->Next;
    }

}

