#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void test_fork(int count, int wait);
void test_thread(int count, int wait);



int main(int argc, char *argv[]) 
{
    int modo, wait, count;
    
    // Chequea los  parametros
    if (argc < 3) {
        fprintf(stderr, "Uso: %s [-p | -h] [-w] cantidad\n", argv[0]);
        fprintf(stderr, "\t-p           Crear procesos.\n");
        fprintf(stderr, "\t-t           Crear hilos.\n");
        fprintf(stderr, "\t-w           Esperar que proceso o hilo finalice.\n");
        fprintf(stderr, "\tcantidad     Número de procesos o hilos a crear.\n");
        exit(EXIT_FAILURE);
    }
    
    modo = argv[1][1];  // debe ser p o t

    if (argc == 4) { 
        wait = argv[2][1];
        if (wait != 'w') {
            fprintf(stderr, "Error: opción invalida %s.\n", argv[2]);
            exit(EXIT_FAILURE);
        }
        count = atoi(argv[3]);
    } else {
        wait = 0;
        count = atoi(argv[2]);
    }
    
    if (count <= 0) {
        fprintf(stderr, "Error: el contador debe ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    if (modo != 'p' && modo != 't') {
        fprintf(stderr, "Error: opción invalida %s.\nUsar -p (procesos) o -t (hilos)\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    if (modo == 'p') {
        printf("Probando fork()...\n");
        test_fork(count, wait);
    } else if (modo == 't') {
        printf("Probando pthread_create()...\n");
        test_thread(count, wait);
    }

    exit(EXIT_SUCCESS);
}

//==========================================================================
// Código para la prueba con fork()
//==========================================================================
void test_fork(int num_pro, int wait)
{
    pid_t hijos[num_pro];

    for (int i = 0; i < num_pro; i++) {
        //Fork
        hijos[i] = fork();
        if (hijos[i] == 0) {  // Proceso hijo
            
            exit(0);

        }else if (hijos[i] > 0 && wait != 0) {  // Proceso padre
            waitpid(hijos[i], NULL, 0);
        }else if (hijos[i] < 0) {
            perror("Error al crear hijo");
            //retur 1;
        }
    }

}

//==========================================================================
// Código para la prueba con pthread_create()
//==========================================================================

void test_thread(int num_thr, int wait) 
{
}
