#include <iostream>
#include <thread>
#include <vector>

#include "include/linear.h"
#include "include/profiler.h"

using parallelizm::profile;


int main()
{	
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
	std::vector<long long> v;
	v.reserve(NUM_ELEMENTS + 100);
	for (unsigned i = 0; i < NUM_ELEMENTS; ++i)
	{
		v.push_back( rand() % 15 );
	}
	
	std::cout << "sequential run ... " << std::endl;
	long long sum = 0;
	profile([&]()
		{ 
			return parallelizm::sum_sequential(v, sum, 0, v.size());
		},
		sum
	);

	std::cout << "running with " << std::thread::hardware_concurrency() << " threads ... " << std::endl;
	profile([&]()
		{
			return parallelizm::sum_concurrent(v);
		},
		sum
	);



	return 0;
}