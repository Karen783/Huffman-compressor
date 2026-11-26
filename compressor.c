#include "compressor.h"
#include <stdio.h>
#include <stdlib.h>

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

struct diccionario
{
    char key;
    char *binario;
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

void imprimir_heap(heap *h)
{
    for (int i = 0; i < h->total; i++)
    {
        nodo *actual = *(h->datos + i);
        printf("(%c:%d) -> ", actual->letra, actual->frecuencia);
    }
    printf("\n");
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

void imprimir_arbol(nodo *n)
{
    if (n == NULL)
        return;
    if (n->izquierda == NULL && n->derecho == NULL)
        printf("(%c:%d) ", n->letra, n->frecuencia);
    imprimir_arbol(n->izquierda);
    imprimir_arbol(n->derecho);
}

void liberar_arbol(nodo *n)
{
    if (n == NULL)
        return;
    liberar_arbol(n->izquierda);
    liberar_arbol(n->derecho);
    free(n);
}

int recorrer(nodo *n, char *codigo, int profundidad, dic *tabla, int cuenta)
{
    if (n == NULL)
        return cuenta;

    if (n->izquierda == NULL && n->derecho == NULL)
        return cuenta + 1;

    codigo[profundidad] = '0';
    cuenta = recorrer(n->izquierda, codigo, profundidad + 1, tabla, cuenta);
    codigo[profundidad] = '1';
    cuenta = recorrer(n->derecho, codigo, profundidad + 1, tabla, cuenta);
    return cuenta;
}