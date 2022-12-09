#include "doors.h"
#include "input.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>

namespace calendar {
	void solve_puzzle(std::function<bool(std::pair<int, int>&, std::pair<int, int>&)> is_overlap) {
		std::ifstream inf{ get_input_stream(4) };
		if (!inf) { return; }

		int fully_contained_count{ 0 };
		while (inf) {
			std::string curr_line{};
			std::getline(inf, curr_line);
			if (curr_line.length() == 0) { continue; }

			std::istringstream line_stream{ curr_line };
			std::pair<int, int> first_interval{};
			std::pair<int, int> second_interval{};
			char skip_dash{};
			char skip_comma{};
			line_stream >> first_interval.first >> skip_dash >> first_interval.second >> skip_comma;
			line_stream >> second_interval.first >> skip_dash >> second_interval.second;

			if (is_overlap(first_interval, second_interval)) { fully_contained_count++; }
		}

		std::cout << fully_contained_count << '\n';
	}

	bool inclusion_overlap(std::pair<int, int>& first_interval, std::pair<int, int>& second_interval) {
		return first_interval.first == second_interval.first ||
			first_interval.second == second_interval.second ||
			(first_interval.first < second_interval.first&& first_interval.second > second_interval.second) ||
			(first_interval.first > second_interval.first && first_interval.second < second_interval.second);
	}

	bool any_overlap(std::pair<int, int>& first_interval, std::pair<int, int>& second_interval) {
		return inclusion_overlap(first_interval, second_interval) ||
			(first_interval.first <= second_interval.first&& first_interval.second >= second_interval.first) ||
			(first_interval.first <= second_interval.second&& first_interval.second >= second_interval.second);
	}

	template<>
	void first<4>() {
		solve_puzzle(inclusion_overlap);
	}

	template<>
	void second<4>() {
		solve_puzzle(any_overlap);
	}
}

