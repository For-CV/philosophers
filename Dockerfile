FROM debian:bookworm-slim

LABEL maintainer="RMS35"
LABEL description="Entorno de desarrollo C para 42 philosophers con Valgrind y Sanitizers"

# Instalación de herramientas
RUN apt-get update && apt-get install -y \
    build-essential \
    valgrind \
    gdb \
    cppcheck \
    nano \
    vim \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /philosophers
CMD ["/bin/bash"]
