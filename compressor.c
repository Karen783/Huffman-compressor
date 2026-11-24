#include <stdlib.h>
#include "compressor.h"
#include <stdio.h>
#include <stdlib.h>

struct frecuencias
{
    int frecuencia;
    char letra;
};

fr *calcularFrecuencias()
{
    int letra, total = 0, capacidad = 4;
    fr *array = malloc(sizeof(fr) * capacidad);
    if (array == NULL)
    {
        printf("Upsi dupsi \n");
    }

    while ((letra = getchar()) != '\n' && letra != EOF)
    {
        int pos = -1;
        for (int i = 0; i < total; i++)
        {
            if (array[i].letra == letra)
            {
                pos = i;
                break;
            }
        }

        if (pos != -1)
        {
            array[pos].frecuencia++;
        }

        else
        {
            if (total == capacidad)
            {
                capacidad *= 2; // duplicar el tamaño :)
                array = realloc(array, capacidad * sizeof(fr));
            }
            array[total].letra = letra;
            array[total].frecuencia = 1;
            total++;
        }
    }
    for (int i = 0; i < total; i++)
    {
        printf("%c %d\n", array[i].letra, array[i].frecuencia);
    }
}
