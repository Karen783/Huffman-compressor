#include "compressor.h"
#include <stdio.h>
#include <stdlib.h>

fr *calcularFrecuencias(int *total_out)
{
    int letra, total = 0, capacidad = 4;
    fr *array = malloc(sizeof(fr) * capacidad);
    
    if (array == NULL)
    {
        if (total_out != NULL)
            *total_out = 0;
        return NULL;
    }

    while ((letra = getchar()) != '\n' && letra != EOF)
    {
        int pos = -1;
        for (int i = 0; i < total; i++)
        {
            if (array[i].letra == (char)letra)
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
                fr *temp = realloc(array, capacidad * sizeof(fr));
                if (temp == NULL)
                {
                    free(array);
                    if (total_out != NULL)
                        *total_out = 0;
                    return NULL;
                }
                array = temp;
            }
            array[total].letra = (char)letra;
            array[total].frecuencia = 1;
            total++;
        }
    }

    if (total_out != NULL)
    {
        *total_out = total;
    }

    return array;
}
