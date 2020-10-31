#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<fstream>
#include<iostream>
#include<stdlib.h>

using namespace std;
using namespace cv;

void G(Mat Mix,FILE *fout)
{
	//Mat graymix;


	//graymix.create(Mix.size(), Mix.type());
	//cvtColor(Mix, graymix, CV_BGR2GRAY);
	double G1=0,G2=0,G3=0;
	for (int i = 1; i <Mix.rows; i++)
	{
		for (int j = 1; j < Mix.cols; j++)
		{
			G1 += sqrt(pow(Mix.ptr<uchar>(i)[3*j] - Mix.ptr<uchar>(i - 1)[3*j], 2) + pow(Mix.ptr<uchar>(i)[3*j] - Mix.ptr<uchar>(i)[3*(j - 1)], 2));
			G2 += sqrt(pow(Mix.ptr<uchar>(i)[3 * j+1] - Mix.ptr<uchar>(i - 1)[3 * j+1], 2) + pow(Mix.ptr<uchar>(i)[3 * j+1] - Mix.ptr<uchar>(i)[3 * (j - 1)+1], 2));
			G3 += sqrt(pow(Mix.ptr<uchar>(i)[3 * j+2] - Mix.ptr<uchar>(i - 1)[3 * j+2], 2) + pow(Mix.ptr<uchar>(i)[3 * j+2] - Mix.ptr<uchar>(i)[3 * (j - 1)+2], 2));
		}
	}
	fprintf(fout,"\n平均梯度：%lf\n", G1 / ((Mix.rows - 1)*(Mix.cols - 1)));
	fprintf(fout, "平均梯度：%lf\n", G2 / ((Mix.rows - 1)*(Mix.cols - 1)));
	fprintf(fout, "平均梯度：%lf\n", G3 / ((Mix.rows - 1)*(Mix.cols - 1)));
}

void averdif(Mat Mix, Mat spot,FILE *out)
{
	Mat gray;
	cvtColor(Mix, gray, CV_BGR2GRAY);
	double a, b;
	a = b = 0;
	for (int i = 0; i < spot.rows; i++)
	{
		for (int j = 0; j < spot.cols; j++)
		{
			a += Mix.ptr<uchar>(i)[j];
			b += spot.ptr<uchar>(i)[j];
		}
	}
	a /= (Mix.rows*Mix.cols);
	b /= (spot.cols*spot.rows);

	fprintf(out,"与全色图像的均值偏差：%lf\n" , abs(a + b) / b);

	double va, vb;
	va = vb = 0;
	for (int i = 0; i < spot.rows; i++)
	{
		for (int j = 0; j < spot.cols; j++)
		{
			va += pow(Mix.ptr<uchar>(i)[j] - a, 2);
			vb += pow(spot.ptr<uchar>(i)[j] - b, 2);
		}

	}
	va /= (Mix.rows*Mix.cols);
	vb /= (spot.cols*spot.rows);
	fprintf(out, "与全色图像的方差偏差：%lf\n" ,abs(va - vb) / vb);
}



double Getcor(Mat P,Mat XS,double Pv,double XSv)
{
	double fz=0;
	double fmP=0;
	double fmX = 0;
	for (int i = 0; i < P.rows; i++)
	{
		for (int j=0; j < P.cols; j++)
		{
			fz += (P.ptr<uchar>(i)[j] - Pv)*(XS.ptr<uchar>(i)[j] - XSv);
			fmP += pow((P.ptr<uchar>(i)[j] - Pv), 2); fmX += pow((XS.ptr<uchar>(i)[j] - XSv), 2);
		}

	}
	return fz / sqrt(fmP*fmX);
}

void jiaquan(Mat spot, Mat quanse, Mat way1, Mat way2, Mat way3,FILE *fout)
{
	Mat Mix;
	Mix.create(quanse.rows, quanse.cols, CV_8UC3);
	double Pv, XS1v, XS2v, XS3v;
	double r1, r2, r3;//三个相关系数
	Pv = XS1v = XS2v = XS3v = 0;
	for (int i = 0; i < Mix.rows; i++)
	{
		for (int j = 0; j < Mix.cols; j++)
		{
			Pv += double(spot.ptr<uchar>(i)[j]);
			XS1v += double(quanse.ptr<uchar>(i)[3 * j]);
			XS2v += double(quanse.ptr<uchar>(i)[3 * j + 1]);
			XS3v += double(quanse.ptr<uchar>(i)[3 * j + 2]);
			way1.ptr<uchar>(i)[j] = quanse.ptr<uchar>(i)[3 * j];
			way2.ptr<uchar>(i)[j] = quanse.ptr<uchar>(i)[3 * j + 1];
			way3.ptr<uchar>(i)[j] = quanse.ptr<uchar>(i)[3 * j + 2];
		}
	}
	Pv /= (quanse.rows*quanse.cols);
	XS1v /= (quanse.rows*quanse.cols);
	XS2v /= (quanse.rows*quanse.cols);
	XS3v /= (quanse.rows*quanse.cols);


	r1 = Getcor(spot, way1, Pv, XS1v);
	r2 = Getcor(spot, way2, Pv, XS2v);
	r3 = Getcor(spot, way3, Pv, XS3v);
	for (int i = 0; i < spot.rows; i++)
	{
		for (int j = 0; j < spot.cols; j++)
		{
			Mix.ptr<uchar>(i)[3 * j] = round(0.5*((1 + abs(r1))*spot.ptr<uchar>(i)[j] + (1 - abs(r1))*way1.ptr<uchar>(i)[j]) + 0.5);
			Mix.ptr<uchar>(i)[3 * j + 1] = round(0.5*((1 + abs(r2))*spot.ptr<uchar>(i)[j] + (1 - abs(r1))*way2.ptr<uchar>(i)[j]) + 0.5);
			Mix.ptr<uchar>(i)[3 * j + 2] = round(0.5*((1 + abs(r3))*spot.ptr<uchar>(i)[j] + (1 - abs(r1))*way3.ptr<uchar>(i)[j]) + 0.5);
		}
	}
	//cout << spot.rows << " " << spot.cols << " " << quanse.rows << " " << quanse.cols;

	/*cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
	imshow("image", Mix);
	waitKey();*/
	imwrite("加权融合后的影像.bmp", Mix);
	fprintf(fout, "加权融合精度评定：\n");
	G(Mix, fout);
	averdif(Mix, spot, fout);

}

void bizhi(Mat spot, Mat quanse, Mat way1, Mat way2, Mat way3,FILE *fout)
{
	Mat Mix;
	Mix.create(quanse.rows, quanse.cols, CV_8UC3);

	for (int i = 0; i < spot.rows; i++)
	{
		for (int j = 0; j < spot.cols; j++)
		{
			Mix.ptr<uchar>(i)[3 * j] = round(double(way1.ptr<uchar>(i)[j]) / (double(way1.ptr<uchar>(i)[j]) + double(way2.ptr<uchar>(i)[j]) + double(way3.ptr<uchar>(i)[j]))*double(spot.ptr<uchar>(i)[j])+0.5);
			Mix.ptr<uchar>(i)[3 * j + 1] = round(double(way2.ptr<uchar>(i)[j]) / (double(way1.ptr<uchar>(i)[j]) + double(way2.ptr<uchar>(i)[j]) + double(way3.ptr<uchar>(i)[j]))*double(spot.ptr<uchar>(i)[j]) + 0.5);
			Mix.ptr<uchar>(i)[3 * j + 2] = round(double(way3.ptr<uchar>(i)[j]) / (double(way1.ptr<uchar>(i)[j]) + double(way2.ptr<uchar>(i)[j]) + double(way3.ptr<uchar>(i)[j]))*double(spot.ptr<uchar>(i)[j]) + 0.5);
		}
	}
	imwrite("比值变换融合结果.bmp",Mix);
	fprintf(fout, "\n比值变换融合精度评定：\n");
	G(Mix, fout);
	averdif(Mix, spot, fout);
}

void chengji(Mat spot, Mat quanse, Mat way1, Mat way2, Mat way3,FILE *fout)
{
	Mat Mix;
	Mix.create(quanse.rows, quanse.cols, CV_8UC3);
	for (int i = 0; i < spot.rows; i++)
	{
		for (int j = 0; j < spot.cols; j++)
		{
			Mix.ptr<uchar>(i)[3 * j] = spot.ptr<uchar>(i)[j] * way1.ptr<uchar>(i)[j]/256;
			Mix.ptr<uchar>(i)[3 * j + 1] = spot.ptr<uchar>(i)[j] * way2.ptr<uchar>(i)[j]/256;
			Mix.ptr<uchar>(i)[3 * j + 2] = spot.ptr<uchar>(i)[j] * way3.ptr<uchar>(i)[j]/256;
		}
	}
	imwrite("乘积变换融合结果.bmp", Mix);
	fprintf(fout, "\n乘积变换融合精度评定：\n");
	G(Mix, fout);
	averdif(Mix, spot, fout);
}

int main()
{
	//Mat spot = imread("spot.tif");
	//Mat quanse = imread("743.tif");
	//cvNamedWindow("spot", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("quanse", CV_WINDOW_AUTOSIZE);
	//imshow("quanse", quanse);
	//imshow("spot", spot);
	//cout << spot.rows<<" "<<spot.cols<<" "<<quanse.rows<<" "<<quanse.cols;
	//waitKey();
	// imwrite("spot.bmp", spot);
	//imwrite("743.bmp", quanse);
	Mat spot = imread("spot.bmp",0);//单色图像
	Mat quanse = imread("743.bmp");

	Mat way1, way2, way3;
	FILE *fout;
	fout = fopen("精度评定结果.txt", "w");

	//vector<Mat> fenli;
//	split(quanse, fenli);//通道分离
	way1.create(spot.size(), CV_8UC1);
	way2.create(spot.size(), CV_8UC1);
	way3.create(spot.size(), CV_8UC1);
	//手动分离通道
	
	jiaquan(spot, quanse, way1, way2, way3,fout);
	chengji(spot, quanse, way1, way2, way3,fout);
	bizhi(spot, quanse, way1, way2, way3,fout);

	fclose(fout);
	system("pause");

	return 0;
}
