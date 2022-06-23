# Compilação e Versão GCC

- **Versão**: 9.3.0
- **Compilação**: gcc -o trab2 trab2.c -lpthread -lm -Wall

Caso prefira, ultilizar o makefile:

```sh
mkdir Trab2
cd Trab2
make
./trab2 <nº de thread> <Tam. Blocos> <nome arq. entrada>.txt <nome arq. saida>.txt
```

# Script

o arquivo 'testes.sh' é um script pra realizações de testes.

o arquivo 'gerarArquivo.sh' é um script que gerar uma entrada para o programa
