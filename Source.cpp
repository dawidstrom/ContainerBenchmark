#include <iostream>
#include <list>
#include <array>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <math.h>
#include <functional>


template <std::size_t SIZE>
void benchmarkFunction(std::array<int, SIZE>& b)
{
	auto start = std::chrono::high_resolution_clock::now();

	std::sort(b.begin(), b.end());

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;

	std::cout << elapsed.count() << std::endl;
}

int main(int argc, char** argv)
{
	const int size = 2 << 10;

	std::list<int> a;
	std::vector<int> c;
	std::array<int, size> b;

	// Populate the collections with random numbers.
	for (int i = 0; i < size; i++) {
		int tmp = rand();
		a.push_back(tmp);
		c.push_back(tmp);
		b[i] = tmp;
	}

	benchmarkFunction(b);

	return 0;
}