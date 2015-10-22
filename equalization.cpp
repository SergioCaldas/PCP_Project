#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

#define COL 256

int *histograma (Mat img){
	int i;
	int hist[COL];
	for (i=0;i<COL;i++){
		hist[i]=0;
	}
	int row, col;
	for (col=0; col < img.cols; col++){
		for (row=0; row < img.rows; row++){
			hist[img.at<uchar>(col,row)]++;
		}
	}
	FILE *fp;
	fp=fopen("histograma.csv","w");
	fprintf(fp, "Cor, Quantidade\n");
	for (i=0;i<COL;i++){
		fprintf(fp,"%d,%d\n",i,hist[i]);	
	}
	fclose(fp);
	return hist;
}

int main (){
	Mat img = imread("imagem.jpg", 0);
	histograma(img);
	return 1;
}