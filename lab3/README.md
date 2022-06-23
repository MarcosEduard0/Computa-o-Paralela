# Compilação e Versão GCC

- **Versão**: 9.3.0
- **Compilação**: gcc -o lab3 lab3.c -lpthread -Wall

Caso prefira, ultilizar o makefile:

```sh
mkdir MarcosLab3
cd MarcosLab3
make
./lab3 <dimensao do vetor> <nº de thread>
```

## Relatório de desempenho

| Dimensão do Vetor | Sequencial |
| ----------------- | ---------- |
| 10^5              | 0.000356   |
| 10^7              | 0.025790   |
| 10^9              | 2.440929   |

| Dimensão do Vetor | Concorrente | Nº de Thread | Desempenho |
| ----------------- | ----------- | ------------ | ---------- |
| 10^5              | 0.000430    | 2            | 0.83       |
| 10^7              | 0.014454    | 2            | 1.78       |
| 10^9              | 1.372909    | 2            | 1.78       |

| Dimensão do Vetor | Concorrente | Nº de Thread | Desempenho |
| ----------------- | ----------- | ------------ | ---------- |
| 10^5              | 0.000421    | 4            | 0.85       |
| 10^7              | 0.010756    | 4            | 2.40       |
| 10^9              | 0.843386    | 4            | 2.89       |

- Os dados foram obtidos ao realizar 10 vezes execuções de cada função e obetendo a média das mesmas;
