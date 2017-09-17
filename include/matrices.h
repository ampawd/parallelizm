#ifndef _MATRICES_H
#define _MATRICES_H

#include <vector>
#include <iostream>
#include <initializer_list>

#include <cassert>

namespace parallelizm 
{
	template<class T>
	struct Matrix
	{
		size_t rows, cols;
		std::vector< std::vector<T> > mat;
		Matrix(size_t rows = 3, size_t cols = 3): 
			rows(rows), 
			cols(cols),
			mat(rows, std::vector<T>(cols))
		{}

		Matrix(std::initializer_list< std::initializer_list<T> >&& list)
		{
			mat.assign(list.begin(), list.end());
			rows = mat.size();
			cols = mat[0].size();
		}

		std::vector<T>& operator [] (size_t i)
		{
			return mat[i];
		}

		const std::vector<T>& operator [] (size_t i) const
		{
			return mat[i];
		}

		friend std::ostream& operator << (std::ostream& o, const Matrix& m)
		{
			for (size_t i=0; i < m.rows; ++i)
			{
				for (size_t j = 0; j < m.cols; ++j)
				{
					o << m[i][j] << " ";
				}

				o << std::endl;
			}

			return o;
		}

	};

	template<class T>
	void naive_mult(const Matrix<T>& a, const Matrix<T>& b, Matrix<T>& c)
	{
		assert(a.cols == b.rows);
		size_t n = a.rows, m = a.cols, l = b.rows;
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				for (int k = 0; k < l; ++k)
				{
					c[i][j] += a[i][k]*b[k][j];
				}
			}
		}
	}
};

#endif