
#include <iostream>

void change_a(int& a) {
	a = 20;
}

int main_old() {
	int a = 10;
	change_a(a);
	std::cout << a << '\n';
	return 0;
}

#feature on safety
#include "../vendor/std2.h"

void change_a(int^ a) safe {
	*a = 20;
}

int main() safe {
	int a = 10;
	change_a(^a);
	std2::println(a);
	return 0;
}
