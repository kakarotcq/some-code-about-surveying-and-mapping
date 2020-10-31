using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using matrixcal;
using System.IO;

namespace coordinatetran
{
    class Program
    {
        static void Main(string[] args)
        {
            string filename = "D:/Visual Studio 2015/Projects/coordinatetran/data.txt";
            StreamReader reader = null;
            double[,] b=new double[18,7];
            double[,] l=new double[18,1];
            double[] x = new double[7];
            double[,] p = new double[18, 18];
            double Pi = Math.Atan(1) * 4;//得出Pi的值
           // Console.Write(Pi);
            for (int j = 0; j < p.GetLength(0); j++)
                for (int k = 0; k < p.GetLength(1); k++)
                    if (j == k) p[j, k] = 1; //初始化P矩阵（其实没必要··）
            int i=0;
           // try
           // {
                //FileStream stream =new FileStream(filename, FileMode.Create);
                reader = new StreamReader(filename); //开始读入文件中的数据
                string line;
                for(;;)
                {
                    line = reader.ReadLine();//Console.WriteLine(line); 读入字符串
                    if (line == "end") break; //读完第一组要停下
                    string[] xyz = line.Split(' ');//收集有用的字符并接下来转化成double型数字
                    b[i, 0] = 1;b[i, 1] = 0;b[i, 2] = 0;b[i,3]= double.Parse(xyz[1]);b[i, 4] = 0;b[i,5]= -double.Parse(xyz[3]);b[i,6]= double.Parse(xyz[2]);
                    b[++i, 0] = 0;b[i, 1] = 1;b[i,2]=0;b[i,3]=double.Parse(xyz[2]);b[i,4]=double.Parse(xyz[3]);b[i,5]=0;b[i,6]=-double.Parse(xyz[1]);
                    b[++i, 0] = 0;b[i, 1] =0;b[i,2]=1;b[i,3]=double.Parse(xyz[3]);b[i,4]=-double.Parse(xyz[2]);b[i,5]=double.Parse(xyz[1]);b[i,6]=0;
                    Array.Clear(xyz,0,xyz.Length);//清空数组节省空间
                    i++;
                }
                i = 0;
                for (;;)
                {
                    line = reader.ReadLine(); //读入L数组的数据
                    if (line =="end") break;
                    string[] xyz = line.Split(' ');
                    l[i,0] = double.Parse(xyz[1]);
                    l[++i,0] = double.Parse(xyz[2]);
                    l[++i,0] = double.Parse(xyz[3]);
                    i++;
                    Array.Clear(xyz, 0, xyz.Length);
                }
            reader.Close();//读完关文件
            //for (i = 0; i < l.Length; i++)
                //Console.WriteLine("{0}",l[i,0]);
            double[,] tmp = Matrix.MultiplyMatrix(Matrix.Transpose(b),p); 
            //以下是按照公式进行计算
            double[,] X = Matrix.MultiplyMatrix(Matrix.Athwart(Matrix.MultiplyMatrix(tmp, b)), Matrix.MultiplyMatrix(tmp, l));
            //Console.WriteLine("{0} {1}", X.GetLength(0), X.GetLength(1));
            /*for (int j = 0; j < X.GetLength(0);j++)
                Console.WriteLine("{0}",X[j,0]);*/
            double K = X[3, 0] - 1; K *= 1000000;
            double ex = X[4, 0] / X[3, 0]*1296000/(2*Pi);
            double ey = X[5, 0] / X[3, 0]*1296000/(2*Pi);
            double ez = X[6, 0] / X[3, 0]*1296000/(2*Pi);
            Console.WriteLine("DX={0}\nDY={1}\nDZ={2}\nex={3}\ney={4}\nez={5}\nK={6}",X[0,0],X[1,0],X[2,0],ex,ey,ez,K);
            double [,]V = Matrix.SubMatrix(l,Matrix.MultiplyMatrix(b,X));
            double[,] m0 = Matrix.MultiplyMatrix(Matrix.Transpose(V), V);
            m0[0, 0] = Math.Sqrt(m0[0,0]*10000/11);
            Console.WriteLine("m0={0}",m0[0,0]);
            StreamWriter writer = null;
            //本想将数据输出至文件，但是试验后发现可以创建文件却不能写入内容··觉得很奇怪，目前还未解决，暂且输出至黑框
            filename = "D:/Visual Studio 2015/Projects/coordinatetran/dataout.txt";
            FileStream fs = new FileStream(filename, FileMode.OpenOrCreate, FileAccess.ReadWrite);
            writer = new StreamWriter(fs);
            writer.WriteLine("hi");
            Console.ReadLine();
                // Console.Write(x1);
                //double x2
                // }
                //catch (IOException e)
                // { Console.WriteLine(e.Message); }
                // finally
                // { if (reader != null) reader.Close(); }

        }
    }
}
