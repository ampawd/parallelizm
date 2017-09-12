#ifndef _MAX_CONCURRENT_H
#define _MAX_CONCURRENT_H

#include <vector>


namespace parallelizm 
{
	#define NUM_ELEMENTS 50000000

	void max_el(std::vector<int>& v, std::vector<int>::value_type& max, const int& n_threads=1, const unsigned int& tid = 0);

	std::vector<int>::value_type max_el_concurrent(std::vector<int>& v);

	std::vector<int>::value_type max_el_sequential(std::vector<int>& v);

};

#endif