#ifndef _MATRICES_H
#define _MATRICES_H

#include <vector>
#include <iostream>
#include <initializer_list>
#include <algorithm>

#include <thread>
#include <cassert>

namespace parallelizm 
{
	#define MATRIX_ELEMENT_UPPER_BOUND 100

	template<class T>
		class BlockedMatrix;
	
	template<class T>
		class Matrix;


	template<class T>
		void toSimpleMatrix(const BlockedMatrix<T>&, Matrix<T>&); 
	
	template<class T>
		void toBlocked(const Matrix<T>&, BlockedMatrix<T>&);

	template<class T>
		class Matrix
		{
			private:
				std::vector< std::vector<T> > mat;
				size_t m_rows, m_cols;

			public:
				typedef T ElementType;
				Matrix(size_t rows=0, size_t cols=0);
				Matrix(const BlockedMatrix<T>&);
				Matrix(std::initializer_list< std::initializer_list<T> >&& );
				void initWithZeros(size_t, size_t);
				std::vector<T>& operator [] (size_t);
				const std::vector<T>& operator [] (size_t) const;
				bool operator == (const Matrix&) const;	
				bool operator != (const Matrix&) const;	
			template<class U>
				friend std::ostream& operator << (std::ostream&, const Matrix<U>&);
				
				size_t rows() const;
				size_t cols() const;
		};


	template<class T>
		Matrix<T>::Matrix(size_t rows, size_t cols): 
			m_rows(rows),
			m_cols(cols),
			mat(rows, std::vector<T>(cols))
	{}
	
	template<class T>
		Matrix<T>::Matrix(const BlockedMatrix<T>& m) 
	{
		m_rows = m.rows() * m.getBlockSize();
		m_cols = m.cols() * m.getBlockSize();
		initWithZeros(m_rows, m_cols);	
		toSimpleMatrix(m, *this);	
	}

	template<class T>
		Matrix<T>::Matrix(std::initializer_list< std::initializer_list<T> >&& list)
		{
			mat.assign(list.begin(), list.end());
			m_rows = mat.size();
			m_cols = mat[0].size();
		}

	template<class T>
		void Matrix<T>::initWithZeros(size_t fillRows, size_t fillCols)
		{
			mat.resize(fillRows);
			for (size_t i = 0; i < fillRows; i++)
			{
				mat[i].resize(fillCols);
			}
			m_rows = fillRows;
			m_cols = fillCols;
		}


	template<class T>
		std::vector<T>& Matrix<T>::operator [] (size_t i)
		{
			return mat.at(i);
		}

	template<class T>
		const std::vector<T>& Matrix<T>::operator [] (size_t i) const
		{
			return mat.at(i);
		}

	template<class T>
		bool Matrix<T>::operator == (const Matrix& other) const
		{
			assert(rows() == other.rows() && cols() == other.cols());
			for (size_t i = 0; i < rows(); i++)
			{
				for (size_t j = 0; j < cols(); j++)
				{
					if (other[i][j] != mat[i][j]) return false;
				}
			}
			
			return true;
		}
	
	template<class T>
		bool Matrix<T>::operator != (const Matrix& rhs) const
		{
			return !(*this == rhs);
		}

	template<class T>
		size_t Matrix<T>::rows() const
		{
			return m_rows;
		}

	template<class T>
		size_t Matrix<T>::cols() const
		{
			return m_cols;
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
				size_t m_blockSize;
				size_t m_rows, m_cols;

			public:
				typedef T ElementType;
				BlockedMatrix(size_t=0, size_t=0, size_t=0);
				BlockedMatrix(const Matrix<T>&, size_t);
				size_t getBlockSize() const;
				std::vector<Matrix<T>>& operator [] ( size_t );
				const std::vector<Matrix<T>>& operator [] ( size_t ) const;

			template<class U>
				friend std::ostream& operator << (std::ostream&, const BlockedMatrix<U>&);
				size_t rows() const;
				size_t cols() const;
		};

	template<class T>
		BlockedMatrix<T>::BlockedMatrix(size_t rows, size_t cols, size_t blockSize): 
			m_rows(rows),
			m_cols(cols),
			m_blockSize(blockSize),
			mat(rows, std::vector<Matrix<T>>(cols))
	{
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < cols; ++j)
			{
				mat[i][j].initWithZeros(blockSize, blockSize);
			}
		}
	}

	template<class T>
		BlockedMatrix<T>::BlockedMatrix(const Matrix<T>& m, size_t blockSize) 
	{
		m_rows = m.rows()/blockSize;
		m_cols = m.cols()/blockSize;
		m_blockSize = blockSize;
		mat.resize(m_rows);	
		for (size_t i = 0; i < m_rows; ++i)
		{
			mat[i].resize(m_cols);
			for (size_t j = 0; j < m_cols; ++j)
			{
				mat[i][j].initWithZeros(blockSize, blockSize);
			}
		}
		toBlocked(m, *this);
	}

	template<class T>
		size_t BlockedMatrix<T>::getBlockSize() const
		{
			return m_blockSize;
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
			return m_rows;
		}

	template<class T>
		size_t BlockedMatrix<T>::cols() const
		{
			return m_cols;
		}

	template<class T>
		std::ostream& operator << (std::ostream& o, const BlockedMatrix<T>& m)
		{
			size_t rows = m.rows(), cols = m.cols();
			for (size_t i = 0; i < m.mat.size(); ++i)
			{
				for (size_t j = 0; j < m.mat[0].size(); ++j)
				{
					std::cout << m[i][j];
					std::cout << '\n';
				}
			}

			return o;
		}

	template<class T>
		void naiveMult(const Matrix<T>& a, const Matrix<T>& b, Matrix<T>& c)
		{
			assert(a.cols() == b.rows());
			size_t n = a.rows(), m = b.cols(), l = a.cols();
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
		void naiveMult(const BlockedMatrix<T>& a, const BlockedMatrix<T>& b, BlockedMatrix<T>& c)
		{
			assert(a.cols() == b.rows());

			size_t n = a.rows(), m = b.cols(), l = a.cols();
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
		void simpleConcurrentMult(const BlockedMatrix<T>& a, const BlockedMatrix<T>& b, BlockedMatrix<T>& c)
		{
			assert(a.cols() == b.rows());

			size_t n_threads = std::thread::hardware_concurrency();
			std::vector<std::thread> workers(n_threads);
			size_t subRows = a.rows()/n_threads;
						
			for (size_t i = 0; i < n_threads; ++i)
			{

				workers[i] = std::thread( [&](size_t tid) {
					for (size_t i = tid*subRows; i < (tid + 1)*subRows; i++)
					{
						for (size_t j = 0; j < b.cols(); j++)
						{
							for (size_t k = 0; k < a.cols(); k++)
							{
								naiveMult(a[i][k], b[k][j], c[i][j]);
							}
						}						
					}

				}, i );
			
			}	
			
			if (a.rows() % n_threads)
			{
				for (size_t i = subRows*n_threads; i < a.rows(); i++)
				{
					for (size_t j = 0; j < b.cols(); j++)
						for (size_t k = 0; k < a.cols(); k++)
							naiveMult(a[i][k], b[k][j], c[i][j]);
				}
			}
			
			for (size_t i = 0; i < n_threads; ++i)
			{
				workers[i].join();
			}	
		}


	template<class T>
		void toBlocked(const Matrix<T>& m, BlockedMatrix<T>& o) 
		{
			assert(o.rows() != 0 && o.cols() != 0);	
			size_t blockSize = o.getBlockSize(),
			       numBlocksVert = m.rows() / blockSize, 
			       numBlocksHor = m.cols() / blockSize;
			for (size_t i = 0; i < numBlocksVert; ++i)
			{
				for (size_t j = 0; j < numBlocksHor; ++j)
				{
					for (size_t k = 0; k < blockSize; ++k)
					{
						for (size_t l = 0; l < blockSize; ++l)
						{
							o[i][j][k][l] = m[i*blockSize + k][j*blockSize + l];
						}
					}
				}
			}
		}

	template<class T>
		void toSimpleMatrix(const BlockedMatrix<T>& m, Matrix<T>& o) 
		{	
			assert(o.rows() != 0 && o.cols() != 0);	
			size_t blockSize = m.getBlockSize(),
			       numBlocksVert = o.rows() / blockSize,
			       numBlocksHor = o.cols() / blockSize;

			for (size_t i = 0; i < numBlocksVert; ++i)
			{
				for (size_t j = 0; j < numBlocksHor; ++j)
				{
					for (size_t k = 0; k < blockSize; ++k)
					{
						for (size_t l = 0; l < blockSize; ++l)
						{
							o[i*blockSize + k][j*blockSize + l] = m[i][j][k][l];
						}
					}
				}
			}
		}

};

#endif
