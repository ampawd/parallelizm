#include <iostream>
#include <thread>
#include <vector>
#include <fstream>

#include "include/simple.h"

#include "include/matrices.h"
#include "include/profiler.h"

using parallelizm::profile;
using parallelizm::Matrix;
using parallelizm::BlockedMatrix;

int main()
{

	Matrix<int> m1({
							{1, 2, 0, 1,  0, 2, 3, 4,  2, 9, 5, 4,  1, 1, 2, 2},
							{0, 1, 2, 3,  1, 2, 3, 4,  2, 1, 1, 3,  2, 2, 4, 5},
							{2, 1, 2, 3,  1, 2, 1, 1,  0, 9, 0, 2,  1, 1, 2, 3},
							{2, 1, 2, 1,  1, 2, 3, 4,  2, 1, 1, 3,  2, 2, 4, 5},
							
							{9, 1, 2, 2,  1, 2, 1, 1,  0, 9, 0, 2,  1, 1, 2, 3},
							{0, 1, 2, 1,  1, 2, 3, 4,  2, 1, 1, 3,  2, 2, 4, 5},
							{0, 1, 2, 1,  1, 6, 3, 4,  2, 1, 1, 3,  2, 2, 4, 5},
							{9, 1, 2, 4,  1, 3, 1, 1,  0, 9, 0, 2,  1, 1, 2, 3},
							
							{9, 1, 2, 3,  1, 2, 1, 1,  0, 9, 0, 2,  1, 1, 2, 3},
							{0, 1, 2, 3,  1, 1, 3, 4,  2, 1, 1, 3,  2, 2, 4, 5},
							{4, 1, 2, 3,  1, 0, 2, 4,  1, 1, 3, 3,  2, 8, 4, 5},
							{2, 1, 2, 3,  1, 2, 3, 4,  5, 1, 1, 3,  2, 2, 4, 5},
							
							{9, 1, 3, 3,  3, 0, 1, 1,  8, 9, 0, 2,  1, 1, 2, 3},
							{8, 1, 1, 3,  5, 0, 2, 1,  6, 9, 0, 2,  1, 5, 2, 3},
							{1, 1, 5, 3,  7, 2, 3, 1,  0, 9, 0, 2,  1, 1, 2, 3},
							{2, 1, 9, 3,  2, 3, 4, 1,  0, 9, 0, 2,  1, 1, 2, 3}
							});


	Matrix<int> m2({
							{2, 2, 0, 1,  0, 2, 3, 4,  2, 9, 5, 4,  1, 1, 2, 2},
							{3, 1, 2, 4,  2, 2, 3, 4,  2, 1, 1, 3,  2, 2, 4, 5},
							{1, 1, 5, 2,  6, 2, 1, 1,  0, 9, 0, 2,  1, 1, 2, 3},
							{4, 1, 2, 1,  7, 2, 3, 4,  2, 1, 1, 3,  2, 2, 4, 5},
							
							{9, 1, 2, 2,  6, 2, 1, 1,  0, 9, 0, 2,  1, 1, 2, 3},
							{0, 1, 2, 1,  3, 2, 3, 4,  2, 1, 1, 3,  6, 2, 4, 8},
							{0, 1, 2, 1,  1, 2, 7, 8,  4, 8, 9, 4,  5, 2, 1, 4},
							{9, 1, 2, 4,  4, 3, 1, 1,  0, 9, 0, 2,  1, 1, 2, 2},
							
							{9, 1, 2, 3,  6, 2, 1, 2,  0, 9, 0, 2,  1, 1, 2, 3},
							{0, 1, 2, 3,  1, 3, 3, 6,  2, 1, 1, 1,  2, 2, 4, 5},
							{4, 1, 2, 9,  1, 8, 2, 4,  1, 1, 3, 7,  2, 8, 4, 5},
							{2, 1, 2, 0,  3, 5, 3, 4,  5, 1, 1, 9,  9, 2, 4, 5},
							
							{1, 1, 3, 1,  3, 5, 4, 2,  8, 9, 0, 2,  0, 1, 2, 3},
							{2, 6, 7, 2,  5, 0, 1, 3,  6, 9, 0, 2,  5, 5, 2, 3},
							{5, 2, 5, 3,  7, 2, 3, 5,  0, 9, 0, 2,  3, 1, 2, 2},
							{2, 3, 9, 3,  2, 3, 4, 3,  0, 9, 0, 2,  1, 8, 4, 1}
							});

	Matrix<int> m3(m1.cols(), m2.rows());	
	naiveMult(m1, m2, m3);
	std::cout << m3 << '\n';


	BlockedMatrix<int> a(m1.rows()/4, m1.cols()/4, 4);
	BlockedMatrix<int> b(m2.rows()/4, m2.cols()/4, 4);
	BlockedMatrix<int> c(a.rows(), b.cols(), 4);

	toBlocked(m1, a);
	toBlocked(m2, b);

	simpleBlockedMult(a, b, c);
	Matrix<int> cc(c.rows()*c.getBlockSize(), c.cols()*c.getBlockSize());
	parallelizm::toSimpleMatrix(c, cc);
	std::cout << cc;


//	Matrix<int> a1({	{2, 1, 5, 3},
//					{0, 1, 2, -1},
//					{1, 0, -2, 2},
//					{1, 1, 1, 3}}
//				);
//
//
//	Matrix<int> b1({	{6, 0, 1, 2},
//					{1, 0, 1, -1},
//					{2, 1, -1, -2},
//					{3, 3, 2, 2}} 
//				);
//
//	//	NxM * MxK = NxK
//	Matrix<int> c1(a1.rows(), b1.cols());
//	profile([&]() {
//		parallelizm::naiveMult(a1, b1, c1);
//		return c;
//	}, c);
//
//
//	BlockedMatrix<int> a2(2, 2, 2);
//	a2[0][0] = Matrix<int>({{2, 1},	{0, 1}});	
//	a2[0][1] = Matrix<int>({{5, 3}, {2, -1}});
//	a2[1][0] = Matrix<int>({{1, 0}, {1, 1}});
//	a2[1][1] = Matrix<int>({{-2, 2}, {1, 3}});
//
//
//	BlockedMatrix<int> b2(2, 2, 2);
//	b2[0][0] = Matrix<int>({{6, 0},	{1, 0}});	
//	b2[0][1] = Matrix<int>({{1, 2}, {1, -1}});
//	b2[1][0] = Matrix<int>({{2, 1}, {3, 3}});
//	b2[1][1] = Matrix<int>({{-1, -2}, {2, 2}});
//
//
//	//	NxM * MxK = NxK
//	BlockedMatrix<int> c2(a2.rows(), b2.cols(), 2);
//	parallelizm::simpleBlockedMult(a2, b2, c2);
//	std::cout << c2 << '\n';
//
//
//		//	computing max element
//	std::vector<int> v;// = { 10, 2, 3, 3, 12, -1 };
//	v.reserve(NUM_ELEMENTS + 100);
//	for (unsigned i = 0; i < NUM_ELEMENTS; ++i)
//	{
//		v.push_back( rand() % 100 );
//	}
//
//	std::vector<int>::value_type res = 0;
//	std::cout << "data is ready, running ... " << std::endl;
//	std::cout << "sequential" << std::endl;
//	profile(parallelizm::max_el_sequential, v, res);
//
//	std::cout << std::thread::hardware_concurrency() << " threads" << std::endl;
//	profile(parallelizm::max_el_concurrent, v, res);
//
//
//
//	//	computing sum element
//	//	uncomment to test
//	std::vector<long long> v2;
//	v.reserve(NUM_ELEMENTS + 100);
//	for (unsigned i = 0; i < NUM_ELEMENTS; ++i)
//	{
//		v2.push_back( rand() % 15 );
//	}
//	
//	std::cout << "sequential run ... " << std::endl;
//	long long sum = 0;
//	profile([&]()
//		{ 
//			return parallelizm::sum_sequential(v2, sum, 0, v2.size());
//		},
//		sum
//	);
//
//	std::cout << "running with " << std::thread::hardware_concurrency() << " threads ... " << std::endl;
//	profile([&]()
//		{
//			return parallelizm::sum_concurrent(v2);
//		},
//		sum
//	);
//


	return 0;
}
