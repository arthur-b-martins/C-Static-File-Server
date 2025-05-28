#include "server.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>      // close()
#include <sys/socket.h>  // socket(), bind(), listen(), accept()
#include <netinet/in.h>  // sockaddr_in
#include <arpa/inet.h>   // htons()


void accept_requests(int server_socket, struct sockaddr* address, socklen_t* addrlen, Bool v){
    while(1){
        int client_socket = accept(server_socket, address, addrlen);
        if(client_socket < 0){
            perror("\nAccept failed");
            exit(1);
        }

        if(v){
            printf("\nRequest accepted successfully");
        }

        char buffer[1024] = {0};

        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if(bytes_received < 0){
            perror("Read request failed");
            exit(1);
        }

        buffer[bytes_received] = '\0';

        if(v){
            printf("\nData received:\n\n %s\n", buffer);
        }

        char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "\r\n"
        "Hello-world!";

        if(send(client_socket, response, strlen(response), 0) < 0){
            perror("Send failed");
            exit(1);
        }


        if(v){
            printf("\nResponse sent successfully.");
        }

        if(close(client_socket) < 0){
            perror("\nClose client socket failed");
            exit(1);
        }
    }
}


void start_server(Port port, Bool v){
    if(v){
        printf("\n\nStarting server on port %" PRIu16"", port);
    }

  
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // socket() initialize a data struct socket in OS
    // and returns the File Descriptor to this socket 
    // (3 if there are no other files being used by this process)
    
    // AF = Adress family (ipv4, ipv6, local)
    // SOCK_STREAM = TCP, SOCK_DGRAM = UDP (Datagram)
    // 0 = Default to sock_strem, TCP

    if(server_socket == -1){
        perror("\nSocket failed");
        exit(1); 
    }

    if(v){
        printf("\nSocket on FD: %d", server_socket);
    }

    // now we need to create an address strcut with our IP and port
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    memset(&address, 0, addrlen);

    address.sin_family = AF_INET;               // ipv4
    address.sin_addr.s_addr = INADDR_ANY;       // Listen in all IPs 
    address.sin_port = htons(port);             // port, converted to big-endian if necessary


    // now we bind the address to our socket
    if(bind(server_socket, (struct sockaddr *) &address, addrlen) < 0){
        perror("\nBind failed");
        exit(1);
    }

    if(v){
        printf("\nBind done successfully.");
    }

    // Change the socket status to listen
    if(listen(server_socket, 10) < 0){
        perror("\nListen failed");
        exit(1);
    }
    // Now the OS will start receiving data,
    // and putting into the buffer queue
    // (our queue accepts only 10 requests at once)

    if(v){
        printf("\nServer is listening...");
    }


    accept_requests(server_socket, (struct sockaddr *) &address,(socklen_t *) &addrlen, v);


    // close the socket to avoid memory leak
    if( close(server_socket) == -1){
        perror("\nClose socket failed");
        exit(1);
    }

    if(v){
        printf("\n\nSocket closed.\n");
    }
}