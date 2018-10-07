# Dockerfile de ejemplo para simplificar la ejecución de los ejemplos

FROM debian:jessie

RUN apt-get update && apt-get install -y \
	gcc \
	vim

COPY . /TandilSec/
WORKDIR /TandilSec
CMD ["bash"]
