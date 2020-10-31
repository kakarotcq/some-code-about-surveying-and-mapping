#include "Matrix.h"
#include <iostream>
#include <ios>
#include <iomanip>
#include <cassert>
#include <vector>

//Size's member functions:

Size::Size() :cols(0), rows(0){}

Size::Size(int rows, int cols) : rows(rows), cols(cols){}

Size::Size(const Size & s) : rows(s.rows), cols(s.cols){}

std::ostream & operator << (std::ostream &out, const Size & s)
{
	out << '[' << s.rows << ',' << s.cols << ']';
	return out;
}

const Size & Size::operator = (const Size & s)
{
	rows = s.rows;
	cols = s.cols;

	return *this;
}

bool Size:: operator == (const Size &s)
{
	return rows == s.rows && cols == s.cols;
}

bool Size::operator != (const Size &s)
{
	return rows != s.rows || cols != s.cols;
}

//Matrix's member functions:

Matrix::Matrix() : rows(0), cols(0), data(nullptr){}

Matrix::Matrix(const Matrix &mat) : rows(mat.rows), cols(mat.cols), _size(mat._size)
{
	int total = rows*cols;
	data = new double[total];
	double *p = data, *q = mat.data;
	for (int i = 0; i < total; ++i)
	{
		*p++ = *q++;
	}
}

Matrix::Matrix(int rows, int cols, double *p)
	:rows(rows), cols(cols), data(p), _size(rows, cols)
{
	if (!data)
	{
		int total = rows*cols;
		data = new double[total]();
	}
}

Matrix::Matrix(int rows, int cols, const std::vector<double> &v)
	:rows(rows), cols(cols), _size(rows, cols)
{
	data = new double[rows*cols]();
}

Matrix::~Matrix()
{
	//delete[]data;
}

Size Matrix::size()const
{
	return _size;
}

bool Matrix::empty()const
{
	return data == nullptr;
}

void Matrix::create(int rows, int cols)
{
	bool need = data == nullptr && this->rows != 0 && this->cols != 0;
	need = need && rows != this->rows&&cols != this->cols;
	if (need)
		data = new double[rows*cols]{0};
}

Matrix Matrix::inv()const
{
	assert(rows == cols);

	double *src = new double[rows*cols], *p = src,
		*dst = new double[rows*cols], *q = dst;

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			*p++ = *(data + i*cols + j);
			*q++ = i == j ? 1 : 0;
		}
	p = src, q = dst;

	for (int i = 0; i < rows; i++)
	{
		int row = i;
		double max = *(src + i*cols + i);
		for (int j = i + 1; j < rows; j++)
			if (max < *(src + j*cols + i))
			{
				max = *(src + j*cols + i);
				row = j;
			}

		if (max < 1e-15 && max >-1e-15)
		{
			std::cerr << "ÆæÒì¾ØÕó²»¿É×ªÖÃ!\n";
			abort();
		}

		if (row != i)
			for (int k = 0; k < cols; k++)
			{
				double srctemp = *(src + i*cols + k),
					dsttemp = *(dst + i*cols + k);

				*(src + i*cols + k) = *(src + row*cols + k);
				*(src + row*cols + k) = srctemp;

				*(dst + i*cols + k) = *(dst + row*cols + k);
				*(dst + row*cols + k) = dsttemp;
			}

		double div = *(src + i*cols + i);
		for (int j = 0; j < cols; j++)
		{
			*(src + i*cols + j) /= div;
			*(dst + i*cols + j) /= div;
		}

		for (int j = 0; j < rows; j++)
		{
			if (i == j)
				continue;

			double temp = *(src + j*cols + i);
			for (int k = 0; k < cols; k++)
			{
				*(src + j*cols + k) -= temp* *(src + cols*i + k);
				*(dst + j*cols + k) -= temp* *(dst + cols*i + k);
			}
		}

	}

	delete[]src;
	return Matrix(rows, cols, dst);
}

Matrix Matrix::t()const
{
	assert(data != nullptr);

	double *ret = new double[rows*cols],
		*p = ret;

	for (int i = 0; i < cols; i++)
		for (int j = 0; j < rows; j++)
		{
			*p++ = *(data + j*cols + i);
		}

	return Matrix(cols, rows, ret);
}

double Matrix::det()const
{
	assert(rows == cols && data != nullptr);

	if (rows == 1)
		return *data;

	double result = 0;
	int loop = rows == 2 ? 1 : rows;
	for (int i = 0; i < loop; i++)
	{
		double temp = 1;
		for (int row = 0, col = i; row < rows; row++, col++)
			temp *= *(data + row*rows + col%rows);
		result += temp;
	}

	for (int i = 0; i < loop; i++)
	{
		double temp = 1;
		for (int row = 0, col = cols - i - 1 + cols; row < rows; row++, col--)
			temp *= *(data + row*rows + col%rows);
		result -= temp;
	}

	return result;
}

std::vector<double> Matrix::diag()const
{
	assert(data != nullptr);

	std::vector<double> v;
	for (int i = 0; i < rows; i++)
		v.push_back(*(data + i*cols + i));

	return v;
}

void Matrix::diag(int n, const double *p)
{
	rows = cols = n;
	_size = Size(n, n);
	delete[]data;
	data = new double[n*n];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			*(data + i*n + j) = (i == j) ? *p++ : 0;
}

Matrix& Matrix::operator = (const Matrix &mat)
{
	if (this == &mat)
		return *this;

	rows = mat.rows;
	cols = mat.cols;
	int total = rows*cols;

	if (mat.data != nullptr)
	{
		if (_size != mat._size)
		{
			delete[]data;
			data = new double[total];
		}

		double *p = data, *q = mat.data;
		for (int i = 0; i < total; ++i)
			*p++ = *q++;
	}
	else
	{
		delete[]data;
		data = nullptr;
	}
	_size = mat._size;

	return *this;
}

Matrix& Matrix::operator += (const Matrix &mat)
{
	if (&mat == this)
		return *this;

	for (int i = 0; i < rows; ++i)
	{
		size_t offset = i*cols;
		for (int j = 0; j < cols; ++j)
		{
			*(data + offset + j) += *(mat.data + offset + j);
		}
	}

	return *this;
}

std::ostream & operator << (std::ostream &out, const Matrix &mat)
{
	if (mat.data == nullptr)
		return out;

	std::streamsize size = out.precision(4);
	out << std::fixed;
	double *p = mat.data;

	for (int i = 0; i < mat.rows; ++i)
	{
		for (int j = 0; j < mat.cols; j++)
		{
			out << std::setw(6)<< *p++ << '\t';
		}
		out << std::endl;
	}

	out.precision(size);
	return out;
}

std::istream & operator >> (std::istream &in, Matrix &mat)
{

	for (int i = 0; i < mat.rows; i++)
		for (int j = 0; j < mat.cols; j++)
		{
			in >> *(mat.data + i*mat.cols + j);
		}

	return in;
}

Matrix operator + (const Matrix &matl, const Matrix &matr)
{
	assert(matl.size() == matr.size() && matl.data != nullptr);

	double *p = matl.data, *q = matr.data;
	int total = matl.rows * matl.cols;
	double *r = new double[total], *ret = r;
	for (int i = 0; i < total; ++i)
		*(r + i) = 0.0;

	for (int i = 0; i < total; ++i)
		*ret++ = *p++ + *q++;

	return Matrix(matl.rows, matl.cols, r);
}

Matrix operator - (const Matrix &matl, const Matrix &matr)
{
	assert(matl.size() == matr.size() && matl.data != nullptr);

	double *p = matl.data, *q = matr.data;
	int total = matl.rows*matl.cols;
	double *r = new double[total], *ret = r;
	for (int i = 0; i < total; ++i)
		*(r + i) = 0.0;

	for (int i = 0; i < total; ++i)
		*ret++ = *p++ - *q++;

	return Matrix(matl.rows, matl.cols, r);
}

Matrix operator * (const Matrix &matl, const Matrix &matr)
{
	assert(matl.cols == matr.rows && matl.data != nullptr);

	double *p = matl.data, *q = matr.data;
	double *ret = new double[matl.rows*matr.cols];

	for (int i = 0; i < matl.rows; i++)
		for (int j = 0; j < matr.cols; j++)
		{
			double t = 0.0;
			for (int k = 0; k < matl.cols; k++)
				t += *(p + i*matl.cols + k) * *(q + k*matr.cols + j);

			*(ret + i*matr.cols + j) = t;
		}

	return Matrix(matl.rows, matr.cols, ret);
}

Matrix operator * (double v, const Matrix &m)
{
	assert(m.data);

	double *p = m.data;
	int total = m.rows*m.cols;
	double *r = new double[total], *ret = r;
	for (int i = 0; i < total; ++i)
		*r++ = (*p++)*v;

	return Matrix(m.rows, m.cols, ret);
}

Matrix operator * (const Matrix &m, double v)
{
	return v*m;
}

Matrix::operator double()const
{
	if (data) {
		return *data;
	}
	return 0;
}