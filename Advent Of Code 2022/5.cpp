#include "doors.h"
#include "input.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <cassert>

namespace calendar {
	std::vector<std::vector<char>> read_stacks(std::ifstream& inf) {
		int num_stacks{ 0 };
		std::vector<std::vector<char>> stacks{};
		while (inf) {
			std::string curr_line{};
			std::getline(inf, curr_line);

			// break at the end of the stacks input
			if (curr_line.length() == 0) { break; }

			// initialize stack count
			if (num_stacks == 0) {
				if ((curr_line.length() + 1) % 4 != 0) {
					std::cerr << "Malformed input.\n";
					exit(1);
				}

				num_stacks = (static_cast<int>(curr_line.length()) + 1) / 4;
				for (int i{ 0 }; i < num_stacks; i++) {
					stacks.push_back({});
				}
			}

			// add each element of this line to the respective stack
			for (int i{ 0 }; i < num_stacks; i++) {
				char stack_element{ curr_line[i * 4 + 1] };
				if (stack_element != ' ') {
					stacks[i].push_back(stack_element);
				}
			}
		}

		for (int i{ 0 }; i < num_stacks; i++) {
			std::reverse(stacks[i].begin(), stacks[i].end()-1);
			stacks[i].pop_back();
		}
		
		return stacks;
	}

	void solve_puzzle(std::function<void(std::vector<std::vector<char>>&, int, int, int)> rearrange, std::string* result_ptr) {
		std::ifstream inf{ get_input_stream(5) };
		if (!inf) { return; }
		
		std::vector<std::vector<char>> stacks{ read_stacks(inf) };

		while (inf) {
			std::string curr_line{};
			std::getline(inf, curr_line);

			if (curr_line.length() == 0) { continue; }

			int move_count{ 0 };
			int from_stack{ 0 };
			int to_stack{ 0 };
			std::string skip{};
			std::istringstream curr_line_stream{ curr_line };
			curr_line_stream >> skip >> move_count >> skip >> from_stack >> skip >> to_stack;
			
			rearrange(stacks, move_count, from_stack, to_stack);
		}

		for (int i{ 0 }; i < static_cast<int>(stacks.size()); i++) {
			(*result_ptr).push_back(*(stacks[i].end() - 1));
		}
	}

	void rearrange_first(std::vector<std::vector<char>>& stacks, int move_count, int from_stack, int to_stack) {
		for (int i{ 0 }; i < move_count; i++) {
			stacks[to_stack - 1].push_back(*(stacks[from_stack - 1].end() - 1));
			stacks[from_stack - 1].pop_back();
		}
	}

	void rearrange_second(std::vector<std::vector<char>>& stacks, int move_count, int from_stack, int to_stack) {
		stacks[to_stack - 1].insert(stacks[to_stack - 1].end(), stacks[from_stack - 1].end() - move_count, stacks[from_stack - 1].end());
		stacks[from_stack - 1].erase(stacks[from_stack - 1].end() - move_count, stacks[from_stack - 1].end());
	}

	template<>
	void first<5>() {
		std::string result{};
		solve_puzzle(rearrange_first, &result);
		assert(result == "RFFFWBPNS");
		std::cout << result << '\n';
	}

	template<>
	void second<5>() {
		std::string result{};
		solve_puzzle(rearrange_second, &result);
		assert(result == "CQQBBJFCS");
		std::cout << result << '\n';
	}
}

