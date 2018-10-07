#define _GNU_SOURCE
#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <unistd.h>

/* Función de ayuda para imprimir errores */
static void printerror(const char *msg) {
	printf("[-] %s, error=%d: %s\n", msg, errno, strerror(errno));
	exit(EXIT_FAILURE);
}

/* Definimos memoria para la pila del proceso hijo */
#define STACK_SIZE (1024*1024)
static char hijo_stack[STACK_SIZE];

/* Estructura para pasar parametros al proceso hijo */
struct args_hijo {
  char **argv;  /* Lista de argumentos */
};

/* Codigo del proceso hijo */
int proceso_hijo(void * datos) {
  struct args_hijo *args = (struct args_hijo *) datos;

  /* Montar el (pseudo) file system proc.
   * Primero es necesario desmontar el "/proc" que se encuentra montado
   * actualmente, luego es posible montar el nuevo "/proc".
   */
  if (umount2("/proc", 0) != 0) {
    printerror("Error desmontando proc!");
  }
  if (mount("proc", "/proc", "proc", 0, "") != 0) {
    printerror("Error montando nuevo proc!");
  }

  /* Ejecutar el comando recibido como argumento desde el proceso padre. */
  if (execv(args->argv[0], args->argv) != 0) {
    printerror("Error ejecutando el comando!");
  }
  exit(EXIT_FAILURE);
};

/* Rutina principal del proceso padre */
int main(int argc, char *argv[]){
  pid_t pid;
  struct args_hijo args;

  /* Se copian los parametros que el proceso padre recibió por línea de comandos
   * en la estructura a pasar al hijo.
   */
  args.argv = &argv[1];

  /* Se crea un nuevo proceso hijo mediante la syscall "clone", especificando
   * como flag un nuevo namespace de file system.
   */
	printf("[*] Creando un nuevo proceso hijo!\n");
	pid = clone(proceso_hijo, hijo_stack + STACK_SIZE, CLONE_NEWNS | SIGCHLD, &args);
  if (pid == -1){
		printerror("Error creando proceso hijo!");
	};

  /* Ahora conocemos el PID del proceso hijo creado. */
  printf("[*] PID del proceso hijo: %ld\n", (long) pid);

  /* Esperamos a que el proceso hijo termine la ejecución. */
  if (waitpid(pid, NULL, 0) == -1) {
    printerror("[-] Error esperando a que el hijo termine!");
  }

  exit(EXIT_SUCCESS);
}
