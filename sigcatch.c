#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>


// Manejador de señales
void signal_handler(int senial) {
    printf("%d: %s\n", senial, strsignal(senial));
}

int main()//int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = signal_handler; // Configurar el manejador de señales
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    // Registrar el manejador para todas las señales posibles
    for (int i = 1; i < NSIG; i++) {
        if (sigaction(i, &sa, NULL) == -1) {
            fprintf(stderr, "Error al registrar la señal %d\n", i);
            continue;
        }
    }
     // Esperar señales durante el tiempo definido
     while (1) {
         pause(); // Suspender el proceso hasta recibir una señal
     }
     
    exit(EXIT_SUCCESS);
}
