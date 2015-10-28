#include <omp.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/time.h>


#define COL_SIZE 3456
#define LIN_SIZE 3456
#define N_ELEMENTS 11943936
#define MAX_THREADS 1
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

void writeResults (void) {
  ofstream file ("timings.dat");
    file << 0 << " " << duration << endl;
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
    printf("%d\n", (int) acumulado[i]);
  }
}

void transforma_imagem( void ){
  for ( unsigned col=0; col < COL_SIZE ; ++col ){
    for ( unsigned linha ; linha < LIN_SIZE; ++linha ){
      final_image[col][linha] = ( int ) ( acumulado[ initial_image[col][linha]] );
    }
  }
}

int main () {
  fillMatrices();
  clearCache();
  start();
  calcula_histograma();
  calcula_acumulado();
  transforma_imagem();
  stop();
  writeResults();
  return 1;
}

