#include "compressor.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct pq_nodo
{
    char letra;
    int frecuencia;
    nodo *izquierda;
    nodo *derecho;
};

struct min_heap
{
    nodo **datos;  // arreglo de punteros a nodos
    int total;     // cuántos nodos hay ahora
    int capacidad; // cuántos caben
};

int insertar(heap *h, nodo *n)
{
    if (h->total == h->capacidad)
        return -1;
    int i = h->total;
    *(h->datos + i) = n;
    h->total = h->total + 1;

    // i = ultimo elemento del heap
    // Mientras el nodo no llegue al inicio
    while (i > 0)
    {
        int padre = (i - 1) / 2;
        // Si el nodo es menor o igual que el ultimo elemento
        if ((*(h->datos + padre))->frecuencia <= (*(h->datos + i))->frecuencia)
            break;

        nodo *temp = *(h->datos + padre);
        *(h->datos + padre) = *(h->datos + i);
        *(h->datos + i) = temp;
        i = padre;
    }
    return 0;
}

nodo *crear_hoja(char letra, int frecuencia)
{
    nodo *nodo_nuevo = malloc(sizeof(nodo));
    if (nodo_nuevo == NULL)
        return NULL;
    nodo_nuevo->letra = letra;
    nodo_nuevo->frecuencia = frecuencia;
    nodo_nuevo->izquierda = NULL;
    nodo_nuevo->derecho = NULL;

    return nodo_nuevo;
}

nodo *crear_padre(nodo *izq, nodo *der)
{
    nodo *padre = malloc(sizeof(nodo));
    if (padre == NULL)
        return NULL;
    padre->letra = '\0';
    padre->frecuencia = izq->frecuencia + der->frecuencia;
    padre->izquierda = izq;
    padre->derecho = der;
    return padre;
}

heap *crear_heap(int capacidad)
{
    heap *h = malloc(sizeof(struct min_heap));
    if (h == NULL)
        return NULL;

    h->datos = malloc(sizeof(nodo *) * capacidad);
    if (h->datos == NULL)
    {
        free(h);
        return NULL;
    }
    h->total = 0;
    h->capacidad = capacidad;
    return h;
}

nodo *extraer_minimo(heap *h)
{
    if (h->total == 0)
        return NULL;
    nodo *minimo = *(h->datos);                 // posicion 0;
    *(h->datos) = *(h->datos + (h->total - 1)); // mover el ultimo elemento al inicio del heap
    h->total = h->total - 1;

    // Ordenar el heap
    int i = 0;
    while (1)
    {
        int menor = i; // Elemento actual;
        int izq = 2 * i + 1;
        int der = 2 * i + 2;

        // Buscar el menor (puede ser el izq, el derecho, o el actual)
        if (izq < h->total && (*(h->datos + izq))->frecuencia < (*(h->datos + menor))->frecuencia)
            menor = izq;
        if (der < h->total && (*(h->datos + der))->frecuencia < (*(h->datos + menor))->frecuencia)
            menor = der;

        // Esto significa que el actual ya es el menor
        if (menor == i)
            break;

        // Hacer swap para ordenarlo
        nodo *temp = *(h->datos + i);
        *(h->datos + i) = *(h->datos + menor);
        *(h->datos + menor) = temp;
        i = menor;
    }
    return minimo;
}

nodo *crear_arbol(fr *tabla, int total)
{
    if (total == 0)
        return NULL;

    heap *h = crear_heap(total);

    for (int i = 0; i < total; i++)
    {
        nodo *hoja = crear_hoja((tabla + i)->letra, (tabla + i)->frecuencia);
        insertar(h, hoja);
    }

    // Repetir hasta que quede solo la raiz
    while (h->total > 1)
    {
        nodo *izq = extraer_minimo(h);
        nodo *der = extraer_minimo(h);
        nodo *padre = crear_padre(izq, der);
        if (padre == NULL)
            break;
        insertar(h, padre);
    }

    nodo *raiz = extraer_minimo(h);
    free(h->datos);
    free(h);
    return raiz;
}

static void asignar_codigo(fr *tabla, int total, char letra, const char *codigo)
{
    for (int i = 0; i < total; i++)
    {
        if ((tabla + i)->letra == letra)
        {
            size_t len = strlen(codigo);
            (tabla + i)->codigo_binario = malloc(len + 1);
            if ((tabla + i)->codigo_binario != NULL)
            {
                strcpy((tabla + i)->codigo_binario, codigo);
            }
            break;
        }
    }
}

static void recorrer(nodo *n, char *codigo, int profundidad, fr *tabla, int total)
{
    if (n == NULL)
        return;

    // Si es un nodo hoja
    if (n->izquierda == NULL && n->derecho == NULL)
    {
        *(codigo + profundidad) = '\0';
        asignar_codigo(tabla, total, n->letra, codigo);
        return;
    }

    *(codigo + profundidad) = '0';
    recorrer(n->izquierda, codigo, profundidad + 1, tabla, total);
    *(codigo + profundidad) = '1';
    recorrer(n->derecho, codigo, profundidad + 1, tabla, total);
}

void generar_codigos(nodo *raiz, fr *tabla, int total)
{
    if (raiz == NULL || tabla == NULL || total <= 0)
        return;

    char *codigo = malloc(total + 2);
    if (codigo == NULL)
        return;

    recorrer(raiz, codigo, 0, tabla, total);

    free(codigo);
}

static const char *buscar_codigo(const fr *tabla, int total, char c)
{
    for (int j = 0; j < total; j++)
    {
        if ((tabla + j)->letra == c)
            return (tabla + j)->codigo_binario;
    }
    return NULL;
}

void generar_archivo_binario(const char *ruta, const char *oracion, const fr *tabla, int total)
{
    FILE *f = fopen(ruta, "wb");
    if (f == NULL)
    {
        printf("No se pudo abrir el archivo");
        return;
    }

    unsigned char byte = 0;
    int nbits = 0;
    for (int i = 0; oracion[i] != '\0'; i++)
    {
        const char *codigo = buscar_codigo(tabla, total, oracion[i]);
        if (codigo == NULL)
        {
            fclose(f);
            return;
        }
        for (int k = 0; codigo[k] != '\0'; k++)
        {
            byte = (unsigned char)((byte << 1) | (codigo[k] - '0'));
            nbits++;
            if (nbits == 8)
            {
                fputc(byte, f);
                byte = 0;
                nbits = 0;
            }
        }
    }

    if (nbits > 0)
    {
        byte = (unsigned char)(byte << (8 - nbits));
        fputc(byte, f);
    }

    fclose(f);
}

void descomprimir_archivo(const char *ruta_comprimido, const char *ruta_descomprimido, nodo *raiz)
{
    FILE *comprimido = NULL;
    comprimido = fopen(ruta_comprimido, "rb");
    if (comprimido == NULL)
    {
        printf("No se pudo abrir el archivo\n");
        return;
    }

    FILE *descomprimido = NULL;
    descomprimido = fopen(ruta_descomprimido, "a");
    if (descomprimido == NULL)
    {
        printf("No se pudo abrir el archivo\n");
        return;
    }

    int total = raiz->frecuencia;
    nodo *actual = raiz;
    unsigned char byte;

    while (total > 0 && fread(&byte, 1, 1, comprimido) == 1)
    {
        for (int b = 7; b >= 0; b--)
        {
            int bit = (byte >> b) & 1;
            if (bit == 0)
                actual = actual->izquierda;
            else
                actual = actual->derecho;

            if (actual->derecho == NULL && actual->izquierda == NULL)
            {
                fputc(actual->letra, descomprimido);
                actual = raiz;
                total--;
            }
        }
    }
    fputc('\n', descomprimido);

    fclose(descomprimido);
    fclose(comprimido);
}

// ********* FUNCIONES PARA IMPRIMIR COSITAS *********//

void imprimir_archivo_binario(const char *ruta)
{
    FILE *f = fopen(ruta, "rb");
    if (f == NULL)
    {
        printf("No se pudo abrir el archivo\n");
        return;
    }

    unsigned char byte;
    while (fread(&byte, 1, 1, f) == 1)
    {
        for (int b = 7; b >= 0; b--)
            printf("%d", (byte >> b) & 1);
    }
    printf("\n");

    fclose(f);
}

void imprimir_tabla(fr *tabla, int total)
{
    if (tabla == NULL)
        return;

    for (int i = 0; i < total; i++)
    {
        char c = (tabla + i)->letra;
        const char *bin = (tabla + i)->codigo_binario ? (tabla + i)->codigo_binario : "";
        printf("'%c'| frec: %d | bin: %s\n", c, (tabla + i)->frecuencia, bin);
    }
}

void imprimir_arbol(nodo *n)
{
    if (n == NULL)
        return;
    printf("(%c:%d) ", n->letra, n->frecuencia);
    imprimir_arbol(n->izquierda);
    imprimir_arbol(n->derecho);
}

void imprimir_heap(heap *h)
{
    for (int i = 0; i < h->total; i++)
    {
        nodo *actual = *(h->datos + i);
        printf("(%c:%d) -> ", actual->letra, actual->frecuencia);
    }
    printf("\n");
}
// ********* FUNCIONES PARA LIBERAR MEMORIA *********//

void liberar_tabla(fr *tabla, int total)
{
    if (tabla == NULL)
        return;

    for (int i = 0; i < total; i++)
    {
        if ((tabla + i)->codigo_binario != NULL)
        {
            free((tabla + i)->codigo_binario);
        }
    }
    free(tabla);
}

void liberar_arbol(nodo *n)
{
    if (n == NULL)
        return;
    liberar_arbol(n->izquierda);
    liberar_arbol(n->derecho);
    free(n);
}