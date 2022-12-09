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
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    //Uchwyt do pliku wynikowego:
    FILE *fileHandler;

    // Deskryptor gniazda:
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Przyłączenie gniazda do wskazanego portu:
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
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
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);

        }

        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
            }else{
                //Utworzenie nowego procesu dla kazdego zaakceptowanego klienta:
                int newProcess = fork();
                if(newProcess == 0){
                //Odbieram PID klienta, nastepenie kopiuje do zmiennej filename i otwieram nowy plik:
                bzero(buffer, 1024);
                valread = read(new_socket, buffer, 1024);
                //Utworzenie nowego pliku za pomoca uchwytu fileHandler i otwarcie go:
                char filename[1024];
                strcpy(filename, buffer);
                printf("Klient - %s\n", filename);
                fileHandler = NULL;
                if((fileHandler=fopen(filename, "w"))==NULL){
                        printf("Nie udalo sie utworzyc pliku %s.\n", filename);
                }

                while(1){
                    bzero(buffer, 1024);
                    valread = read(new_socket, buffer, 1024);
                    printf("\tOtrzymano: %s", buffer);
                    if (strcmp(buffer, "exit\n") == 0){
                        printf("Zamkniete polaczenie.");
                        close(new_socket);
                        break;
                    }
                    //Zapis do pliku wynikowego odbywa sie za pomoca fprinf() w funkcji printInputInHex()
                    printInputInHex(buffer, valread, fileHandler);
                }
            //Zamknięcie pliku do ktorego zapisane zostaly skonwertowane dane od clienta:
            fclose(fileHandler);
            }
        }
    }
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}