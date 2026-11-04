# AtvSync

Sincronização entre passageiros e vagão de metrô em C com pthreads.

## Arquivos

- `sync_car.h` — declaração da struct e protótipos.
- `sync_car.c` — arquivo com a lógica.
- `sync_car-runner.c` — teste oficial fornecido pelo professor.
- `Makefile` — compilação, execução e limpeza.
- `evidencias.log` — terminal.

## Compilar

    make


## Executar

    ./sync_car-runner

O `sync_car-runner` deve terminar com `Looks good!`.

## Limpar

    make clean

## Sistema testado

Linux (Ubuntu 22.04), gcc 11.4.