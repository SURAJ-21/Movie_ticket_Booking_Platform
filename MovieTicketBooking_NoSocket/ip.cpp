#include <iostream>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> // Include this header for gethostname

int main() {
    char host[256];
    gethostname(host, sizeof(host));

    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, NULL, &hints, &res) != 0) {
        std::cerr << "Unable to get address info." << std::endl;
        return EXIT_FAILURE;
    }

    char ip[INET_ADDRSTRLEN];
    for (p = res; p != nullptr; p = p->ai_next) {
        void *addr;
        struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in*>(p->ai_addr);
        addr = &(ipv4->sin_addr);

        // Convert the IP to a string
        inet_ntop(p->ai_family, addr, ip, sizeof ip);
    }

    freeaddrinfo(res); // Free the linked list

    std::cout << "Your IP address is: " << ip << std::endl;

    return EXIT_SUCCESS;
}
