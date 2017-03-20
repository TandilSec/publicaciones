# Makefile para la compilación de los ejemplos y la creacion de la imagen de
# Docker donde ejecutarlos.

NAME = tandilsec/introduccion-a-contenedores-en-linux
VERSION = 0.1.1

CC=gcc
CFLAGS=-Wall
EJEMPLOS=01-nuevo-namespace-mount 02-unirme-namespace 03-contenedor

01-nuevo-namespace-mount:
	$(CC) $(CFLAGS) $@.c -o $@

02-unirme-namespace:
	$(CC) $(CFLAGS) $@.c -o $@

03-contenedor:
	$(CC) $(CFLAGS) $@.c -o $@

compile: $(EJEMPLOS)

build:
	docker build -t $(NAME):$(VERSION) --rm .

all: compile build

clean:
	rm $(EJEMPLOS)
