#include "doors.h"

using namespace calendar;

template<int DOOR>
void run_single() {
	first<DOOR>();
	second<DOOR>();
}

template<int Start, int End>
constexpr void constexpr_run_all() {
	if constexpr (Start < End) {
		first<Start>();
		second<Start>();
		constexpr_run_all<Start - 1, End>();
	}
}

int main() {
	constexpr int DOOR{ 8 };
	//run_single<DOOR>();
	
	constexpr_run_all<1, DOOR + 1>();

	return 0;
}

