#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



#define SIZE 1024



void menu()
{
    printf("1. Archivo de sequencia\n");
    printf("2. Archivo de referencia\n");
    printf("3. Comparar los archivos\n");
    printf("4. Salir\n");
    printf("\n");
}


void processadorArch(int sockfd)
{
    FILE *fp;
    char fileName[80];
    char path[80] = "archivos/";
    printf("Archivo a ingresar: ");
    scanf("%s", fileName);
    strcat(path, fileName);

    fp = fopen(path, "r");

    if (fp != NULL)
    {
        char data[SIZE] = {0};
        while (fgets(data, SIZE, fp) != NULL)
        {
            if (send(sockfd, data, sizeof(data), 0) == -1)
            {
                perror("Error mandando datos al servidor...");
                exit(1);
            }
            bzero(data, SIZE);
        }
        fclose(fp);
    }
    else
    {
        perror("Error abriendo el archivo\n");
    }
}

void comparar(int sockfd){
    int n;
    char buffer[SIZE];
    while (1)
        {
            n = recv(sockfd, buffer, SIZE, 0);
            if (n <= 0)
            {
                break;
                return;
            }
            printf("%s", buffer);
            bzero(buffer, SIZE);
        }
        sleep(1);
        printf("\n");
}

void mandar(struct sockaddr_in servaddr, int sockfd, char *charValue)
{
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    send(sockfd, charValue, 2 * sizeof(char), 0);
}

int main(int argc, char **argv)
{

    struct sockaddr_in servaddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6789);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1)
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        int option = 0;
        char charValue[2];
        menu();

        fflush(stdin);
        printf("Seleccionar opcion: ");
        scanf("%d", &option);

        charValue[0] = option + '0';
        charValue[1] = '\0';

        mandar(servaddr, sockfd, charValue);
        if ((strcmp(charValue, "1") == 0) || strcmp(charValue, "2") == 0)
        {
            processadorArch(sockfd);
        }
        else if (strcmp(charValue, "3") == 0)
        {
            printf("Esperando... processando...\n");
            comparar(sockfd);
        }
        else if (strcmp(charValue, "4") == 0)
        {
            printf("saliendo del session\n");
            close(sockfd);
            shutdown(sockfd, SHUT_RDWR);
            exit(1);
        }

        bzero(charValue, 2);
        close(sockfd);
    }
}