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
#include <optional>

#define DEBUG false
#define LOG(a) if (DEBUG) { a; }

// Benchmark time to execute the given function.
template<typename ... Args>
double benchmarkFunction(std::function<void(Args ...)> customFunction) {
	auto start = std::chrono::high_resolution_clock::now();

	customFunction(Args ...);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;

	return elapsed.count();
}

inline size_t getSortedPosition(std::vector<int>& c, int random) {
	// Check edge cases.
	if (random <= c[0]) {
		return 0;
	} else if (random >= c[c.size()-1]) {
		return c.size();
	} else {
		size_t left = 0;
		size_t right = c.size() - 1;
		size_t midpoint;

		// While the left and right bounds aren't tight.
		while (left != right-1) {
			midpoint = left + (right - left) / 2;
			if (random < c[midpoint]) {
				// We can discard all elements right of the midpoint for being too large.
				right = midpoint;
			} else if (random >= c[midpoint]) {
				// We can discard all elements left of the midpoint for being too small.
				left = midpoint;
			} else {
				throw "This shouldn't be possible! Vector isn't sorted or is malformed!";
			}
		}
		return right;
	}
}

// Insert new value into sorted vector.
inline void shiftInsert(std::vector<int>& c, int random) {
	size_t shiftIndex = getSortedPosition(c, random);

	// Insert new value at index and shift old values to keep sorted order.

	if (shiftIndex == c.size()) {
		c.push_back(random);
	} else {
		c.push_back(0);
		std::rotate(c.begin()+shiftIndex, c.begin() + shiftIndex+1, c.end());
		c[shiftIndex] = random;
	}
}

// Benchmark the time it takes to insert 'size' number of values 
// while preserving the sorted order of the vector at all times.
double benchmarkVector(std::vector<int>& c, const size_t size) {
	std::vector<int> random_feed;

	for (size_t i = 0; i < size; i++) {
		random_feed.push_back(rand());
	}

	std::function<void()> myfunc = [&c,size,random_feed]() {
		for (size_t i = 0; i < size; i++) {
			shiftInsert(c, random_feed[i]);
		}
	};

	return benchmarkFunction(myfunc);
}

// Benchmark the time it takes to insert 'size' number of values 
// while preserving the sorted order of the list at all times.
double benchmarkList(std::list<int>& c, const size_t size) {
	std::vector<int> random_feed(size);

	for (size_t i = 0; i < size; i++) {
		random_feed.push_back(rand());
	}

	// Insert and retain sorted order of list.
	std::function<void()> myfunc = [&c,size,random_feed]() {
		for (size_t i = 0; i < size; i++) {
			// Find index where to place new value.
			std::list<int>::iterator it;
			for (it=c.begin(); it != c.end(); ++it) {
				if (random_feed[i] < *it) {
					c.insert(it, random_feed[i]);
					break;
				}
			}
		}
	};

	return benchmarkFunction(myfunc);
}

// Check that elements in collection are in ascending sorted order.
template<typename T>
bool isSorted(T start, T end) {
	int prev = *start;

	for (T i = start; i != end; i++) {
		if (*i < prev) { // Element i should come before previous element, collection isnt sorted!
			return false;
		} else {
			prev = *i;
		}
	}
	return true;
}

struct ARGS {
	size_t max, min;
	double rate;
};

std::optional<ARGS> parseArgs(size_t argc, char** argv) {

	ARGS args;
	if (argc != 4) {
		std::cout << "Not enough parameters!\nRequired parameters: min max rate\nWhere min and max refer to 2^min and 2^max." << std::endl;
		return std::nullopt;
	}

	args.min = std::atoi(argv[1]);
	args.max = std::atoi(argv[2]);
	args.rate = std::atof(argv[3]);

	return std::optional<ARGS>(args);
}

int main(const size_t argc, char** argv) {
	std::optional<ARGS> args = parseArgs(argc, argv);

	if (!args) {
		return 1;
	}

	for (size_t i = args->min; i < args->max; i++) {
		size_t size = 2 << i;

		//std::cout << "Starting size: " << size << ", inserting: " << size_t(size * args->rate) << " new elements. " << std::endl;
		std::list<int> a;
		std::vector<int> b;

		// Populate the collections with random numbers.
		for (size_t j = 0; j < size; j++) {
			int tmp = rand();
			a.push_back(tmp);
			b.push_back(tmp);
		}

		// Impose a sorted order.
		a.sort();
		LOG(std::cout << "List sorted..." << std::flush);
		std::sort(b.begin(), b.end());
		LOG(std::cout << "Vector sorted..." << std::flush);
		LOG(std::cout << "starting Benchmark..." << std::endl);

		
		//std::cout << "List: ";
		double elapsed = benchmarkList(a, args->rate * size);
		std::cout << elapsed << std::endl;

		//std::cout << "Vector: ";
		elapsed = benchmarkVector(b, args->rate * size);
		std::cout << elapsed << std::endl;

		
		if (isSorted<std::list<int>::iterator>(a.begin(), a.end())) {
			LOG(std::cout << "Vector is sorted!" << std::endl);
		} else {
			LOG(std::cout << "Vector is NOT sorted!" << std::endl);
		}
		if (isSorted<std::vector<int>::iterator>(b.begin(), b.end())) {
			LOG(std::cout << "Vector is sorted!" << std::endl);
		} else {
			LOG(std::cout << "Vector is NOT sorted!" << std::endl);
		}

		//std::cout << std::endl;
	}

	return 0;
}