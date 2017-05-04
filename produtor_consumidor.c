#include <stdio.h> 
#include <pthread.h> 


pthread_mutex_t count_mutex1 = PTHREAD_MUTEX_INITIALIZER;	
pthread_mutex_t count_mutex2 = PTHREAD_MUTEX_INITIALIZER;	
pthread_cond_t buffer_cheio = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_vazio = PTHREAD_COND_INITIALIZER;


#define TOTAL 50
int posicoes_ocupadas=0;

int valor=0;

int buffer[TOTAL];

void *produtor(void *argumentos){

	while (1){
		if (posicoes_ocupadas==TOTAL){
			printf("!!! Produtor Esperando   !!!\n");
			pthread_cond_wait(&buffer_cheio, &count_mutex1);
		}
		buffer[posicoes_ocupadas] = valor;
		printf("Valor produzido: %d\n", buffer[posicoes_ocupadas]);
		valor++;
		posicoes_ocupadas++;
		if (posicoes_ocupadas==1){
			pthread_cond_signal(&buffer_vazio);
		}
	}

	pthread_exit(NULL);
}

void *consumidor(void *argumentos){
	int leitura;

	while(1){
		if (posicoes_ocupadas==0){
			printf("!!!  Consumidor Esperando  !!!\n");
			pthread_cond_wait(&buffer_vazio,&count_mutex2);
		}
		leitura = buffer[posicoes_ocupadas];
		printf("Valor lido: %d\n", leitura);
		posicoes_ocupadas--;
		if (posicoes_ocupadas== TOTAL-1){
			pthread_cond_signal(&buffer_cheio);
		}
	}


	pthread_exit(NULL);
}

int main (){
pthread_t thread_produtor;
pthread_t thread_consumidor;

// Criando thread do produtor
	pthread_create(&thread_produtor, NULL, produtor, NULL);
// Criando thread do consumidor
	pthread_create(&thread_consumidor, NULL, consumidor, NULL);

	pthread_exit(NULL);
	return 0; /* O programa não vai chegar aqui. */
}