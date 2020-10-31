#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include<string>
#include"Matrix.h"
#include<stdlib.h>

using namespace std;
using namespace cv;

int main()
{
	String filename = "point.txt";
	string a, b;

	ifstream fin(filename.c_str());
	filename = "参数结果及精度评定.out";
	ofstream fout(filename.c_str());
	double	A[5][3];
	double B[5][3];
	double Params[6];
	double LA[5];
	double LB[5];
	double det[2][2];
	//double params[6];//六个参数
	double p1[5][2], p2[5][2];
	
	int i, j, k;
	for (i = 0; i < 5; i++)
	{
		fin >> a >> b >> p1[i][0] >> p1[i][1] >> p2[i][0] >> p2[i][1];
		A[ i][0] = 1; A[ i][1] = p2[i][0]; A[i][2] = p2[i][1];
		B[i][0] = 1; B[i][1] = p2[i][0]; B[i][2] = p2[i][1];
		LA[i] = p1[i][0];
		LB[i] = p1[i][1];
	}
	Matrix MA(5, 3);
	Matrix ML(5,1);
	Matrix inv(3, 3);
	Matrix ATL(3, 1);
	Matrix params(3, 1);
	Matrix v(5, 1);
	Matrix zhongwucha(1, 1);
	Matrix checkpoint(2, 4);

	fin >> a >> b >> checkpoint.at(0, 0) >> checkpoint.at(0, 1) >> checkpoint.at(0, 2) >> checkpoint.at(0, 3);
	fin >> a >> b >> checkpoint.at(1, 0) >> checkpoint.at(1, 1) >> checkpoint.at(1, 2) >> checkpoint.at(1, 3);
	
	
	for ( i = 0; i < 5; i++)
	{
		ML.at(i,0) = LA[i];
		for (j  = 0; j < 3; j++)
		{
			MA.at(i, j) = A[i][j];
		}
	}
	inv = MA.t()*MA;
	//cout << inv<<MA;
	ATL = MA.t()*ML;
	params = inv.inv()*ATL;

	det[0][0] = params.at(0, 0) + params.at(0, 1)*checkpoint.at(0, 2) + params.at(0, 2)*checkpoint.at(0, 3) - checkpoint.at(0, 0);
	det[1][0] = params.at(0, 0) + params.at(0, 1)*checkpoint.at(1, 2) + params.at(0, 2)*checkpoint.at(1, 3) - checkpoint.at(1, 0);

	fout << "a0,a1,a2:\n";
	fout << params;
	Params[3] = params.at(0, 0);
	Params[4] = params.at(2, 0);
	Params[5] = params.at(1, 0);
	v = MA*params - ML;
	zhongwucha = v.t()*v;
	fout << "x的中误差：" << zhongwucha;
	

	for (i = 0; i < 5; i++)
	{
		ML.at(i, 0) = LB[i];
		for (j = 0; j < 3; j++)
		{
			MA.at(i, j) = B[i][j];
		}
	}

	inv = MA.t()*MA;
	//cout << inv<<MA;
	ATL = MA.t()*ML;
	params = inv.inv()*ATL;

	det[0][1] = params.at(0, 0) + params.at(0, 1)*checkpoint.at(0, 2) + params.at(0, 2)*checkpoint.at(0, 3) - checkpoint.at(0, 1);
	det[1][1] = params.at(0, 0) + params.at(0, 1)*checkpoint.at(1, 2) + params.at(0, 2)*checkpoint.at(1, 3) - checkpoint.at(1, 1);

	v = MA*params - ML;
	zhongwucha = v.t()*v;
	fout << "b0,b1,b2:\n";
	fout << params;
	fout << "y的中误差：" << zhongwucha;

	Params[0] = params.at(0, 0);
	Params[1] = params.at(2, 0);
	Params[2] = params.at(1, 0);
	
	fout << "与两对检测点的较差 dx,dy:\n";
	fout << det[0][0] << "  " << det[0][1]<<"\n";
	fout << det[1][0] << "  " << det[1][1]<<"\n";

	fout.close();
	fin.close();
	Mat img = imread("待纠正图像.bmp");
	//Mat gray;//灰色图像
	Mat out;//转换后的图像
	//cvtColor(img, gray, CV_BGR2GRAY);
	/*outgray.create(gray.size(), gray.type());
	for ( i = 0; i < gray.rows/2; i++)
	{
		for ( j = 0; j < gray.cols/3; j++)
		{
			outgray.ptr<uchar>(i)[j]=gray.ptr<uchar>(i)[j];
		}
	}
	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
	imshow("image", outgray);
	waitKey();*/

	//cout << gray.rows <<"  "<< gray.cols;
	out.create(img.rows+90,img.cols+120,img.type());
	//cout << outgray.rows <<"  "<< outgray.cols;
	for ( i = 0; i <out.rows; i++)
	{
		for ( j = 0; j < out.cols; j++)
		{
			Matrix Wx(1, 2);
			Matrix I(2, 2);
			Matrix Wy(2, 1);
			Matrix Ip(1, 1);
			double ai, aj;//平移后的像坐标
			double x, y;//重采样的像坐标
			ai = i; aj = j ;
			x = Params[0] + Params[1] * ai + Params[2] * aj;
			y = Params[3] + Params[4] * ai + Params[5] * aj;
			if (x<=0||y<=0||x>=685||y>=563)
			{
				out.ptr < uchar >(i)[3*j]= 255;
				out.ptr < uchar >(i)[3 * j+1] = 255;
				out.ptr < uchar >(i)[3 * j+1] = 255;
			}
			else
			{
				double dx, dy;
				dx = x - int(x);
				dy = y - int(y);
				Wx.at(0, 0) = 1 - dx;
				Wx.at(0, 1) = dx;
				Wy.at(0, 0) = 1 - dy;
				Wy.at(1, 0) = dy;
				I.at(0, 0) = img.ptr<uchar>(int(x))[3*int(y)]; I.at(0, 1) = img.ptr<uchar>(int(x))[3*(int(y) + 1)];
				I.at(1, 0) = img.ptr<uchar>(int(x)+1)[3*int(y)]; I.at(1, 1) = img.ptr<uchar>(int(x)+1)[3*(int(y) + 1)];
				Ip = Wx*I*Wy;
				out.ptr < uchar >(i)[3*j] = Ip.at(0,0);

				I.at(0, 0) = img.ptr<uchar>(int(x))[3 * int(y)+1]; I.at(0, 1) = img.ptr<uchar>(int(x))[3 * (int(y) + 1)+1];
				I.at(1, 0) = img.ptr<uchar>(int(x) + 1)[3 * int(y)+1]; I.at(1, 1) = img.ptr<uchar>(int(x) + 1)[3 * (int(y) + 1)+1];
				Ip = Wx*I*Wy;
				out.ptr < uchar >(i)[3 * j+1] = Ip.at(0, 0);

				I.at(0, 0) = img.ptr<uchar>(int(x))[3 * int(y)+2]; I.at(0, 1) = img.ptr<uchar>(int(x))[3 * (int(y) + 1+2)];
				I.at(1, 0) = img.ptr<uchar>(int(x) + 1)[3 * int(y)+2]; I.at(1, 1) = img.ptr<uchar>(int(x) + 1)[3 * (int(y) + 1)+2];
				Ip = Wx*I*Wy;
				out.ptr < uchar >(i)[3 * j+2] = Ip.at(0, 0);
			}
		}
	}
	imwrite("纠正后的影像.bmp", out);
	/*cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
	imshow("image", outgray);
	waitKey();
	system("pause");	*/
	return 0;


}