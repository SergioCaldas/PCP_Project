#include <omp.h>
#include <cstdlib>
#include <sys/time.h>
#include <iostream>
#include <fstream>

#define MAX_THREADS 48
#define HIST_SIZE 256
#define TIME_RESOLUTION 1000000 // time measuring resolution (us)

using namespace std;

// globals
int  histogram[HIST_SIZE];
float acumulado[HIST_SIZE];
timeval t;
long long int * initial_image, * final_image;
long long unsigned initial_time, final_time, hist_time, accum_time, transform_time, temporary_time;
long long unsigned hist_duration, accum_duration, transform_duration, total_duration;

void fillMatrices ( long long int total_pixels  ) {

	initial_image = (long long int*) malloc(total_pixels * sizeof ( long long int ) );
	final_image = (long long int*) malloc(total_pixels * sizeof ( long long int ) );

	for (long long int pixel_number = 0; pixel_number < total_pixels; ++pixel_number) {
		initial_image[pixel_number] = (((int) rand()) % ((int) 255));
	}

	for ( unsigned  pos = 0; pos < HIST_SIZE ; pos++ ){
		histogram[pos]=0;
		acumulado[pos] = 0.0f;
	}
}

void clearCache(){
	double clearcache [30000000];
	for (unsigned i = 0; i < 30000000; ++i)
		clearcache[i] = i;
}

void writeResults (int number_threads) {
        ofstream file ("timing/timings.dat" , ios::out | ios::app );
        file << number_threads << " , " << hist_duration << " , " << accum_duration << " , "<< transform_duration << " , " << total_duration << endl;
        file.close();
}

void start (void) {
        gettimeofday(&t, NULL);
        initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

void mark_time ( int break_num ) {
        gettimeofday(&t, NULL);
        temporary_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
        if ( break_num == 1 ){
                hist_time = temporary_time;
        } else if ( break_num == 2 ){
                accum_time = temporary_time;
        }
        else if ( break_num == 3 ){
                transform_time = temporary_time;
        }
}

void stop ( void ) {
        gettimeofday(&t, NULL);
        final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
        hist_duration = hist_time - initial_time;
        accum_duration = accum_time - hist_time;
        transform_duration = transform_time - accum_time;
        total_duration =  final_time - initial_time;
}

void calcula_histograma ( long long int total_pixels  ){
	for (long long int pixel_number = 0; pixel_number < total_pixels; ++pixel_number) {
		histogram[ initial_image[pixel_number] ]++;
	} 
}

void calcula_acumulado ( long long int total_pixels  ){
	int valor_acumulado = 0;
	for ( unsigned i = 0 ; i < HIST_SIZE ; i++ ){
		valor_acumulado += histogram[i];
		acumulado[i] = valor_acumulado * 255.0f / total_pixels ;
	}
}

void transforma_imagem( long long int total_pixels , int thread_count  ){
#pragma omp parallel num_threads( thread_count ) 
#pragma omp for nowait 
	for (long long int pixel_number = 0; pixel_number < total_pixels; ++pixel_number) {
		final_image[pixel_number] = (int )( acumulado[ initial_image[pixel_number]] );
	} 
}

int main (int argc, char *argv[]) {
	if ( argc > 3 ){
		int number_threads = atoi(argv[1]);
		int rows = atoi(argv[2]);
		int columns = atoi(argv[3]);
		long long int total_pixels = rows * columns;
		fillMatrices(total_pixels);
		clearCache();
		if (number_threads > MAX_THREADS ){
			number_threads = MAX_THREADS;
		}
		start();
		calcula_histograma( total_pixels );
		mark_time(1);
		calcula_acumulado( total_pixels );
		mark_time(2);
		transforma_imagem( total_pixels , number_threads );
		mark_time(3);
		stop();
		writeResults(number_threads);
		return 0;
	}
	else {
		return 1;
	}
}

