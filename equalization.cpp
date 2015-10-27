#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

#define COL 256
int histogram[256];
int histogram_eq[256];
float acumulado[256];


void calcula_histograma (Mat img){
  int i;
  int row, col;
  for (col=0; col < img.cols; col++){
    row = 0;
    for (row=0; row < img.rows; row++){
      histogram[img.at<uchar>(col,row)]++;
    }
  }

}

void calcula_acumulado ( int numero_pixels  ){
  int i = 0;
  int valor_acumulado = 0;
  for ( i = 0 ; i < COL; i++ ){
    valor_acumulado += histogram[i];
    acumulado[i] = valor_acumulado * 255 / numero_pixels ;
  }
}

void transforma_imagem( Mat img , Mat nova_imagem ){
  int n_pixels = img.rows * img.cols;
  int row, col;
  for (col=0; col < img.cols; col++){
    row = 0;
    for (row=0; row < img.rows; row++){
      nova_imagem.at<uchar>(col,row) = ( int ) ( acumulado[ img.at<uchar>(col,row)] );
    }
  }
  imwrite ("imagem_equalizada.jpg",nova_imagem);
}

int main (){
  Mat img = imread("imagem.jpg", 0);
  Mat nova_imagem(img.cols, img.rows, CV_8UC1);
  calcula_histograma(img);
  int tamanho = img.cols * img.rows;
  calcula_acumulado ( tamanho );
  transforma_imagem (img, nova_imagem);
  return 0;
}
