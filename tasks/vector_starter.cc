#include <iostream>
#include <vector>

void reverse_vector(std::vector<int>& vec) {
	int i = 0, j = (int)vec.size() - 1;
	while (i < j) {
		int tmp = vec[i];
		vec[i] = vec[j];
		vec[j] = tmp;

		++i;
		--j;
	}
}

#feature on safety
#include "../vendor/std2.h"

// Starter code
void reverse_vector(std2::vector<int>^ vec) safe {
	int i = 0, j = (int)vec.size() - 1;
	while (i < j) {
		int tmp = vec[i];
		vec[i] = vec[j];
		vec[j] = tmp;

		++i;
		--j;
	}
}

void check(const std2::vector<int>^ vec, std2::vector<int> truth) safe {
	std2::assert(vec.size() == truth.size(), "Sizes are not equal");
	for (size_t i = 0; i < vec.size(); ++i)
		std2::assert(vec[i] == truth[i], "Entries are not equal");
}

int main() safe {
	std2::vector<int> hello { 1, 2, 5, 6 };
	reverse_vector(^hello);

	check(hello, { 6, 5, 2, 1 });

	hello = {};
	reverse_vector(^hello);

	check(hello, {});
	
	hello = { 1 };
	reverse_vector(^hello);

	check(hello, { 1 });

	hello = { 1, 2, 3 };
	reverse_vector(^hello);

	check(hello, { 3, 2, 1 });
}
