#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

#define COL 256
int histogram[256];
int histogram_eq[256];
int acumulado[256];


void calcula_histograma (Mat img){
  int i;
  int row, col;
  for (col=0; col < img.cols; col++){
    row = 0;
    for (row=0; row < img.rows; row++){
      histogram[img.at<uchar>(col,row)]++;
    }
  }

  FILE *fp;
  fp=fopen("histograma.csv","w");
  fprintf(fp, "Cor, Quantidade\n");
  for (i=0;i<COL;i++){
    fprintf(fp,"%d,%d\n",i,histogram[i]);	
  }
  fclose(fp);
}

void calcula_acumulado ( ){
  int i = 0;
  int valor_acumulado = 0;
  for ( i = 0 ; i < COL; i++ ){
    valor_acumulado += histogram[i];
    acumulado[i] = valor_acumulado;
  }

  FILE *fp;
  fp=fopen("histograma_acumulados.csv","w");
  fprintf(fp, "Cor, Quantidade\n");
  int p = 0;  
  for (p=0;p<COL;p++){
    fprintf(fp,"%d,%d\n",p,acumulado[p]);	
  }
  fclose(fp);
}

void transforma_imagem( Mat img , Mat nova_imagem ){
  int n_pixels = img.rows * img.cols;
  int row, col;
  for (col=0; col < img.cols; col++){
    row = 0;
    for (row=0; row < img.rows; row++){
      nova_imagem.at<uchar>(col,row) = ((acumulado[ img.at<uchar>(col,row)]*255))/ n_pixels;
      histogram_eq[nova_imagem.at<uchar>(col,row)]++;
    }
  }
  imwrite ("imagem_equalizada.jpg",nova_imagem);

  FILE *fp;
  fp=fopen("histograma_eq.csv","w");
  fprintf(fp, "Cor, Quantidade\n");
  for (int i=0;i<COL;i++){
    fprintf(fp,"%d,%d\n",i,histogram_eq[i]);	
  }
  fclose(fp);
}

int main (){
  int* hist;
  Mat img = imread("imagem.jpg", 0);
  Mat nova_imagem(img.cols, img.rows, CV_8UC1);
  calcula_histograma(img);
  calcula_acumulado ();
  transforma_imagem (img, nova_imagem);
  return 0;
}
