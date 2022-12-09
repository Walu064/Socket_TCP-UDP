#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

//Funkcja konwertujaca otrzymany od clienta tekst na hex:
void printInputInHex(char buffer[1024], int numberOfSigns, FILE* fileHandler){
    char textFromClientInHex[1024];
    for(int i = 0, j = 0; i < numberOfSigns; i++, j+=2)
        sprintf(textFromClientInHex + j, "%02x", buffer[i] & 0xff);
    printf("\tPo konwersji (hex): %s\n", textFromClientInHex);
    fprintf(fileHandler, "%s\n", textFromClientInHex);
}

int main(int argc, char const* argv[])
{
    int server_fd, recvMsg, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    //Uchwyt do pliku wynikowego:
    FILE *fileHandler;

    // Deskryptor gniazda:
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //Informacja o porcie nasłuchiwania:
    printf("Serwer nasluchuje na 0.0.0.0:%d\n", PORT);

    while(1)
    {
        bzero(buffer, 1024);
        if ((valread = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("recvFrom");
            exit(EXIT_FAILURE);
            }else{
                int newProcess = fork();
                if(newProcess == 0){
                        //Odbieram PID klienta, nastepenie kopiuje do zmiennej filename i otwieram nowy plik:
                        if(buffer[0] == 'P' && buffer[1] == 'I' && buffer[2] == 'D' && buffer[3] == ':'){
                                char filename[1024];
                                strcpy(filename, buffer);
                                printf("Klient - %s\n", filename);
                                fileHandler = NULL;
                                if((fileHandler = fopen(filename, "a")) == NULL){
                                        printf("Nie udalo sie utworzyc pliku %s.\n");
                                }
                        }else{
                                printf("\tOtrzymano: %s", buffer);
                                printInputInHex(buffer, valread, fileHandler);
                                fclose(fileHandler);
                        }
                }
            }
    }

    return 0;
}