#include "doors.h"
#include <iostream>

using namespace calendar;

template<int DOOR>
constexpr void run_single() {
	std::cout << "Door " << DOOR << ":\n";
	std::cout << "Puzzle 1: ";
	first<DOOR>();
	std::cout << "Puzzle 2: ";
	second<DOOR>();
	std::cout << '\n';
}

template<int Start, int End>
constexpr void constexpr_run_all() {
	if constexpr (Start < End) {
		run_single<Start>();
		constexpr_run_all<Start + 1, End>();
	}
}

int main() {
	constexpr int DOOR{ 9 };
	//run_single<DOOR>();
	
	constexpr_run_all<1, DOOR + 1>();

	return 0;
}

