#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/* Rutina principal */
int main(int argc, char *argv[]){

  int fd;

  /* Chequear parametros */
  if (argc < 3) {
    printf("[-] Parametros invalidos!\n");
    exit(EXIT_FAILURE);
  }

  /* Abrir el archivo especificado por linea de comando con el descriptor del
   * namespace.
   */
  fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    printf("[-] Error abriendo descriptor del namespace!\n");
    exit(EXIT_FAILURE);
	};

  /* Unirme al namespace mediante la syscall "setns", pasando el descriptor del
   * namespace.
   */
  if (setns(fd, 0) == -1) {
    printf("[-] Error uniendome al namespace!\n");
    close(fd);
    exit(EXIT_FAILURE);
	};

  /* Ejecutar el comando especificado por línea de comando dentro del namespace.
   */
  execvp(argv[2], &argv[2]);

  /* Cerrar el archivo y finalizar. */
  close(fd);
  exit(EXIT_SUCCESS);
}
