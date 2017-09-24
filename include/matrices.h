#ifndef _MATRICES_H
#define _MATRICES_H

#include <vector>
#include <iostream>
#include <initializer_list>

#include <cassert>


namespace parallelizm 
{
	#define MATRIX_ELEMENT_UPPER_BOUND 100


	template<class T>
	class Matrix
	{
	private:
		std::vector< std::vector<T> > mat;
		size_t _rows, _cols;
		
	public:
		
		Matrix(size_t rows=0, size_t cols=0);
		
		Matrix(std::initializer_list< std::initializer_list<T> >&& );
		
		void fill(size_t blockSize, const T& value = 0);
		
		std::vector<T>& operator [] (size_t);

		const std::vector<T>& operator [] (size_t) const;
		size_t rows() const;
		size_t cols() const;
	};


	template<class T>
	Matrix<T>::Matrix(size_t rows, size_t cols): 
		_rows(rows),
		_cols(cols),
		mat(rows, std::vector<T>(cols))
	{}

	template<class T>
	Matrix<T>::Matrix(std::initializer_list< std::initializer_list<T> >&& list)
	{
		mat.assign(list.begin(), list.end());
		_rows = mat.size();
		_cols = mat[0].size();
	}

	template<class T>
	void Matrix<T>::fill(size_t blockSize, const T& value)
	{
		mat.resize(blockSize);
		for (int i = 0; i < blockSize; ++i)
		{
			mat[i].resize(blockSize);
			for (int j = 0; j < blockSize; ++j)
			{
				mat[i][j] = value;
			}
		}
		_rows = _cols = blockSize;
	}


	template<class T>
	std::vector<T>& Matrix<T>::operator [] (size_t i)
	{
		return mat[i];
	}

	template<class T>
	const std::vector<T>& Matrix<T>::operator [] (size_t i) const
	{
		return mat[i];
	}

	template<class T>
	size_t Matrix<T>::rows() const
	{
		return _rows;
	}

	template<class T>
	size_t Matrix<T>::cols() const
	{
		return _cols;
	}

	template<class T>
	std::ostream& operator << (std::ostream& o, const Matrix<T>& m)
	{
		size_t rows = m.rows(), cols = m.cols();
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < cols; ++j)
			{
				o << m[i][j] << " ";
			}

			o << std::endl;
		}

		return o;
	}





	template<class T>
	class BlockedMatrix
	{
	private:
		std::vector<std::vector<Matrix<T>>> mat;
		size_t blockSize;
		size_t _rows, _cols;

	public:
		BlockedMatrix(size_t, size_t, size_t);
		size_t getBlockSize() const;
		std::vector<Matrix<T>>& operator [] ( size_t );
		const std::vector<Matrix<T>>& operator [] ( size_t ) const;
		size_t rows() const;
		size_t cols() const;
	};

	template<class T>
	BlockedMatrix<T>::BlockedMatrix(size_t rows, size_t cols, size_t blockSize): 
		_rows(rows),
		_cols(cols),
		blockSize(blockSize),
		mat(rows, std::vector<Matrix<T>>(cols))
	{
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				mat[i][j].fill(blockSize);
			}
		}
	}

	template<class T>
	size_t BlockedMatrix<T>::getBlockSize() const
	{
		return blockSize;
	}

	template<class T>
	std::vector<Matrix<T>>& BlockedMatrix<T>::operator [] (size_t i)
	{
		return mat[i];
	}

	template<class T>
	const std::vector<Matrix<T>>& BlockedMatrix<T>::operator [] (size_t i) const
	{
		return mat[i];
	}

	template<class T>
	size_t BlockedMatrix<T>::rows() const
	{
		return _rows;
	}

	template<class T>
	size_t BlockedMatrix<T>::cols() const
	{
		return _cols;
	}

	template<class T>
	std::ostream& operator << (std::ostream& o, const BlockedMatrix<T>& m)
	{
		size_t rows = m.rows(), cols = m.cols();
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < cols; ++j)
			{
				o << m[i][j];
			}
		}

		return o;
	}





	template<class T>
	void naive_mult(const Matrix<T>& a, const Matrix<T>& b, Matrix<T>& c)
	{
		assert(a.cols() == b.rows());
		size_t n = a.rows(), m = a.cols(), l = b.rows();
		for (size_t i = 0; i < n; ++i)
		{
			for (size_t j = 0; j < m; ++j)
			{
				for (size_t k = 0; k < l; ++k)
				{
					c[i][j] += a[i][k]*b[k][j];
				}
			}
		}
	}



	void simple_blocked_mult(const BlockedMatrix<int>& a, const BlockedMatrix<int>& b, BlockedMatrix<int>& c)
	{
		assert(a.cols() == b.rows());

		size_t n = a.rows(), m = a.cols(), l = b.rows();
		for (size_t i = 0; i < n; ++i)
		{
			for (size_t j = 0; j < m; ++j)
			{
				for (size_t k = 0; k < l; ++k)
				{					
					naive_mult(a[i][k], b[k][j], c[i][j]);
				}

			}
		}
	}
	
};

#endif