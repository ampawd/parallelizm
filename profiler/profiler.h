#include <iostream>
#include <chrono>


using namespace std::chrono;

namespace parallelizm 
{
	static double runtime = 0.0f;

	static high_resolution_clock::time_point start, end;


	template< class Func, class Container, class ResultType >
	static void profile(Func func, Container cont, ResultType& res)
	{
		std::cout << "profiling started ... " << std::endl;
		
		start = high_resolution_clock::now();
		res = func(cont);
		end = high_resolution_clock::now();
		runtime = duration<double>(end - start).count();
		
		std::cout << "elapsed time = " << runtime << " sec" << std::endl;
		std::cout << res << std::endl;
	}

	template< class Func, class ResultType >
	static void profile(Func func, ResultType& res)
	{
		std::cout << "profiling started ... " << std::endl;
		start = high_resolution_clock::now();
		res = func();
		end = high_resolution_clock::now();
		runtime = duration<double>(end - start).count();
		std::cout << "elapsed time = " << runtime << " sec" << std::endl;
		std::cout << res << std::endl;
	}

	template< class Func >
	static void profile(Func func)
	{
		std::cout << "profiling started ... " << std::endl;
		start = high_resolution_clock::now();
		func();
		end = high_resolution_clock::now();
		runtime = duration<double>(end - start).count();
		std::cout << "elapsed time = " << runtime << " sec" << std::endl;
	}
};
