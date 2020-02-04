#include <iostream>
#include <list>
#include <array>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <math.h>
#include <functional>
#include <chrono>
#include <thread>


// Benchmark time to execute the given function.
template<typename ... Args>
void benchmarkFunction(std::function<void(Args ...)> customFunction)
{
	auto start = std::chrono::high_resolution_clock::now();

	customFunction(Args ...);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;

	std::cout << elapsed.count() << std::endl;
}

inline size_t getSortedPosition(std::vector<int>& c, int random)
{
	// Check edge cases.
	if (random <= c[0])
	{
		return 0;
	}
	else if (random >= c[c.size()-1]) {
		return c.size();
	}
	else
	{
		size_t left = 0;
		size_t right = c.size() - 1;
		size_t midpoint;

		// While the left and right bounds aren't tight.
		while (left != right-1)
		{
			midpoint = left + (right - left) / 2;
			if (random < c[midpoint])
			{
				// We can discard all elements right of the midpoint for being too large.
				right = midpoint;
			}
			else if (random >= c[midpoint])
			{
				// We can discard all elements left of the midpoint for being too small.
				left = midpoint;
			}
			else
			{
				throw "This shouldn't be possible! Vector isn't sorted or is malformed!";
			}
		}
		return right;
	}
}

// Insert new value into sorted vector.
inline void shiftInsert(std::vector<int>& c, int random)
{
	size_t shiftIndex = getSortedPosition(c, random);

	// Insert new value at index and shift old values to keep sorted order.

	if (shiftIndex == c.size())
	{
		c.push_back(random);
	}
	else
	{
		int tmp = c[shiftIndex];
		int tmp2;
		c[shiftIndex] = random;

		for (size_t i = shiftIndex; i < c.size()-1; i++)
		{
			tmp2 = c[i+1];
			c[i+1] = tmp;
			tmp = tmp2;
		}
		c.push_back(tmp);
	}
}

// Benchmark the time it takes to insert 'size' number of values 
// while preserving the sorted order of the vector at all times.
void benchmarkVector(std::vector<int>& c, const size_t size)
{
	std::vector<int> random_feed(size);
	for (size_t i = 0; i < size; i++)
	{
		random_feed.push_back(rand());
	}

	std::function<void()> myfunc = [&c,size,random_feed]() {
		for (size_t i = 0; i < size; i++)
		{
			shiftInsert(c, random_feed[i]);
		}
	};

	benchmarkFunction(myfunc);
}

// Benchmark the time it takes to insert 'size' number of values 
// while preserving the sorted order of the list at all times.
void benchmarkList(std::list<int>& c, const size_t size)
{
	std::vector<int> random_feed(size);
	for (size_t i = 0; i < size; i++)
	{
		random_feed.push_back(rand());
	}

	// Insert and retain sorted order of list.
	std::function<void()> myfunc = [&c,size,random_feed]() {
		for (size_t i = 0; i < size; i++)
		{
			// Find index where to place new value.
			std::list<int>::iterator it;
			for (it=c.begin(); it != c.end(); ++it)
			{
				if (random_feed[i] < *it) {
					c.insert(it, random_feed[i]);
					break;
				}
			}
		}
	};

	benchmarkFunction(myfunc);
}


int main(int argc, char** argv)
{
	const int size = 2 << 10;
	std::cout << "Size: " << size << std::endl;

	std::list<int> a;
	std::vector<int> b;

	// Populate the collections with random numbers.
	for (int i = 0; i < size; i++) {
		int tmp = rand();
		a.push_back(tmp);
		b.push_back(tmp);
	}

	// Impose a sorted order.
	a.sort();
	std::cout << "List sorted..." << std::flush;
	std::sort(b.begin(), b.end());
	std::cout << "Vector sorted..." << std::flush;
	std::cout << "starting Benchmark..." << std::endl;

	benchmarkList(a, 0.3*size);
	benchmarkVector(b, 0.3*size);

	return 0;
}