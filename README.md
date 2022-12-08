# Robô Drumonsters Like

## Contextualização
Considere um robô que se movimenta em uma sala de 300cm x 400cm. Para a análise da
trajetória destes robô, ele será monitorado a partir de três fontes diferentes: uma câmera
superior, um gps e um sensor de odometria embarcados. Estas fontes levam, respectivamente,
um tempo de 100ms, 300ms e 1000ms para capturar a posição do robô. Para o
monitoramento da movimentação do robô, será desenvolvido um sistema que deverá
processar as leituras dos sensores, de forma concorrente, a partir de um buffer de tamanho
finito N (fornecido pelo usuário).

Para cada posição do robô inserida no buffer, o sistema deverá fazer a fusão das informações
das diferentes fontes (considerando a média das leituras atuais), além de calcular a velocidade
e a aceleração atual do robô. Após o processamento dessas informações, elas deverão ser
disponibilizados em uma estrutura de dados compartilhada com a interface gráfica do usuário
(GUI), que será responsável por apresentar as informações do movimento do robô em tempo
real. A interface somente deverá ser atualizada quando ocorrerem alterações no movimento
do robô.

## Descrição da Atividade
Crie um programa em C++, usando threads (pthreads) e semáforos, de forma tal a permitir que
as tarefas de captura, processamento e apresentação do movimento do robô ocorram de
forma concorrente e sincronizada. A posição inicial do robô dentro da sala pode ser gerada de
forma aleatória. Para gerar a trajetória do robô, considere que cada nova posição obtida por
um sensor será igual a leitura anterior acrescida de valores aleatórios entre -10cm e 10cm para
as componentes em x e y.
