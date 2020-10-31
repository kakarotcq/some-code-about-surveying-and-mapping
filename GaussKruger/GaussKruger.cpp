#include<iostream>
#include<fstream>
#include<string>
using namespace std;
const double Pi = 4 * atan(1);
double a = 6378140, b = 6356755.2881575287, c = 6399596.6519880105, f = 1 / 298.257; //1975坐标系
double e2 = 0.006694384999588;
double e12 = 0.006739501819473;
double N = 6383646.608422;

string filename = "data,out";
ofstream fout(filename.c_str());
double trans(int a, int b, double c) //角转弧度
{
	return (a * 3600 + b * 60 + c) / ((360 * 60 * 60) / (2 * Pi));
}
void itrans(double angle)  //弧度转角度
{
	angle *= ((360 * 60 * 60) / (2 * Pi));
	double a = angle - (int)angle / 60 * 60;
	angle = (angle - a) / 60;
	double b = ((int)angle) % 60;
	angle = (angle - b) / 60;
	double c = angle;
	fout << c << "°" << b << "'" << a << "\"";
}

void inverse(double x,double y)
{
	double Bf, l, B;
	//double x, y;
	int tzone;//时区
	//cout << "输入x,y\n";
	//cin >> x >> y;
	B = x / 6367452.133;
	tzone = (int)y / pow(10, 6);
	y = y - tzone*pow(10, 6) - 500000;
	Bf = B + (50228976 + (293697 + (2383 + 22 * pow(cos(B), 2))*pow(cos(B), 2))*pow(cos(B), 2))*pow(10, -10)*sin(B)*cos(B);
	double Wf, Nf, Vf, Mf;
	double tf, yitaf;
	tf = tan(Bf);
	yitaf = e12*pow(cos(B), 2);
	Wf = sqrt(1 - e2*pow(sin(B), 2));
	Vf = sqrt(1 - e2*pow(cos(B), 2));
	Nf = a / Wf;
	Mf = c / pow(Vf, 3);
	B = Bf - tf*y*y / (2 * Mf*Nf) + tf*(5 + 3 * tf*tf + pow(yitaf, 2) - 9 * pow(yitaf, 2)*pow(tf, 2))*pow(y, 4) / (24 * Mf*pow(Nf, 5)) - tf*(61 + 90 * pow(tf, 2) + 45 * pow(tf, 4))*pow(y, 6) / (720 * Mf*pow(Nf, 5));
	l = y / (Nf*cos(Bf)) - (1 + 2 * tf*tf + pow(yitaf, 2))*pow(y, 3) / (6 * pow(Nf, 3)*cos(Bf)) + (5 + 28 * tf*tf + 24 * pow(tf, 4))*pow(y, 5) / (120 * pow(Nf, 5)*cos(Bf));
	l = l + (tzone - 1 + 0.5) * 2 * Pi / 360 * 6;
	fout << "反算结果";
	fout << "B=:"; itrans(B); fout << " ";
	fout << "L=:";
	itrans(l); fout << "\n";
}
int main()
{
	/*char tmp;
	cout << "正算输入D，反算输入I\n";
	tmp = getchar();
	if (tmp == 'D') direct();
	else if (tmp = 'I')
	{
		inverse();
	}
	else
		cout << "wrong input";*/
	//double M, N, V, W;
	
	double B, L, X;
	double t, yita2;
	int i, j=0; double k=0;
	int a,b=0 ;
	double x, y;
	//cout << "输入纬度B，单位间用空格隔开\n";
	for (i = 0; i <= 80; i += 10)
		for (a = 111; a < 114; a++)
		{
			while (b<=60)
			{
				
				B = trans(i, j, k); 
				//cout << "输入经度L，单位间用空格隔开\n";
				//cin >> i >> j >> k;	
				L = trans(a, b, k);
				t = tan(B);
				yita2 = e12*pow(cos(B), 2);
				L = L * 360 / (2 * Pi);
				int tmp;
				if (L - L / 6)  tmp = L / 6 + 1;
				else
				{
					tmp = L / 6;
				}
				L = L - (int)L / 6 * 6;
				L = L - 3;
				L = L * 2*Pi/360;

				double B0 = B * 360 / (2 * Pi);
				//double tmpB = 0, n = 10000, dB = B / n; //求出 X
				X = 111133.005*B0 - 32009.858*sin(B)*cos(B) - 133.960*pow(sin(B), 3)*cos(B) - 0.698*pow(sin(B), 5)*cos(B);
				/*double tB = 0;
				for (i = 0; i < n; i++)
				{
				tmpB = tB + dB / 2.0;
				//W = sqrt(1 - e2*pow(sin(tmpB), 2));
				V = sqrt(1 - e2*pow(cos(tmpB), 2));
				M = c / pow(V, 3);
				X += (M*dB);
				tB += dB;
				}*/
				//W = sqrt(1 - e2*pow(sin(B), 2));
				//V = sqrt(1 - e2*pow(cos(B), 2));
				//N = a / W;
				//M = c / pow(V, 3);
				x = X + N / 2 * sin(B)*cos(B)*pow(L, 2) + N / 24 * sin(B)*pow(cos(B), 3)*(5 - t*t + 9 * yita2 + 4 * pow(yita2, 2))*pow(L, 4) + N / 720 * sin(B)*pow(cos(B), 5)*(61 - 58 * t*t + pow(t, 4))*pow(L, 6);
				y = N*cos(B)*L + N / 6 * pow(cos(B), 3)*(1 - t*t +yita2)*pow(L, 3) + N / 120 * pow(cos(B), 5)*(5 - 18 * pow(t, 2) + pow(t, 4) + 14 * yita2 - 58 * t*t*yita2)*pow(L, 5);
				y = y + tmp * 1000000 + 500000;
				fout << "输入纬度B=" << i << "°" << j << "'" << k << "\"";
				fout << "输入经度L=" << a << "°" << b << "'" << k << "\"" << "\n";
				fout << "正算结果:";
				fout <<"x="<< x << " ";
				fout <<"y="<< y << "\n";
				inverse(x, y);
				b += 20;
				fout << "\n";
			}
			b = 20;
		}

	
	
	system("pause");
	return 0;
}
