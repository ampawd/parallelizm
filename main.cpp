#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std::chrono;

void max_el(
	std::vector<int>& v,
	std::vector<int>::value_type& max, 
	const int& n_threads=1,
	const unsigned int& tid = 0)
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


template< class Func, class Container, class ResultType >
void profile(Func func, Container cont, ResultType res)
{
	high_resolution_clock::time_point start, now;
	double runtime = 0.0f;

	start = high_resolution_clock::now();
	res = func(cont);
	now = high_resolution_clock::now();
	runtime = duration<double>(now - start).count();
	std::cout << "runing time = " << runtime << " sec" << std::endl;
	std::cout << res << std::endl;
}


#define NUM_ELEMENTS 50000000


int main()
{
	std::vector<int> v;// = { 10, 2, 3, 3, 12, -1 };
	v.reserve(NUM_ELEMENTS + 100);
	for (unsigned i = 0; i < NUM_ELEMENTS; ++i)
	{
		v.push_back( rand() % 100 );
	}
	std::vector<int>::value_type res = 0;

	std::cout << "data is ready, running ... " << std::endl;
	std::cout << "sequential" << std::endl;
	profile(max_el_sequential, v, res);

	std::cout << std::thread::hardware_concurrency() << " threads" << std::endl;
	profile(max_el_concurrent, v, res);

	return 0;
}