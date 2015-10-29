#include <omp.h>
#include <cstdlib>
#include <sys/time.h>
#include <iostream>
#include <fstream>

#define COL_SIZE 3456
#define LIN_SIZE 3456
#define N_ELEMENTS 11943936
#define MAX_THREADS 48
#define HIST_SIZE 256
#define TIME_RESOLUTION 1000000 // time measuring resolution (us)

using namespace std;

// globals
int  histogram[HIST_SIZE];
float acumulado[HIST_SIZE];
timeval t;
int  initial_image[COL_SIZE][LIN_SIZE] , final_image[COL_SIZE][LIN_SIZE];
long long unsigned initial_time, final_time, duration;

void fillMatrices (void) {
	for (unsigned i = 0; i < COL_SIZE; ++i) {
		for (unsigned j = 0; j < LIN_SIZE; ++j) {
			initial_image[i][j] = (((int) rand()) % ((int) 255));
		}
	}
}

void clearCache(){
	double clearcache [30000000];
	for (unsigned i = 0; i < 30000000; ++i)
		clearcache[i] = i;
}

void writeResults (int number_threads) {
	ofstream file ("timing/timings.dat" , ios::out | ios::app );
	file << number_threads << " " << duration << endl;
	file.close();
}

void start (void) {
	gettimeofday(&t, NULL);
	initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

void stop ( void ) {
	gettimeofday(&t, NULL);
	final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
	duration =  final_time - initial_time;
}

void calcula_histograma ( void ){

	for ( unsigned  pos = 0; pos < HIST_SIZE ; pos++ ){
		histogram[pos]=0;
	}
	for (unsigned col=0; col < COL_SIZE; ++col){
		for (unsigned linha=0; linha < LIN_SIZE; ++linha){
			histogram[(initial_image[col][linha])]++;
		}
	}
}

void calcula_acumulado ( void ){

	for ( unsigned pos = 0; pos < HIST_SIZE ; pos++ ){
		acumulado[pos] = 0.0f;
	}
	int valor_acumulado = 0;
	for ( unsigned i = 0 ; i < HIST_SIZE ; i++ ){
		valor_acumulado += histogram[i];
		acumulado[i] = valor_acumulado * 255.0f / N_ELEMENTS ;
	}
}

void transforma_imagem( void ){
	#pragma omp parallel 
	{
	#pragma omp  for schedule (static)
		for ( unsigned col=0; col < COL_SIZE ; ++col ){
	#pragma vector always
	#pragma omp parallel for schedule (static)
			for ( unsigned linha = 0 ; linha < LIN_SIZE; ++linha ){
				final_image[col][linha] = ( int ) ( acumulado[ initial_image[col][linha]] );
			}
		}
	}
}

int main (int argc, char *argv[]) {
	fillMatrices();
	clearCache();
	if ( argc > 1 ){
		int number_threads = atoi(argv[1]);
		if (number_threads > MAX_THREADS ){
			number_threads = MAX_THREADS;
		}
		omp_set_num_threads( number_threads );
		start();
		calcula_histograma();
		calcula_acumulado();		
		transforma_imagem();
#pragma omp single
		stop();
		writeResults(number_threads);
		return 0;
	}
	else {
		return 1;
	}
}

