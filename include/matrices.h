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
		
		void fill(size_t fillSize, const T& value = 0);
		
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
	void Matrix<T>::fill(size_t fillSize, const T& value)
	{
		mat.resize(fillSize);
		for (int i = 0; i < fillSize; ++i)
		{
			mat[i].resize(fillSize);
			for (int j = 0; j < fillSize; ++j)
			{
				mat[i][j] = value;
			}
		}
		_rows = _cols = fillSize;
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
				std::cout << m[i][j];
				//for (size_t k = 0, s1 = m[i][j].rows(); k < s1; ++k)
				//{
				//	for (size_t l = 0, s2 = m[i][j].cols(); l < s2; ++l)
				//	{
				//		o << m[i][j][k][l] << " ";	
				//	}
				//}
				std::cout << '\n';
			}
		}

		return o;
	}

	template<class T>
	void naiveMult(const Matrix<T>& a, const Matrix<T>& b, Matrix<T>& c)
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


	template<class T>
	void simpleBlockedMult(const BlockedMatrix<T>& a, const BlockedMatrix<T>& b, BlockedMatrix<T>& c)
	{
		assert(a.cols() == b.rows());

		size_t n = a.rows(), m = a.cols(), l = b.rows();
		for (size_t i = 0; i < n; ++i)
		{
			for (size_t j = 0; j < m; ++j)
			{
				for (size_t k = 0; k < l; ++k)
				{					
					naiveMult(a[i][k], b[k][j], c[i][j]);
				}
			}
		}
	}
	
	template<class T>
	void toBlocked(Matrix<T>& m, BlockedMatrix<T>& o) 
	{	
		int size = m.rows(), blockSize = o.getBlockSize();
		int numBlocks = size / blockSize;

		for (int i = 0; i < numBlocks; ++i)
		{
			for (int j = 0; j < numBlocks; ++j)
			{
				for (int k = 0; k < blockSize; ++k)
				{
					for (int l = 0; l < blockSize; ++l)
					{
						o[i][j][k][l] = m[i*blockSize + k][j*blockSize + l];
					}
				}
			}
		}
	}

	template<class T>
	void toSimpleMatrix(BlockedMatrix<T>& m, Matrix<T>& o) 
	{	
		int size = o.rows(), blockSize = m.getBlockSize();
		int numBlocks = size / blockSize;

		for (int i = 0; i < numBlocks; ++i)
		{
			for (int j = 0; j < numBlocks; ++j)
			{
				for (int k = 0; k < blockSize; ++k)
				{
					for (int l = 0; l < blockSize; ++l)
					{
						o[i*blockSize + k][j*blockSize + l] = m[i][j][k][l];
					}
				}
			}
		}
	}

};

#endif
