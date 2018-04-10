#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;

#define d 5 // Para o problema, temos 5 dimensões
#define n 10 // Para o problema, está sendo considerada uma nuvem com 10 partículas
#define PI 3.14159265
#define c1 0.8 // Define o coeficiente individual em 0.8
#define c2 1.3 // Define o coeficiente social em 1.3
#define w 0.8 // Define o fator de inércia em 0.8

struct Particula
{
    float p[d]; // Vetor que armazena a posição atual da partícula
    float pbest[d]; // Vetor que armazena a melhor posição da partícula
    float v[d]; // Vetor que armazena a velocidade da partícula
    float fitness; // A avaliação de cada partida, que é baseado na posição atual da mesma
};

void inicializa_particulas(float p[], float pbest[], float v[]); // Inicializa os vetores de posicao, pbest e velocidade das particulas iniciais
float calcula_fitness(float p[]); // Calcula o fitness de cada partícula com base na função utilizada na prova
void calcula_velocidade(float p[], float pbest[], float v[], float gbest[]); // Calcula a nova velocidade de cada partícula utilizando a fórmula "padrão" do PSO
void atualiza_posicao(float p[], float v[]); // Atualiza a posição de cada partícula com base na posição antiga e na nova velocidade
void imprime_resultados(float p[], float pbest[], float v[], float gbest[], float fitness, int k); // Imprime os parâmetros de cada partícula


int main ()
{
    clock_t begin = clock();
    srand(time(0));
    int indice;
    float trono, gbest[d];
    double elapsed_secs;
    bool solucao = false;
    Particula pt[n]; // Define a nuvem como um conjunto de n partículas

    /// INICIALIZANDO AS PARTÍCULAS
    for(int i=0; i<n; i++)
    {
        inicializa_particulas(pt[i].p, pt[i].pbest, pt[i].v);
        pt[i].fitness = calcula_fitness(pt[i].p);
    }

    /// INICIALIZANDO O GBEST
    for(int i=0; i<d; i++)  // Inicializa gbest com o pbest da primeira particula
    {
        gbest[i] = pt[0].pbest[i];
    }

    /// COMEÇANDO AS ITERAÇÕES
    while(solucao==false)  // O critério de solução adotado neste caso é por tempo (4 segundos)
    {
        for(int i=0; i<n; i++)
        {
            calcula_velocidade(pt[i].p, pt[i].pbest, pt[i].v, gbest); // Calcula a nova velocidade de cada particula
            atualiza_posicao(pt[i].p, pt[i].v); // Atualiza a posicao de cada particula com base em sua nova velocidade
            pt[i].fitness = calcula_fitness(pt[i].p); // Atualiza a avaliação de cada partícula

            /// SUBSTITUINDO, OU NÃO, O PBEST DE CADA PARTÍCULA
            if(pt[i].fitness < calcula_fitness(pt[i].pbest))  // Se o fitness da posicao atual da particula for melhor que o fitness da posicao pbest dela
            {
                for(int j=0; j<d; j++)  // Atualize o pbest
                {
                    pt[i].pbest[j] = pt[i].p[j];
                }
            }
        }

        /// DESCOBRINDO QUAL EH O MELHOR PBEST
        trono = calcula_fitness(pt[0].pbest);
        indice = 0;
        for(int i=1; i<n; i++)
        {
            if(calcula_fitness(pt[i].pbest) < trono)
            {
                trono = calcula_fitness(pt[i].pbest);
                indice = i;
            }
        }

        /// SUBSTITUINDO, OU NÃO, O GBEST
        if( calcula_fitness(pt[indice].pbest) < calcula_fitness(gbest) )  // Se o fitness do melhor pbest for melhor que o fitness do gbest
        {
            for(int i=0; i<d; i++)  // gbest recebe o melhor pbest
            {
                gbest[i] = pt[indice].pbest[i];
            }
        }

        for(int i=0; i<n; i++)
        {
            imprime_resultados(pt[i].p, pt[i].pbest, pt[i].v, gbest, pt[i].fitness, i+1);
            cout << endl << endl;
        }
        clock_t end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        if(elapsed_secs > 4)
        {
            cout << endl << "MINIMO DA FUNCAO (APROXIMADO) : " << calcula_fitness(gbest) << endl << endl << endl;
            solucao = true;
        }
    }
}

void inicializa_particulas(float p[], float pbest[], float v[])
{
    for(int i=0; i<d; i++)
    {
        p[i] = (rand() % 200 -100)/10.0; // Gera um numero aleatorio entre -100 e 100
        pbest[i] = (rand() % 200 -100)/10.0; // Gera um numero aleatorio entre -100 e 100
        v[i] = (rand() % 200 -100)/10.0; // Gera um numero aleatorio entre -10 e 10
    }
}

float calcula_fitness(float p[])
{
    float soma=0, resultado;
    for(int i=0; i<d; i++)
    {
        soma = soma + (p[i]*p[i] - 10.0*cos(2*PI*p[i]));
    }
    resultado = 50 + soma;
    return resultado;
}

void calcula_velocidade(float p[], float pbest[], float v[], float gbest[])
{
    float p1, p2;
    p1 = (rand()%100)/100.0;
    p2 = (rand()%100)/100.0;
    for(int i=0; i<d; i++)
    {
        v[i] = w*v[i] + p1*c1*(pbest[i]-p[i]) + p2*c2*(gbest[i]-p[i]);
    }
}

void atualiza_posicao(float p[], float v[])
{
    for(int i=0; i<d; i++)
    {
        p[i] = p[i] + v[i];
    }
}

void imprime_resultados(float p[], float pbest[], float v[], float gbest[], float fitness, int k)
{
    cout << "PARTICULA " << k;
    cout << endl << "POSICAO ATUAL: ";
    for(int j=0; j<d; j++)
    {
        cout << p[j] << " ";
    }
    cout << endl << "MELHOR POSICAO: ";
    for(int j=0; j<d; j++)
    {
        cout << pbest[j] << " ";
    }
    cout << endl << "VELOCIDADE ATUAL: ";
    for(int j=0; j<d; j++)
    {
        cout << v[j] << " ";
    }
    cout << endl << "FITNESS: " << fitness;
    cout << endl << "FITNESS DO PBEST: " << calcula_fitness(pbest);
}
