#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include "../include/simple.h"


namespace parallelizm 
{
	std::vector<long long>::value_type sum_sequential(std::vector<long long>& v, std::vector<long long>::value_type& res,
		size_t start, size_t end)
	{
		res = std::accumulate(v.begin() + start, v.begin() + end, 0);
		return res;
	}
	
	std::vector<long long>::value_type sum_concurrent(std::vector<long long>& v)
	{
		typedef std::vector<long long>::value_type ResType;

		int n_threads = std::thread::hardware_concurrency();
		size_t chunk_size = v.size() / n_threads;
		int s = 0, e = chunk_size;
		std::vector<long long>::value_type all_sums = 0;
		std::vector<ResType> sums(n_threads);	

		std::vector<std::thread> workers(n_threads);
		for (int i = 0; i < n_threads; ++i)
		{
			workers[i] = std::thread(sum_sequential, std::ref(v), std::ref(sums[i]), s, e);
			s += chunk_size;
			e += chunk_size;			
		}

		for (int i = 0; i < n_threads; ++i)
		{
			workers[i].join();
		}

		sum_sequential(sums, all_sums, 0, sums.size());

		ResType last_sum = 0;
		if (e - chunk_size < v.size())
		{
			sum_sequential(v, last_sum, e - chunk_size, v.size());
		}

		return all_sums + last_sum;
	}

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
