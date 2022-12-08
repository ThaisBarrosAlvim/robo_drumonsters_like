# Robô Drumonsters Like

Programa em C++, utilizando threads (pthreads) e semáforos, de uma forma que permita que
as tarefas de captura, processamento e apresentação do movimento do robô ocorram de
forma concorrente e sincronizada. Existem 3 fontes de captura cada uma gera os dados em frequências
diferentes, e algumas coincidem, exigindo fusão de dados em determinados momentos, um buffer de tamanho escolhido pelo
usuário é criado no começo do programa.

![alt text](https://raw.githubusercontent.com/ThaisBarrosAlvim/robo_drumonsters_like/main/contexto.png?token=GHSAT0AAAAAAB4FMGDXP7KFLT3M275V6IGQY4SMOPQ)

**Esboços iniciais por:** Thaís Alvim, Gabriela Amorim e Brenda Gobira.  
**Programado por:** Thaís Alvim.  

## Como Executar
### Compilar:
````shell
g++ -pthread main.cpp -o projeto2SO 
````
### Execução (linux):
````shell
./projeto2SO 
````
### Execução (Windows):
````cmd
start projeto2SO.exe
````
