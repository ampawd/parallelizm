#include <iostream>
#include <thread>
#include <vector>


#include "include/linear.h"

#include "include/matrices.h"
#include "include/profiler.h"

using parallelizm::profile;
using parallelizm::Matrix;
using parallelizm::BlockedMatrix;



int main()
{

	Matrix<int> a({	{2, 1, 5, 3},
					{0, 1, 2, -1},
					{1, 0, -2, 2},
					{1, 1, 1, 3}}
				);


	Matrix<int> b({	{6, 0, 1, 2},
					{1, 0, 1, -1},
					{2, 1, -1, -2},
					{3, 3, 2, 2}} 
				);

	//	NxM * MxK = NxK
	Matrix<int> c(a.rows(), b.cols());
	


	profile([&]() {
		parallelizm::naive_mult(a, b, c);
		return c;
	}, c);


	BlockedMatrix<int> a2(2, 2, 2);
	a2[0][0] = Matrix<int>({{2, 1},	{0, 1}});	
	a2[0][1] = Matrix<int>({{5, 3}, {2, -1}});
	a2[1][0] = Matrix<int>({{1, 0}, {1, 1}});
	a2[1][1] = Matrix<int>({{-2, 2}, {1, 3}});


	BlockedMatrix<int> b2(2, 2, 2);
	b2[0][0] = Matrix<int>({{6, 0},	{1, 0}});	
	b2[0][1] = Matrix<int>({{1, 2}, {1, -1}});
	b2[1][0] = Matrix<int>({{2, 1}, {3, 3}});
	b2[1][1] = Matrix<int>({{-1, -2}, {2, 2}});


	//	NxM * MxK = NxK
	BlockedMatrix<int> c2(a2.rows(), b2.cols(), 2);

	parallelizm::simple_blocked_mult(a2, b2, c2);
	
	std::cout << c2 << '\n';



	



	//	computing max element
	// std::vector<int> v;// = { 10, 2, 3, 3, 12, -1 };
	// v.reserve(NUM_ELEMENTS + 100);
	// for (unsigned i = 0; i < NUM_ELEMENTS; ++i)
	// {
	// 	v.push_back( rand() % 100 );
	// }

	// std::vector<int>::value_type res = 0;
	// std::cout << "data is ready, running ... " << std::endl;
	// std::cout << "sequential" << std::endl;
	// profile(parallelizm::max_el_sequential, v, res);

	// std::cout << std::thread::hardware_concurrency() << " threads" << std::endl;
	// profile(parallelizm::max_el_concurrent, v, res);



	//	computing sum element
	//	uncomment to test
	// std::vector<long long> v;
	// v.reserve(NUM_ELEMENTS + 100);
	// for (unsigned i = 0; i < NUM_ELEMENTS; ++i)
	// {
	// 	v.push_back( rand() % 15 );
	// }
	
	// std::cout << "sequential run ... " << std::endl;
	// long long sum = 0;
	// profile([&]()
	// 	{ 
	// 		return parallelizm::sum_sequential(v, sum, 0, v.size());
	// 	},
	// 	sum
	// );

	// std::cout << "running with " << std::thread::hardware_concurrency() << " threads ... " << std::endl;
	// profile([&]()
	// 	{
	// 		return parallelizm::sum_concurrent(v);
	// 	},
	// 	sum
	// );



	return 0;
}