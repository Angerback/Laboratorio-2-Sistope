#include <stdlib.h>

typedef struct lista_d{
  double * dato;
  int largo;
  int cursor;
}lista_d;

lista_d * crear_lista_d(int largo){
    double * li = ( double * ) malloc(sizeof( double ) * largo);
    lista_d * l = ( lista_d * ) malloc(sizeof (struct lista_d));;
    l->dato = li;
    l->largo = largo;
    l->cursor = 0;
    return l;
}

void add_lista_d(lista_d * l, double num){
    int existe = 0;
    int i = 0;

    for (i = 0; i < l->largo; i++) {
        if((l->dato)[i] == num){
            existe = 1;
            break;
        }
    }

    if(!existe){
        if(l->cursor == l->largo){
            //Se debe pedir mas espacio
            l->largo = l->largo + 1;
            l -> dato = (double *) realloc((l->dato), sizeof(double) * (l->largo));
        }
        (l->dato)[l->cursor] = num;
        l->cursor = (l->cursor) + 1;
    }
    return;
}
