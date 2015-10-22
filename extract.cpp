#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

int main (){
	Mat img = imread("imagem.jpg", 0);
	FILE *fp;
	fp = fopen("matriz.txt","w");
	int row, col;
	for (col=0; col < img.cols; col++){
		for (row=0; row < img.rows; row++){
			fprintf(fp,"%d ",img.at<uchar>(col,row));
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	return 1;
}