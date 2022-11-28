
#include <string.h>
#include <limits.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024
#define num 256
#define MAX_LINE_LENGTH 100000

void buscador()
{
    unsigned int contline = 0;
    long tamArch;
    char act;
    int icant, counterbuscar, charCont, Contador;
    char cScan;
    char *subS[1000];
    int contE = 0;
    char *path;
    char *refS;
    FILE *file, *file2;
    char line[MAX_LINE_LENGTH];
   

    int ind;
    int SubContador = 0;
    float pE;

    icant = 0;
    counterbuscar = 0;

    file = fopen("archivos/secuencia.txt", "r");

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        tamArch = ftell(file);
        rewind(file);

        refS = (char *)malloc(sizeof(char) * tamArch);

        fread(refS, 1, tamArch, file);

        while (strlen(refS) > tamArch)
        {
            memmove(&refS[strlen(refS) - 1], &refS[strlen(refS)], strlen(refS) - (strlen(refS) - 1));
        }
    }
    else
    {
        printf("Error al leer. \n");
        exit(1);
    }

    char *temp = (char *)malloc(sizeof(char) * 100000);
    for (int i = 0; i <= tamArch; i += 100000)
    {
        strncpy(temp, refS + i, 100000);
    }
    fclose(file);

   

    file2 = fopen("archivos/referencia.txt", "r");

    if (!file)
    {
        perror("s_cerevisia_2022_03.seq");
        exit(1);
    }

    int iSize = strlen(refS);
    char *p;
    char *result = strstr(refS, line);

    int f, nf, ant, tot, uAl, si, no;
    si = 0;
    no = 0;
    ant = 0;
    f = 0;
    nf = 0;
    tot = 0;
    uAl = 0;

    FILE *fptr;
    fptr = fopen("archivos/resultados.txt", "w");

#pragma omp parallel for
    for (int y = 0; y < 1000; y++)
    {
        #pragma omp critical
        while (fscanf(file2, "%s", line) == 1)
        {
            if (strcmp(line, "\0") != 0)
            {
                contE++;
                fprintf(fptr, "%s \n",line);
                char *resultados = strstr(refS, line);

                if (resultados)
                {
                    int position = resultados - refS;
                    uAl = strlen(line);
                    int end = position + uAl;

        
                    fprintf(fptr, "A partir del caracter: %d\n", position);

                    tot += uAl;
                    si++;
                }
                else
                {
                    fprintf(fptr, "no se encontro\n");
                    no++;
                }
            }
        }
    }

    int lengthf = strlen(refS);
    int numXs = 0;
    for (int x = 0; x < lengthf; x++)
    {
        if (refS[x] == 'X')
        {
            numXs++;
        }
    }


    pE = (iSize * 1.0) / (tot * 1.0);
    pE *= 100;

    fprintf(fptr, "El archivo cubre el %f por ciento del genoma de referencia\n", pE);
    fprintf(fptr, "Mapeadas %d\n", si);
    fprintf(fptr, "No Mapeadas %d\n", no);

    free(refS);
    fclose(fptr);

    if (fclose(file))
    {
        exit(1);
        perror(path);
    }
}