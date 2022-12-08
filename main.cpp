#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <sys/time.h>

using namespace std;


class Pos {
public:
    int x{}, y{}, time{}, f{};

    Pos();

    Pos(int x, int y, int time, int f);

    Pos &operator=(Pos);
};

Pos::Pos(int x, int y, int time, int f = -1) {
    this->x = x;
    this->y = y;
    this->time = time;
    this->f = f;
}


Pos::Pos() {
    x = -1;
    y = -1;
    time = -1;
    f = -1;
}

std::ostream &operator<<(std::ostream &os, Pos const &m) {
    return os << "(" << m.x << ", " << m.y << ") " ;
//    return os << "(" << m.x << ", " << m.y << ") [" << m.time << "]" << " f:" << m.f << ", ";
}

Pos &Pos::operator=(Pos outra) {
    this->x = outra.x;
    this->y = outra.y;
    this->time = outra.time;
    this->f = outra.f;
    return *this;
}


#ifdef __linux__
void clear_terminal(){
    system("clear");
}
#elif _WIN32
void clear_terminal(){
    system("cls");
}

#endif



int N=0;
int TIME_MAX = 100;
//tamanho sala
const int room_x = 300;
const int room_y = 400;

//memoria compartilhada
int pos_x = 0;
int pos_y = 0;
int bfi = 0;
queue<Pos> buffer;

//declaração mutex
sem_t mutex;


void imprime_buffer(queue<Pos> gq) {
//    usleep(10000);
//    cout << "Buffer: [" << gq.size() << "]: ";
    queue<Pos> g = gq;
    while (!gq.empty()) {
        cout << gq.front();
        g.push(gq.front());
        gq.pop();
    }
    cout << '\n';
    gq = g;
}


inline int rand_pos() {
    return (rand() % 21) - 10;
}

void *tempo(void *arg) {
    struct timeval stop{}, start{};
    gettimeofday(&start, nullptr);
    for (int i = 0; i < TIME_MAX; ++i) {
        // delay na resolução de 100ms
        usleep(1000 * 100);
        // adiciona +1 no indice do buffer
        bfi++;
    }
    gettimeofday(&stop, nullptr);
    printf("Rotina gastou %lu ms\n", ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)/1000);
    exit(1);
}



//funcao que vai executar em paralelo
void *fonte(void *arg) {
    int fontid = ((size_t) arg);
    for (int i = 0; i < TIME_MAX; ++i) {
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

            if (buffer.front().time == bfi) {
                //  cout << "Faz media!" << endl;
                buffer.front().x = (buffer.front().x + pos_x) / 2;
                buffer.front().y = (buffer.front().y + pos_y) / 2;
            }
                // faz a media do existente com o que vai ser adicionado
            else {
                buffer.push(Pos(pos_x, pos_y, bfi, fontid));
            }
        } else {
//            cout << "Encheu o buffer!\n";
        }


//        imprime_buffer(buffer);

        //sair da zona crítica
        sem_post(&mutex);


        //delay especifico da fonte
        int old_bfi = bfi;
//        cout << "T[" << bfi << "] -- Fonte(" << fontid << "): " << "Trava " << endl;
        while (true) {
            if (bfi != 0 && bfi != old_bfi && bfi % fontid == 0 && buffer.size() < N)
                break;
        }
//        cout << endl << "T[" << bfi << "] -- Fonte(" << fontid << "): " << "Libera " << endl;
    }
}

double calc_velocidade(Pos pos1, Pos pos2) {
    int xd = pos2.x - pos1.x, yd = pos2.y - pos1.y;
    return sqrt(pow(xd, 2) + pow(yd, 2))/100;
}

double calc_aceleracao(double v1, double v2) {
    return (v2-v1)/100;
}

void clean_unordered_map(unordered_map<int, Pos> &map, int start, int end){
//    cout << "Limpa o mapa: " << start << " a "<< end << " size: " << map.size() <<endl;
    for (int i = start; i < end; ++i) {
        map.erase(i);
    }
}


[[noreturn]] void *processamento(void *arg) {
    Pos data, data_last;
    unordered_map<int, Pos> posicoes;
    int pi = 0, clean_time=0;
    double velocidade_atual, velocidade_passada;
    while (true) {
        //entrar na zona crítica
        sem_wait(&mutex);
//        cout << "\nT[" << bfi << "] -- Processamento Libera\n";
        imprime_buffer(buffer);

        data = buffer.front();
        buffer.pop();

        //sair da zona crítica
        sem_post(&mutex);

        // Faz a media das posições iguais
        if (posicoes.count(data.time)) {
//            usleep(1000);
//            cout << "Faz media! (procs) time: " << data.time << endl;
            posicoes[data.time].x = (posicoes[data.time].x + data.x) / 2;
            posicoes[data.time].y = (posicoes[data.time].y + data.y) / 2;
            data = posicoes[data.time]; // atualiza data para o valor da media
        }
        else{
            posicoes[data.time] = data;
        }

        // Printa posição
        clear_terminal();
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

        pi++;
        // Limpeza automatica das posições que ficam "cacheadas" para que sejam feitas as medias
        if (posicoes.size() > 60 && bfi%30==0){
            clean_unordered_map(posicoes, clean_time, bfi-30);
            clean_time += 30;
        }
        // Travamento caso o buffer esteja pequeno
        while (buffer.size() != N);

    }
}

void start_routine() {
    // Inicia as posições com valores aleatorios
    pos_x = rand() % room_x;
    pos_y = rand() % room_y;
//    cout << "Iniciou na posição: (" << pos_x << ", " << pos_y << ")" << endl;

    buffer.push(Pos(pos_x, pos_y, bfi));
    bfi++;
    imprime_buffer(buffer);
}

int main(int argc, char *argv[]) {
    // Inicia semente de randomizacao
    srand(time(nullptr));

    //Pergunta o tamanho do buffer
//    while(N <= 0){
//        cout << "Digite um valor para o tamanho do buffer: ";
//        cin >> N;
//        if (N <= 0) {
//            cout << "Valor invalido, deve ser maior que 0!\n";
//        } else {
//            break;
//        }
//    }
    N = 10;

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

//    cout << "post_x: " << pos_x << endl;
//    cout << "post_y: " << pos_y << endl;
//    cout << "buffer: " << endl;
    imprime_buffer(buffer);
    return 0;
}