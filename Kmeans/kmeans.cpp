#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<stdlib.h>
#include<ctime>
#include<cstdlib>
#define N 999
const int MAX_KERNEL_LENGTH = 10;
using namespace std;
using namespace cv;
double GetDistance(Point3d a, Point3d b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}

void  SelectPoint(vector<Point3d> &center, int Pnum,int m,Mat &img)
{
	double sum=0,max=0,Lnum=0;
	Point3d MaxPoint;
	if (Pnum==m)
	{
		return;
	}
	for (int j = 0; j < img.rows; j++)
	{
		for (int k = 0; k < img.cols; k++)
		{
			
			Point3d tmp;
			double Tmp=0;
			tmp.x = img.ptr<uchar>(j)[3 * k];
			tmp.y = img.ptr<uchar>(j)[3 * k + 1];
			tmp.z = img.ptr<uchar>(j)[3 * k + 2];
			bool jud = true;
			for (int i = 0; i < center.size(); i++)
			{
				if (tmp == center[i])
				{
					jud = false;
					break;
				}
			}
			if (jud)
			{
				for (int i = 0; i < center.size(); i++)
				{
					Tmp += GetDistance(tmp, center[i]);

				}
				if (Tmp>max)
				{
					max = Tmp;
					MaxPoint = tmp;
				}
			}
			
		}
	}
	center.push_back(MaxPoint);
	SelectPoint(center, ++Pnum, m, img);
}


int main()
{
	Mat img = imread("a.png");
	vector<Point3d> guiyi;
	vector<Point3d> center ;
	vector<Point3d> newcenter;
	vector<Point2d> sort[5];
	Point3d sum[5];
	Point3d tmp;
	int Pnum;
	printf("输入初始坐标:\n");
	scanf("%lf %lf %lf",&tmp.x,&tmp.y,&tmp.z);
	/*for ( int i = 0; i < img.rows; i ++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			Point3d tmp;
			tmp.x = double(img.ptr<uchar>(i)[3 * j]);
			tmp.y = double(img.ptr<uchar>(i)[3 * j+1]);
			tmp.z = double(img.ptr<uchar>(i)[3 * j+2]);
			guiyi.push_back(tmp);

		}
	}*/
	/*newcenter[0].x = 50, newcenter[0].y = 50,newcenter[0].z=50;
	newcenter[1].x = 80, newcenter[1].y = 80,newcenter[1].z=80;
	newcenter[2].x = 100, newcenter[2].y = 100,newcenter[2].z=100;
	newcenter[3].x = 130, newcenter[3].y = 130,newcenter[3].z=130;
	newcenter[4].x = 160, newcenter[4].y = 160,newcenter[4].z=160;*/
	//srand(time(NULL));
	/*for (int i = 0; i < 5; i++)
	{
		
		newcenter[i].x = rand()%(N+1)/(double)(N+1)*255;
		newcenter[i].y = rand() % (N + 1) / (double)(N + 1) * 255;
		newcenter[i].z = rand() % (N + 1) / (double)(N + 1) * 255;
	}*/
	//srand(time(NULL));
	//tmp.x = tmp.y = tmp.z = 50;
	newcenter.push_back(tmp);
	SelectPoint(newcenter, 1, 5, img);
	//对center初始化
	tmp.x = tmp.y = tmp.z = 0;
	for (int i = 0; i < 5; i++)
	{
		center.push_back(tmp);
	}
	
	double min;
	int minN;
	Point2d p;
	Point3d a;
	int m = 0;
	while (abs(GetDistance(newcenter[0], center[0])) + abs(GetDistance(newcenter[1], center[1])) + abs(GetDistance(newcenter[2], center[2])) + abs(GetDistance(newcenter[3], center[3])) + abs(GetDistance(newcenter[4], center[4]))>1)
	{
		m++;
		cout << m;
		for (int i = 0; i < 5; i++)
		{
			sort[i].clear();
			sum[i].x = 0; sum[i].y = 0; sum[i].z = 0;
			center[i] = newcenter[i];
		}
		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				min = 1000000;
				for (int k = 0; k < 5; k++)
				{
					
					a.x = double(img.ptr<uchar>(i)[3 * j]);
					a.y = double(img.ptr<uchar>(i)[3 * j+1]);
					a.z = double(img.ptr<uchar>(i)[3 * j+2]);
					double tmp = GetDistance(a, center[k]);
					if (tmp<min)
					{
						min = tmp;
						minN = k;
					}
				}
				p.x = i, p.y = j;
				sort[minN].push_back(p);
				sum[minN].x+=a.x;
				sum[minN].y += a.y;
				sum[minN].z += a.z;
			}
		}
		for (int i = 0; i < 5; i++)
		{
			newcenter[i].x = sum[i].x / sort[i].size();
			newcenter[i].y = sum[i].y / sort[i].size();
			newcenter[i].z = sum[i].z / sort[i].size();
		}
	}
	for (int i = 0; i < sort[0].size(); i++)
	{
		int x = sort[0][i].x;int  y = sort[0][i].y;
		img.ptr<uchar>(x)[y * 3] = 255;
		img.ptr<uchar>(x)[y * 3+1] = 0;
		img.ptr<uchar>(x)[y * 3+2] = 0;
	}

	for (int i = 0; i < sort[1].size(); i++)
	{
		int x = sort[1][i].x;int  y = sort[1][i].y;
		img.ptr<uchar>(x)[y * 3] = 0;
		img.ptr<uchar>(x)[y * 3+1] = 255;
		img.ptr<uchar>(x)[y * 3+2] = 0;
	}
	for (int i = 0; i < sort[2].size(); i++)
	{
		int x = sort[2][i].x;int y = sort[2][i].y;
		img.ptr<uchar>(x)[y * 3] = 0;
		img.ptr<uchar>(x)[y * 3+1] = 0;
		img.ptr<uchar>(x)[y * 3+2] = 255;
	}
	for (int i = 0; i < sort[3].size(); i++)
	{
		int x = sort[3][i].x;int y = sort[3][i].y;
		img.ptr<uchar>(x)[y * 3] = 0;
		img.ptr<uchar>(x)[y * 3+1] = 255;
		img.ptr<uchar>(x)[y * 3+2] = 255;
	}
	for (int i = 0; i < sort[4].size(); i++)
	{
		int x = sort[4][i].x;int y = sort[4][i].y;
		img.ptr<uchar>(x)[y * 3] = 255;
		img.ptr<uchar>(x)[y * 3+1] = 255;
		img.ptr<uchar>(x)[y * 3+2] = 0;
	}


	Mat dst;//平滑后的结果
	for (int i = 1; i<MAX_KERNEL_LENGTH; i += 2) {
		GaussianBlur(img, dst, Size(i, i), 0, 0);
		imshow("smoothing", dst);
		waitKey(1000);
	}


	imwrite("img.bmp", img);
	imwrite("Gaudist.bmp", dst);

	/*cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
	imshow("img", img);
	waitKey();*/
	
	return 0;
}
