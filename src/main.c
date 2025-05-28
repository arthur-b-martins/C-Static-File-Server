#include "server/server.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


#define PORT_TAG "-p"
#define DEFAULT_PORT 8080

#define VERBOSE_TAG "-v"
#define VERBOSE_INACTIVE 0
#define VERBOSE_ACTIVE 1

int main(int argc, char *argv[]){
    Port port = DEFAULT_PORT;
    Bool verbose = VERBOSE_INACTIVE;

    if(argc >= 2){
        for(int i = 0; i < argc; i++){
            if(strcmp(VERBOSE_TAG, argv[i]) == 0){
                verbose = VERBOSE_ACTIVE;
            }
            
            if(strcmp(PORT_TAG, argv[i]) == 0 && argc >= 3){
                port = atoi(argv[i + 1]);

                if(port < 1024 || port > 65535){
                    fprintf(stderr, "ERROR: Invalid port number!");
                    exit(1);
                }
            }
        }
    }

    if(verbose){
        printf("\nVerbose mode active.");
        printf("\nPort: %" PRIu16, port);
    }

    start_server(port, verbose);
    return 0;
}