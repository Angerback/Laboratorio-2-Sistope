#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>

#include "lista.h"

struct parametrosHebra {
    pthread_mutex_t * mutexBuffer;
    pthread_mutex_t * mutex_interseccion;
    lista * listas;
    lista * interseccion;
    int indice_corto;
    int indice_a_revisar;
    int inicio;
    int fin;
}parametrosHebra;


int equipos_abriendo_archivo; //Trabaja como semaforo contador
int n_hebras;
char * ruta;
int n_listas;

pthread_mutex_t archivo = PTHREAD_MUTEX_INITIALIZER;

/*
    Función que abre y lee un archivo con el formato dado en el enunciado.
    Recbie la ruta del archivo y retorna un arreglo con las listas extraídas
    del archivo.
*/
struct lista * abrir_archivo(char * ruta){
    FILE * archivo;
    char * buffer;
    char cursor;
    int i= 0;
    //printf("Archivo abierto.\n");
    archivo = fopen(ruta, "r");
    int largo_lista = 0, numero = 0;
    n_listas = 0;
    lista * retorno/* = (lista *) malloc(sizeof(struct lista))*/; //espacio para una lista
    if(archivo){
        //printf("Archivo abierto.\n");
        cursor = getc(archivo);
        while(cursor != EOF){
            i = 0;
            //printf("Lista\n");
            n_listas++;
            if(n_listas == 1){
                retorno = (lista *) malloc(sizeof(struct lista));
            }else{
                retorno = (lista *) realloc(retorno, sizeof(struct lista) * (n_listas));
            }

            //Codigo para archivo valido
            buffer = (char *) malloc(sizeof(char));

            //Se lee elemento por elemento (es decir, cada espacio hay un elemento)
            //cursor = getc(archivo);
            while(cursor!=' '){
                buffer[i] = cursor;
                cursor = getc(archivo);
                    if(cursor != ' '){
                      i++;
                      buffer = realloc(buffer, (i+1)*sizeof(char));
                    }
            }
            //Una vez sacado el primer término, este se convierte a un entero
            largo_lista = atoi(buffer);
            //printf("buffer: (%s)", buffer);
            //printf("- Largo lista %d: %d\n", n_listas, largo_lista);
            /*
            Obtenido el largo de la lista, los siguientes valores son numeros de
            la lista
            */
            cursor = getc(archivo);
            lista * list = crear_lista(largo_lista);
            while(cursor != '\n'){
                i = 0;
                free(buffer);
                buffer = (char *) malloc(sizeof(char));
                while(cursor!=' '){
                    if(cursor == '\n'){
                        //printf("Salto de linea \n");
                        break;
                    }
                    buffer[i] = cursor;
                    cursor = getc(archivo);
                    if(cursor != ' ' && cursor != '\n'){
                      i++;
                      buffer = realloc(buffer, (i+1)*sizeof(char));
                    }
                }

                // Luego de que se tiene un numero, se pasa a int y se agrega a la lista
                int num = atoi(buffer);
                //printf("- Elemento de la lista: %d\n", num);
                add_lista(list, num);
                // se itera para todos los numeros.

                if(cursor == '\n'){
                    //printf("Salto de linea\n");
                    //cursor = getc(archivo);
                    break;
                }
                //printf("Esto no debe aparecer luego de un salto de linea\n");
                cursor = getc(archivo);
            }
            //printf("Holi\n");
            /*
            printf("asfdf\n");
            int k = 0;
            for (k = 0; k < list->largo; k++) {
                printf("%d  ", (list->dato)[k]);
            }
            printf("\n");
            */
            cursor = getc(archivo);
            //printf("cursor: %c\n", cursor);
            retorno[n_listas-1].dato = list->dato;
            retorno[n_listas-1].largo = list->largo;
            retorno[n_listas-1].cursor = list->cursor;
        }
        printf("Terminando\n");
    }
    /*
    i = 0;
    int j = 0;
    for(i = 0; i < 3; i++){
        printf("Lista %d \n", i);
        for (j = 0; i < (retorno[i].largo); j++) {
            printf("Elemento de lista: %d\n", (retorno[i].dato)[j]);
        }
    }*/
    return retorno;
}

void *hebra(void * context){
    struct parametrosHebra * ph = context;
    lista * listas = ph -> listas;
    lista * interseccion = ph -> interseccion;
    int indice_corto = ph -> indice_corto;
    int indice_a_revisar = ph -> indice_a_revisar;
    pthread_mutex_t * mutexBuffer = ph -> mutexBuffer;
    pthread_mutex_t * mutex_interseccion = ph -> mutex_interseccion;
    int inicio = ph -> inicio;
    int fin = ph -> fin;

    printf("Hebra preparada\n");

    // Con cada hebra preparada, se ejecuta el algoritmo solicitado:
    int i = 0;
    /*
    Por cada elemento de S (recorrido secuencial O(S)), hacer búsqueda binaria
    en los elementos de K que pueda visualizar la hebra (búsqueda binaria O ((K/P) log
    (K/P)).
    */
    for(i = 0; i < listas[indice_corto].largo; i++){
        //Se bloquea el elemento puntual que se desea mirar
        int elemento_s;
        pthread_mutex_lock(&(mutexBuffer[i]));
            // se saca una copia del elemento para no limitar la concurrencia
            elemento_s = listas[indice_corto].dato[i];
        pthread_mutex_unlock(&(mutexBuffer[i]));
        //Realizar busqueda binaria en el segmento visible de K.
        int piso = inicio, cielo = fin, pivote, encontrado = 0;
        //printf("(Hebra) Inicio: %d, Fin: %d, piso: %d, cielo: %d\n",inicio, fin, piso, cielo);
        while((piso <= cielo) && (!encontrado)){
            pivote = (piso + cielo) / 2;
            if(elemento_s == listas[indice_a_revisar].dato[pivote]){
                encontrado = 1;
            }
            else if(elemento_s < listas[indice_a_revisar].dato[pivote]){
                cielo = pivote - 1;
            }else{
                piso = pivote + 1;
            }
        }

        if(encontrado){
            printf("Elemento intersecto: %d\n", elemento_s);
            /* Si se encontró el documento revisado en la lista K,
            entonces agregar elelemento a S’ (bloqueando la lista S’
            para acceso exclusivo).
            */
            pthread_mutex_lock(mutex_interseccion);
                //agregar
                //if((interseccion -> largo) == 1){

                    //Se agrega el primer elemento.
                    //(interseccion -> dato)[0] = elemento_s;
                    add_lista(interseccion, elemento_s);
                /*}else{
                    // Se debe agrandar la lista.
                    printf("Agrandar------------------\n");
                    agrandar_lista(interseccion, (interseccion->largo)+1);
                    add_lista(interseccion, elemento_s);
                }*/
            pthread_mutex_unlock(mutex_interseccion);
        }

    }



    pthread_exit(NULL);
}

/*
    Funcion que provee un comportamiento para cada equipo
    Se comporta como una hebra. Es una hebra.
*/
void *equipo(){
    printf("Hola soy un equipo!\n");
    lista * listas;
    //Cada equipo abre el archivo por separado:
    pthread_mutex_lock(&archivo);
        //Se lee el archivo de texto, protegido
        listas = abrir_archivo(ruta);
    pthread_mutex_unlock(&archivo);
    //Se debe buscar la lista más corta en el arreglo.
    //printf("0\n");
    int indice_corto = 0, i;
    for (i = 0; i < n_listas; i++) {
        if(listas[i].largo < listas[indice_corto].largo){
            indice_corto = i;
        }
    }
    int listas_count = 0;

    while(listas_count < n_listas){
        if(listas_count != indice_corto){
            printf("Lista corta: largo: %d, cursor: %d\n", listas[indice_corto].largo , listas[indice_corto].cursor);
            for(i = 0; i < listas[indice_corto].largo ; i++){
                printf("%d ", (listas[indice_corto].dato)[i]);
            }
            printf("\n");

            //printf("1\n");
            //printf("Indice Corto: %d\n", indice_corto);
            //Conociendo la lista mas corta, se puede comenzar a intersectar.
            /*
            A cada hebra se le debe entregar un subconjunto de la siguiente lista a revisar
            K, es decir se le entregará k, que es una sublista de K (k e K).
            Además, cada hebra debe tener acceso a la lista más corta, y solo debe
            bloquearla cada vez que va a a leer un elemento para proveer concurrencia.
            */
            // Luego de que cada equipo ha leido el archivo de texto, se puede competir:
            // Cada equipo debe inicializar a sus hebras participantes:
            pthread_t *hebras = ( pthread_t * ) malloc ( sizeof( pthread_t ) * n_hebras );

            //Se inicializan los semaforos para cada elemento de la lista más corta:
            //Un semaforo por elemento (se bloquea la lista de a un elemento)
            pthread_mutex_t * mutex_s = (pthread_mutex_t * ) malloc(sizeof(pthread_mutex_t) * listas[indice_corto].largo);

            int indice_relativo = listas_count;
            int hebra_count;
            int inicio_subconjunto = 0;
            int tamano_subconjunto = listas[indice_relativo].largo / n_hebras;
            struct parametrosHebra ph[n_hebras];

            // Intersección es S'
            lista * interseccion = crear_lista(1);

            // Se inicializan los semaforos
            // hay problemas aqui
            // Se crea mutex para proteger la lista de intersección S' contra escritura
            pthread_mutex_t * mutex_interseccion = (pthread_mutex_t * ) malloc(sizeof(pthread_mutex_t) );
            //* mutex_interseccion = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
            pthread_mutex_init(mutex_interseccion, NULL);
            int semaforos = 0;
            for (semaforos = 0; semaforos < listas[indice_corto].largo; semaforos++) {
                //mutex_s[semaforos] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
                //Se inicializan los semaforos para proteger cada elemento de S
                //Nunca se necesitarán más que la cantidad inicial.
                pthread_mutex_init(&(mutex_s[semaforos]), NULL);
            }

            for(hebra_count = 0; hebra_count < n_hebras; hebra_count++){
                //A cada hebra se le pasa un subconjunto de la lista K.
                ph[hebra_count].mutexBuffer = mutex_s;
                ph[hebra_count].listas = listas;
                ph[hebra_count].indice_corto = indice_corto;
                ph[hebra_count].indice_a_revisar = indice_relativo;
                ph[hebra_count].inicio = tamano_subconjunto * hebra_count;
                ph[hebra_count].fin = ph[hebra_count].inicio + tamano_subconjunto - 1;
                ph[hebra_count].interseccion = interseccion;
                ph[hebra_count].mutex_interseccion = mutex_interseccion;
                //printf("Inicio: %d, Fin: %d\n", ph[hebra_count].inicio, ph[hebra_count].fin);
                pthread_create(&hebras[hebra_count], NULL, &hebra, &(ph[hebra_count]));
            }


            //Como minimo, hay que esperar a todas las hebras.
            //Es la segunda condición para permitir la colaboración
            for(i=0;i<n_hebras;i++){
                pthread_join(hebras[i],NULL);
            }
            /*
            //Mostrar intersección:
            printf("Intersección: largo: %d, cursor: %d\n", interseccion -> largo , interseccion -> cursor);
            for(i = 0; i < interseccion -> largo ; i++){
                printf("%d ", (interseccion->dato)[i]);
            }
            printf("\n");
            */
            //Se debe cambiar la lista corta por la intersección.
            listas[indice_corto].dato = interseccion -> dato;
            listas[indice_corto].largo = interseccion -> largo;
            listas[indice_corto].cursor = interseccion -> cursor;

            free(interseccion);
            free(hebras);
            free(mutex_s);
            free(mutex_interseccion);
        }
        listas_count++;
    }
    pthread_exit(NULL);
}

int main(int argc, char * argv[]){

    int n_equipos = 0;
    n_hebras = 0;
    ruta = 0; //Relativa

    int arg_index = 1;

    while(arg_index < argc - 1){
        if(!strcmp(argv[arg_index], "-g")){ //Cantidad de equipos
            n_equipos = atoi(argv[arg_index + 1]);
        }
        else if(!strcmp(argv[arg_index], "-h")){ // Hebras por equipo
            n_hebras = atoi(argv[arg_index + 1]);
        }
        else if(!strcmp(argv[arg_index], "-i")){ // Ruta del archivo de entrada
            ruta = argv[arg_index + 1];
        }
        // Se aumenta de a dos ya que siempre se recibe el identificador del
        // parametro y luego el valor.
        arg_index += 2;
    }
    printf("Equipos: %d, Hebras: %d, Ruta: %s\n", n_equipos, n_hebras, ruta);

    if(n_equipos < 0 || n_hebras < 0 || ruta <= 0){
        return 1;
    }

    /*
    //Debería ser capaz de imprimirlas:

    int i = 0, j = 0;
    for(i = 0; i < 3; i++){
        printf("Lista %d \n", i);
        lista l = listas[i];
        printf("Largo lista: %d\n", l.largo);
        for (j = 0; j < l.largo; j++) {
            printf("%d  ", l.dato[j]);
        }
        printf("\n");
    }
    */
    //Luego de tener las listas, se puede inicar la competencia.


    equipos_abriendo_archivo = n_equipos;
    pthread_t *equipos = ( pthread_t * ) malloc ( sizeof( pthread_t ) * n_equipos );

    int hebra_count;
    for(hebra_count=0; hebra_count < n_equipos; hebra_count++){
        pthread_create(&equipos[hebra_count], NULL, &equipo, NULL);
    }


    //Como minimo, hay que esperar a todas las hebras.
    //Es la segunda condición para permitir la colaboración
    int i;
    for(i = 0; i < n_equipos ; i++){
        pthread_join(equipos[i], NULL);
    }

    return 0;
}
