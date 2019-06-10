/*
 * Me baseando no problema do barbeiro ardomecido, eu fiz esse do ESTACIONAMENTO.
 * Aqui se não tem carros, o estacionamento está "desligado"
 * Quando chegar carro, ele "Liga".
 * Aqui foi feito o uso de semáforo e threads.
 * Fontes: http://www.ic.unicamp.br/~islene/mc514/barbeiro/barbeiro.pdf
 *
*/
#include <pthread.h> // Bliblioteca de threads
#include <semaphore.h> // Bliblioteca do semáforo
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N_Carros 10 // Número de carros, cada carro vai ser uma thread
#define N_Vagas 5 // Número de vagas para os carros.
/*
 * sem_t significa que estamos criando os semáforos
*/
sem_t vaga;
sem_t vaga_estacionamento;
sem_t sem_vaga_estacionamento;
sem_t sem_vaga_carro;
/*
 *Declaração das funções
*/
void* carro(void* arg);
void* estacionamento(void* arg);

int main() {
  pthread_t thr_carro[N_Carros], thr_estacionamento; // Declarando as threads
  int i, id[N_Carros]; // o contador e a identificação dos carros

/*
 * sem_init significa que está inicializando o semáforo
*/
  sem_init(&vaga_estacionamento, 0, 5);
  sem_init(&vaga, 0, 1);
  sem_init(&sem_vaga_carro, 0, 0);
  sem_init(&sem_vaga_estacionamento, 0, 0);
/*
 * Cria threads
*/
  for (i = 0; i < N_Carros; i++) {
    id[i] = i;
    pthread_create(&thr_carro[i], NULL, carro, (void*) &id[i]);
  }

    pthread_create(&thr_estacionamento, NULL, estacionamento, NULL);
  
  for (i = 0; i < N_Carros; i++) 
    pthread_join(thr_carro[i], NULL);

  return 0;
}
/*
 * Função que indica as funções do estacionamento
 * Aqui começa sem carros para prenncher as vagas, logo, quando chegar um carro ou mais ele vai liberando.
*/
void * estacionamento(void *arg) {

  while(1) {
    sem_wait(&sem_vaga_carro); // Espera de vagas para o carro, ou seja, não liberada a entrada. 
    printf("Estacionamento liberou entrada do carro.\n");
    sem_post(&sem_vaga_estacionamento); // Libera a entrada de carros.
  }
  return NULL;
}
/*
 * Função que indica as função do carro.
 * O sem_wait() Bloqueia, ou seja, deixa em espera.
 * O sem_post() Libera, ou seja, da continuidade ao programa.
 * Aqui o carro entra e "espera" para poder estacionar, ou seja achar a vaga e parar nela.
 * Quando um ou mais carros saem, o estacionamento libera entrada.
*/
void * carro(void* arg) {
  int id = *(int*) arg;

  sleep(id%5);
  if (sem_trywait(&vaga_estacionamento) == 0) {

    printf("Carro %d entrou no Estacionamento.\n", id);
    sem_wait(&vaga);
    printf("Carro %d entrou na vaga disponível.\n", id);
    sem_post(&vaga_estacionamento);
    sem_post(&vaga);
    sem_post(&sem_vaga_carro);
    sem_post(&sem_vaga_estacionamento);
    printf("Carro %d deixou o Estacionamento.\n", id);
} else {
    printf("Carro %d não entrou no estacionamento.\n", id);
  } 
  return NULL;
}