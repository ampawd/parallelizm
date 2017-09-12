#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include "../include/max_concurrent.h"


namespace parallelizm 
{
	void max_el(
		std::vector<int>& v,
		std::vector<int>::value_type& max, 
		const int& n_threads,
		const unsigned int& tid)
	{
		max = v[tid];
		for (size_t i = tid, end = v.size(); i < end; i += n_threads)
		{
			if (v[i] > max)
			{
				max = v[i];
			}
		}
	}

	std::vector<int>::value_type max_el_concurrent(std::vector<int>& v)
	{
		int n_threads = std::thread::hardware_concurrency();
		std::vector<std::thread> workers(n_threads);
		std::vector<int> res(n_threads);

		for (size_t i = 0; i < n_threads; ++i)
		{
			workers[i] = std::thread(max_el, std::ref(v), std::ref(res[i]), n_threads, i);
		}

		for (auto& worker: workers)
		{
			worker.join();
		}


		std::vector<int>::value_type final_max;
		max_el(std::ref(res), std::ref(final_max));
		return final_max;
	}


	std::vector<int>::value_type max_el_sequential(std::vector<int>& v)
	{
		std::vector<int>::value_type max;
		max_el(v, max);
		return max;
	}


};