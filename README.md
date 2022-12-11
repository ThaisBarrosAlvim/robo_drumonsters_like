# Robô Drumonsters Like

Programa em C++, utilizando threads (pthreads) e semáforos, de uma forma que permita que
as tarefas de captura, processamento e apresentação do movimento do robô ocorram de
forma concorrente e sincronizada. Existem 3 fontes de captura cada uma gera os dados em frequências
diferentes, e algumas coincidem, exigindo fusão de dados em determinados momentos, um buffer de tamanho escolhido pelo
usuário é criado no começo do programa.

![diagrama](https://raw.githubusercontent.com/ThaisBarrosAlvim/robo_drumonsters_like/main/contexto.png?token=GHSAT0AAAAAAB4FMGDXJYSIBNZTY6F63YAUY4VGRUA )

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

## Programa em funcionamento
### Rodando Normalmente
Neste modo você deve responder "n" quando perguntado sobre iniciar em modo DEBUG
  
![exemplo_no_debug](https://raw.githubusercontent.com/ThaisBarrosAlvim/robo_drumonsters_like/main/exemplo_no_debug.png?token=GHSAT0AAAAAAB4FMGDXABQXCXKCW5L4XRXWY4VGS6A)

### Rodando em modo Debug
Neste modo você deve responder "s" quando perguntado sobre iniciar em modo DEBUG no começo da execução do programa  
Obs: Cada item impresso do buffer segue o padrão: (posição_x, posição_y) /[tempo em que foi pego] f: delay da fonte em resolução 100ms. Ex: 1=100ms. E -1 é a posição inicial.
  
![exemplo_debug](https://raw.githubusercontent.com/ThaisBarrosAlvim/robo_drumonsters_like/main/exemplo_debug.png?token=GHSAT0AAAAAAB4FMGDXG6IDTZ6G5UVSVCPUY4VGSTA)