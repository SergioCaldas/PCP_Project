#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

#define COL 256
unsigned int histogram[COL];
float acumulado[COL];

void calcula_histograma (Mat img){

  for ( int pos = 0; pos < COL ; pos++ ){
    histogram[pos]=0;
  }

  for (int col=0; col < img.cols; col++){
    for (int row=0; row < img.rows; row++){
      histogram[img.at<uchar>(row,col)]++;
    }
  }
}

void calcula_acumulado ( int numero_pixels  ){

  unsigned int valor_acumulado = 0;
  for ( int pos = 0; pos < COL ; pos++ ){
    acumulado[pos] = 0.0f;
  }

  for ( int i = 0 ; i < COL; i++ ){
    valor_acumulado += histogram[i];
    acumulado[i] = valor_acumulado * 255.0f / numero_pixels ;
  }
}

void transforma_imagem( Mat img , Mat nova_imagem ){
  int n_pixels = img.rows * img.cols;

  for (int col=0; col < img.cols; col++){
    for (int row=0; row < img.rows; row++){
      nova_imagem.at<uchar>(row,col) = ( int ) ( acumulado[ img.at<uchar>(row,col)] );
    }
  }
  imwrite ("imagem_equalizada.jpg",nova_imagem);
}

int main (){
  Mat img = imread("imagem.jpg", 0);
  Mat nova_imagem(img.rows, img.cols, CV_8UC1);
  calcula_histograma(img);
  int tamanho = img.cols * img.rows;
  calcula_acumulado ( tamanho );
  transforma_imagem (img, nova_imagem);
  return 0;
}
