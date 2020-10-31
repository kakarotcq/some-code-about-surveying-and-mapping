#pragma once
#include <vector>

struct Size
{
	int cols;
	int rows;
	Size();
	Size(const Size &);
	Size(int row, int col);
	friend std::ostream & operator << (std::ostream &out, const Size &);
	const Size & operator = (const Size &);
	bool operator == (const Size &);
	bool operator != (const Size &);
};


class Matrix
{
public:
	Matrix();
	Matrix(const Matrix &);
	Matrix(int row, int col, double *data = nullptr);
	Matrix(int row, int col, const std::vector<double> &);
	~Matrix();

	Size size()const;
	bool empty()const;
	void create(int rows, int cols);	//need modifying
	double &at(int row, int col);
	const double &at(int row, int col)const;

	Matrix inv()const;
	Matrix t()const;
	double det()const;
	std::vector<double> diag()const;
	void diag(int n, const double *p);

	friend std::ostream & operator << (std::ostream & out, const Matrix &);
	friend std::istream & operator >> (std::istream &in, Matrix &);

	explicit operator double()const;
	Matrix& operator = (const Matrix &);
	Matrix& operator += (const Matrix &);
	friend Matrix operator * (const Matrix &, const Matrix &);
	friend Matrix operator + (const Matrix &, const Matrix &);
	friend Matrix operator - (const Matrix &, const Matrix &);
	friend Matrix operator * (double, const Matrix &);
	friend Matrix operator * (const Matrix &, double);

private:
	int rows;
	int cols;
	Size _size;
	double *data;
};

Matrix operator * (const Matrix &, const Matrix &);
Matrix operator + (const Matrix &, const Matrix &);
Matrix operator - (const Matrix &, const Matrix &);


inline double &Matrix::at(int row, int col)
{
	return *(data + cols*row + col);
}

inline const double &Matrix::at(int row, int col)const
{
	return *(data + cols*row + col);
}
