#include <sys/times.h>
#include <unistd.h>       // Para fork(), getpid(), sleep(), y pause()
#include <sys/types.h>    // Para tipos como pid_t
#include <sys/time.h>     // Para funciones relacionadas con tiempos
#include <sys/resource.h> // Para setpriority() y funciones relacionadas
#include <signal.h>       // Para sigaction() y kill()
#include <time.h>         // Para la función time()
#include <stdio.h>        // Para printf()
#include <stdlib.h>       // Para exit(), atoi()
#include <sys/wait.h>     // Para wait()

void busywork(time_t start_time, time_t max_duration) {
    struct tms buf;
    while (time(NULL) - start_time < max_duration) {
        times(&buf);
    }
}
//Ejemplo ./forkprio 3 2 1
//Se ejecutan 3 forks, con 2 seg de ejecución cada uno y se reduce la prioridad progresivamente
int main(int argc, char* argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Uso: %s <num_hijos> <segundos> <reducir_prioridad>\n", argv[0]);
        return 1;
    }

    int num_hijos = atoi(argv[1]);
    int tiempo_ex = atoi(argv[2]);
    int reducir_prio = atoi(argv[3]);
    pid_t hijos[num_hijos];

    for (int i = 0; i < num_hijos; i++) {
        //Fork
        pid_t pid = fork();
        if (pid == 0) {  // Proceso hijo
            int nice_prio = reducir_prio ? 20 + i : 0;
            setpriority(PRIO_PROCESS, 0, nice_prio);

            //Para que imprima el tiempo que realmente ejecutó
            time_t tiempo_comienzo = time(NULL);
            busywork(time(NULL), tiempo_ex);
            time_t tiempo_fin = time(NULL);
            printf("Hijo con pid %d (prioridad %2d):\t%3ld\n", getpid(), nice_prio, tiempo_fin - tiempo_comienzo);
            exit(0);
        } else if (pid > 0) {  // Proceso padre
            hijos[i] = pid;
        } else {
            perror("Error al crear hijo");
            return 1;
        }
    }
    

    if (tiempo_ex > 0) {
        sleep((unsigned int)tiempo_ex);
        for (int i = 0; i < num_hijos; i++) {
            //Matar el proceso 
            kill(hijos[i], SIGTERM);
        }
    } else {
        printf("Ejecutando indefinidamente.\n");
        while (1) pause();
    }

    for (int i = 0; i < num_hijos; i++) {
        wait(NULL);  
    }

    printf("Todos los procesos hijos finalizaron.\n");
    return 0;
}


