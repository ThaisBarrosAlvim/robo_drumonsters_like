#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <queue>
#include <cmath>
#include <sys/time.h>
#include "Pos.h"

using namespace std;

// Mudanças de sistemas
#ifdef __linux__
void clear_terminal(){
    system("clear");
}
void pause_run(){
}
#elif _WIN32
void clear_terminal(){
    system("cls");
}
void pause_run(){
    system("pause");
}

#endif


//variaveis globais
int N=0;
int TIME_MAX = 0;
bool DEBUG = false;

//tamanho sala
const int room_x = 300;
const int room_y = 400;

//memoria compartilhada
int pos_x = 0;
int pos_y = 0;
unsigned long int bfi = 0;
queue<Pos> buffer;

//declaração mutex
sem_t mutex;

// Permite printarmos Pos utilizando cout << Pos();
std::ostream &operator<<(std::ostream &os, Pos const &m) {
    if (DEBUG)
        return os << "(" << m.x << ", " << m.y << ") [" << m.time << "]" << " f:" << m.f;
    else
        return os << "(" << m.x << ", " << m.y << ")";
}

void imprime_buffer(queue<Pos> gq) {
    usleep(10000);
    cout << "Buffer: [" << gq.size() << "]: ";
    queue<Pos> g = gq;
    while (!gq.empty()) {
        cout << gq.front() << ", ";
        g.push(gq.front());
        gq.pop();
    }
    cout << '\n';
    gq = g;
}

// Retorna valor entre -10 e 10
inline int rand_pos() {
    return (rand() % 21) - 10;
}

// Thread que controla o tempo
void *tempo(void *arg) {
    struct timeval stop{}, start{};
    gettimeofday(&start, nullptr);
    for (int i = 0; i < TIME_MAX*10; ++i) {
        // delay na resolução de 100ms
        usleep(1000 * 100);
        // adiciona +1 no indice do buffer
        bfi++;
    }
    gettimeofday(&stop, nullptr);
    printf("Rotina gastou %lu ms\n", ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)/1000);
    pause_run();
    exit(1);
}

//funcao que vai executar em paralelo
void *fonte(void *arg) {
    int fontid = ((size_t) arg);
    for (int i = 0; i < TIME_MAX*10; ++i) {
        // Trava a execução se o buffer tiver cheio
        while(buffer.size() >= N);

        //entrar na zona crítica
        sem_wait(&mutex);

        //adiciona no pos x
        int new_x = pos_x + rand_pos();
        if (new_x >= 0 && new_x <= room_x) //validacao do range(0 <= novo_valor <= tamanho_sala)
            pos_x = new_x;

        //adiciona no pos y
        int new_y = pos_y + rand_pos();
        if (new_y >= 0 && new_y <= room_y) //validacao do range(0 <= novo_valor <= tamanho_sala)
            pos_y = new_y;


        // aloca o espaco e adiciona
        if (buffer.size() < N) {
            if (buffer.back().time == bfi) {
                if (DEBUG){
                    usleep(1000);
                    cout << "Media entre " << buffer.back() << " e " << Pos(pos_x, pos_y, bfi, fontid) << endl;
                }
                buffer.back().x = (buffer.back().x + pos_x) / 2;
                buffer.back().y = (buffer.back().y + pos_y) / 2;
            }
                // faz a media do existente com o que vai ser adicionado
            else {
                buffer.push(Pos(pos_x, pos_y, bfi, fontid));
            }
        } else {
            if (DEBUG)
                cout << "Encheu o buffer!\n";
        }

        //sair da zona crítica
        sem_post(&mutex);


        //delay especifico da fonte
        int old_bfi = bfi;
        while (true) {
            if (bfi != 0 && bfi != old_bfi && bfi % fontid == 0 && buffer.size() < N)
                break;
        }
    }
    return nullptr;
}

double calc_velocidade(Pos pos1, Pos pos2) {
    int xd = pos2.x - pos1.x, yd = pos2.y - pos1.y;
    return sqrt(pow(xd, 2) + pow(yd, 2))/100;
}

double calc_aceleracao(double v1, double v2) {
    return (v2-v1)/100;
}


// Rotina de processamento
[[noreturn]] void *processamento(void *arg) {
    Pos data, data_last;
    size_t pi = 0; // contador de processamento
    double velocidade_atual, velocidade_passada;
    while (true) {
        //entrar na zona crítica
        sem_wait(&mutex);
        if (DEBUG){
            imprime_buffer(buffer);
        }
        // Remove posição do buffer
        data = buffer.front();
        buffer.pop();

        //sair da zona crítica
        sem_post(&mutex);

        // Limpa a tela
        if (!DEBUG)
            clear_terminal();

        // Printa tempo, posição
        cout << "Tempo: " << bfi*100 << " ms" << endl;
        cout << "Posição: " << data << endl;

        // Printa velocidadee
        if (pi == 0) {
            cout << "Velocidade: Aguardando mais dados..." << endl;
            data_last = data;
        } else {
            velocidade_atual = calc_velocidade(data_last, data);
            printf("Velocidade: %.2f cm/s\n", velocidade_atual*1000);
            data_last = data;
        }

        // Printa aceleração
        if (pi < 2){
            cout << "Aceleração: Aguardando mais dados..." << endl;
            velocidade_passada = velocidade_atual;
        }
        else{
            printf("Aceleração: %.2f cm/s^2\n", calc_aceleracao(velocidade_atual, velocidade_passada)*1000);
            velocidade_passada = velocidade_atual;
        }
        cout << endl;

        // Delay de processamento, só acontece se o buffer tiver com menos de 80% de sua capacidade
        if (buffer.size() < N-lround(N/10)*2)
            usleep(1000*105);

        pi++;

        // Travamento caso o buffer esteja vazio
        while (buffer.empty());

    }
}

void start_routine() {
    // Inicia as posições com valores aleatorios
    pos_x = rand() % room_x;
    pos_y = rand() % room_y;

    buffer.push(Pos(pos_x, pos_y, bfi));
    bfi++;
}

int main() {
    // Inicia semente de randomizacao
    srand(time(nullptr));


    //  Pergunta o tamanho do buffer
    while(N <= 0){
        cout << "Digite um valor para o tamanho do buffer: ";
        cin >> N;
        if (N <= 0) {
            cout << "Valor invalido, deve ser maior que 0!\n";
        } else {
            break;
        }
    }

    //  Pergunta tempo de execução
    while(TIME_MAX <= 0){
        cout << "Digite um valor para o tempo de execução da rotina em segundos: ";
        cin >> TIME_MAX;
        if (TIME_MAX <= 0) {
            cout << "Valor invalido, deve ser maior que 0!\n";
        } else {
            break;
        }
    }

    //  Pergunta se quer iniciar em modo debug
    char res_debug = 'z';
    while(res_debug != 's' and res_debug != 'n'){
        cout << "Quer iniciar em modo DEBUG ? [s/n] ";
        cin >> res_debug;
        if (res_debug != 's' and res_debug != 'n') {
            cout << "Resposta invalida, deve ser 's' ou 'n'!\n";
        } else {
            DEBUG = res_debug == 's';
            break;
        }
    }
    cout << endl << endl;

    // Inicia os valores
    start_routine();

    // Inicia os identificadores de threads
    pthread_t f1, f2, f3, t, p;
    //iniciando main
    sem_init(&mutex, 0, 1);

    //Inicia Threads:
    //iniciando gerenciador de tempo, em resolução de 100ms
    pthread_create(&t, nullptr, tempo, nullptr);
    //iniciando rotina de processamento
    pthread_create(&p, nullptr, processamento, nullptr);
    //iniciando Fontes 1,2,3
    pthread_create(&f1, nullptr, fonte, (void *) 1); // 100ms
    pthread_create(&f2, nullptr, fonte, (void *) 3); //300ms
    pthread_create(&f3, nullptr, fonte, (void *) 10);//1000ms

    //Espera Threads
    pthread_join(t, nullptr);
    pthread_join(p, nullptr);
    pthread_join(f1, nullptr);
    pthread_join(f2, nullptr);
    pthread_join(f3, nullptr);

    return 0;
}