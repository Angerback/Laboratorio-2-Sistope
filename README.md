# README #

### Laboratorio 2 SISTOPE Usach. Hebras y concurrencia ###

* Laboratorio escrito en C para realizar competencia entre equipos de hebras que intersectan listas de numeros.
* Version 1.0
* [Pthread](https://computing.llnl.gov/tutorials/pthreads/)
* Autores: Sebastián Meneses y Matías Calderon

### Instrucciones ###

* Ir al directorio raíz del proyecto (la carpeta donde se encuentra este README).
* En una terminal linux, ejecutar el comando

```
#!c

make
```

* Para ejecutar el programa, dirigirse a la carpeta "build" recién creada:


```
#!bash

cd ./build
```

* Colocar un archivo de listan esta carpeta, conforme al formato especificado en el enunciado.
* Ejecutar el archivo "competencia":


```
#!bash

./competencia -g 4 -h 4 -i lista.dat
```

El comando anterior ejecutará el programa con 4 equipos de 4 hebras para el archivo "lista.dat"