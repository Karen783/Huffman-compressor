#ifndef COMPRESSOR_H_
#define COMPRESSOR_H_

typedef struct frecuencias {
    int frecuencia;
    char letra;
} fr;

fr *calcularFrecuencias(int *total_out);

#endif
 