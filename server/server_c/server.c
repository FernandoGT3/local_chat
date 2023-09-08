#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <paths.h>

#define MAXCHAR 250
#define MAXCLIENT 100
#define PORT 8080

int main(){
    struct sockaddr_in server_address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };

    int server_fd;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Erro No Socket!\n");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(server_address);

    if(bind(server_fd, (struct sockaddr*)&server_address, addrlen) < 0){
        perror("Falha no Bind!!");
        exit(EXIT_FAILURE);
    }

    printf("Socket vinculado na porta: %d\n", PORT);

    if(listen(server_fd, 5) < 0){
        perror("Falha no Listen!!");
        exit(EXIT_FAILURE);
    }

    printf("Listening...\n");

    int cliente_fd[MAXCLIENT];
    char buffer[MAXCHAR];

    struct sockaddr_in cliente_address;
    socklen_t addrlen_size;

    int i = 0;

    while (1){
        if((cliente_fd[i] = accept(server_fd, (struct sockaddr*)&cliente_address, &addrlen_size)) < 0){
            perror("Falha no Accept!!");
            exit(EXIT_FAILURE);
        }else{
            recv(cliente_fd[i], buffer, MAXCHAR, 0);
            printf("%s Conectado!!\n", buffer);
            i++;
        }

        recv(cliente_fd[i], buffer, MAXCHAR, 0);

        for (int j = 0; j < i + 1; j++){
            if (j != i){
                send(cliente_fd[j], buffer, strlen(buffer), 0);
            }
        }

        if(strcmp(buffer, "Desconectado do Servidor...")){
            i--;
        }

        if (i == 0){
            break;
        }
    }

    shutdown((server_fd), SHUT_RDWR);

    return 0;

}