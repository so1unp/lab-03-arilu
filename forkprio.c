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

#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>      // times()

int busywork(void)
{
    struct tms buf;
    for (;;) {
        times(&buf);
    }
}

void obtenerTiempo(){
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    long tiempo_usuario = usage.ru_utime.tv_sec;
    long tiempo_sistema = usage.ru_stime.tv_sec;

    long tiempo_total = tiempo_usuario + tiempo_sistema;
    int prioridad = getpriority(PRIO_PROCESS,0);

    printf("Hijo con pid %d (prioridad %2d):tiempo en ejecución \t%ld\n", getpid(), prioridad, tiempo_total);
            
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
            
            int nice_prio = reducir_prio ? 1 + i : 0;
            
            setpriority(PRIO_PROCESS, 0, nice_prio);
            signal(SIGTERM, obtenerTiempo);
            //Para que imprima el tiempo que realmente ejecutó

            //sumar los dos user cpu y system cpu
            busywork();

            // Calcular tiempo total
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
    

    printf("Todos los procesos hijos finalizaron.\n");
    return 0;
}


