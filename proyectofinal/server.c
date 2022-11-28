#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "archivos/buscador.h"

#define SIZE 1024

void archivoHandler(int sockfd, char option)
{
    printf("datos recividos...\n");
    int n;

    FILE *fp;
    char buffer[SIZE];

    if (strstr(&option, "1") != NULL)
    {
        printf("parseando archivo recibido... \n");
        fp = fopen("archivos/secuenciaProcessado.txt", "w");
        if (fp == NULL)
        {
            perror("error no se pude crear archivo...");
            exit(1);
        }
        while (1)
        {
            n = recv(sockfd, buffer, SIZE, 0);
            if (n <= 0)
            {
                break;
                return;
            }
            fprintf(fp, "%s", buffer);
            bzero(buffer, SIZE);
        }
        fclose(fp);
    }
    else if (strstr(&option, "2") != NULL)
    {
        printf("parseando archivo recibido...\n");
        fp = fopen("archivos/referenciaprocessado.txt", "w");
        if (fp == NULL)
        {
            perror("error no se pude crear archivo...");
            exit(1);
        }
        while (1)
        {
            n = recv(sockfd, buffer, SIZE, 0);
            if (n <= 0)
            {
                break;
                return;
            }
            fprintf(fp, "%s", buffer);
            bzero(buffer, SIZE);
        }
        fclose(fp);
    }
    else if (strstr(&option, "3") != NULL)
    {
        printf("Procesando... \n");
        buscador();

        fp = fopen("archivos/resultados.txt", "r");

        while (fgets(buffer, SIZE, fp) != NULL)
        {
            if (send(sockfd, buffer, sizeof(buffer), 0) == -1)
            {
                perror("error mandando archivo al cliente");
                exit(1);
            }
            bzero(buffer, SIZE);
        }
    }
}

int main(int argc, char **argv)
{

    struct sockaddr_in servaddr;
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6789);

    bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(sock, 5);
    printf("servidor abierto...\n");

    while (1)
    {
        printf("Esperando cliente...\n");
        int sockfd = accept(sock, (struct sockaddr *)NULL, NULL);

        if (sockfd >= 0)
        {
            char buffer[SIZE];
            char option;

            if ((recv(sockfd, buffer, SIZE, 0)) > 0)
            {
                option = buffer[0];
                bzero(buffer, SIZE);

                
                if (strcmp(&option, "4") == 0)
                {
                    printf("Cerrando Servidor...");
                    close(sockfd);
                    shutdown(sockfd, SHUT_RDWR);
                    exit(1);
                }
                else
                {
                    archivoHandler(sockfd, option);
                }
            }
        }
        close(sockfd);
    }
}