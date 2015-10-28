#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>

#define COL_SIZE 3456
#define LIN_SIZE 3456
#define N_ELEMENTS 11943936
#define MAX_THREADS 48
#define HIST_SIZE 256
#define TIME_RESOLUTION 1000000 // time measuring resolution (us)

unsigned  histogram[HIST_SIZE];
float acumulado[HIST_SIZE];

unsigned  initial_image[COL_SIZE][LIN_SIZE] , final_image[COL_SIZE][LIN_SIZE];
double initial_time;
double final_time;
double duration;
timeval t;

void fillMatrices (void) {
  for (unsigned i = 0; i < COL_SIZE; ++i) {
    for (unsigned j = 0; j < LIN_SIZE; ++j) {
      initial_image[i][j] = ((int) rand()) / ((int) RAND_MAX) % 255;
    }
  }
}

void calcula_histograma ( void ){

  for ( unsigned  pos = 0; pos < HIST_SIZE ; pos++ ){
    histogram[pos]=0;
  }

  for (unsigned col=0; col < COL_SIZE; ++col){
    for (unsigned linha=0; linha < LIN_SIZE; ++linha){
      histogram[initial_image[col][linha]]++;
    }
  }
}

void calcula_acumulado ( void ){

  for ( unsigned pos = 0; pos < HIST_SIZE ; pos++ ){
    acumulado[pos] = 0.0f;
  }
  unsigned valor_acumulado = 0;
  for ( unsigned i = 0 ; i < HIST_SIZE ; i++ ){
    valor_acumulado += histogram[i];
    acumulado[i] = valor_acumulado * 255.0f / N_ELEMENTS ;
  }
}

void transforma_imagem( void ){
  for ( unsigned col=0; col < COL_SIZE ; ++col ){
    for ( unsigned linha ; linha < LIN_SIZE; ++linha ){
      final_image[col][linha] = ( unsigned ) ( acumulado[ initial_image[col][linha]] );
    }
  }
}

void writeResults (void) {
  FILE *fp;
  duration = final_time - initial_time;
  fp = fopen ("tempos.dat" , "wa");
  fprintf( fp, "%.10f\n", duration );
  fclose(fp );
}

int main (int argc, char *argv[]) {
  fillMatrices();
  omp_set_num_threads (MAX_THREADS);
  initial_time = omp_get_wtime();
  calcula_histograma();
  calcula_acumulado();
  transforma_imagem();
  final_time = omp_get_wtime();
  writeResults();
  return 1;
}

