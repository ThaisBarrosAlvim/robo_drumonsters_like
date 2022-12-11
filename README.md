# Robô Drumonsters Like

Programa em C++, utilizando threads (pthreads) e semáforos, de uma forma que permita que
as tarefas de captura, processamento e apresentação do movimento do robô ocorram de
forma concorrente e sincronizada. Existem 3 fontes de captura cada uma gera os dados em frequências
diferentes, e algumas coincidem, exigindo fusão de dados em determinados momentos, um buffer de tamanho escolhido pelo
usuário é criado no começo do programa.

![diagrama](https://raw.githubusercontent.com/ThaisBarrosAlvim/robo_drumonsters_like/main/imagens/diagrama.png?token=GHSAT0AAAAAAB4FMGDXSHMXUIGJBO6DGP7UY4VGWJQ)

**Analise do Problema:** Thaís Alvim, Gabriela Amorim e Brenda Gobira.  
**Programado por:** Thaís Alvim.  

## Como Executar
### Compilar:
````shell
g++ -pthread main.cpp -o auto_robo
````
### Execução (linux):
````shell
./auto_robo 
````
### Execução (Windows):
````cmd
start auto_robo.exe
````

## Programa em funcionamento
### Rodando Normalmente
Neste modo você deve responder "n" quando perguntado sobre iniciar em modo DEBUG
  
![exemplo_no_debug](https://raw.githubusercontent.com/ThaisBarrosAlvim/robo_drumonsters_like/main/imagens/exemplo_no_debug.png?token=GHSAT0AAAAAAB4FMGDWHEC5TF67EZWU5CCMY4VGWWA)

### Rodando em modo Debug
Neste modo você deve responder "s" quando perguntado sobre iniciar em modo DEBUG no começo da execução do programa  
Obs: Cada item impresso do buffer segue o padrão: (posição_x, posição_y) [tempo em que foi pego] f: delay da fonte em resolução 100ms. Ex: 1=100ms. E -1 é a posição inicial.
  
![exemplo_debug](https://raw.githubusercontent.com/ThaisBarrosAlvim/robo_drumonsters_like/main/imagens/exemplo_debug.png?token=GHSAT0AAAAAAB4FMGDXL6OXZFOKSWCCPOQSY4VGWOQ)
