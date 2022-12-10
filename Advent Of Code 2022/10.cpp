#include "doors.h"
#include "input.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

namespace calendar {
	void read_input(std::ifstream& inf, std::vector<int>& x_changes, std::vector<int>& x_values) {
		int curr_x_change{ 0 };
		while (inf) {
			std::string curr_instruction{};
			inf >> curr_instruction;
			
			if (curr_instruction.length() == 0) { continue; }
			
			x_changes.push_back(0);
			x_values.push_back(x_values.back());
			if (curr_instruction == "addx") { 
				inf >> curr_x_change;
				x_changes.push_back(curr_x_change);
				x_values.push_back(x_values.back() + curr_x_change);
			}
		}
	}

	template<>
	void first<10>() {
		std::ifstream inf{ get_input_stream(10) };
		if (!inf) { return; }

		std::vector<int> x_changes{};
		std::vector<int> x_values{ 1 };
		read_input(inf, x_changes, x_values);

		int result{ 0 };
		for (int cycle{ 20 }; cycle < x_values.size(); cycle += 40) {
			result += cycle * x_values[cycle - 1];
		}

		assert(result == 13480);
		std::cout << result << '\n';
	}

	template<>
	void second<10>() {
		std::ifstream inf{ get_input_stream(10) };
		if (!inf) { return; }

		std::vector<int> x_changes{};
		std::vector<int> x_values{ 1 };
		read_input(inf, x_changes, x_values);

		std::string result{};
		result.push_back('\n');
		for (int cycle{ 1 }; cycle < static_cast<int>(x_values.size()); ++cycle) {
			int column = (cycle - 1) % 40;
			if (abs(column - x_values[cycle - 1]) < 2) { result.push_back('#'); }
			else { result.push_back('.'); }

			if (cycle % 40 == 0) { result.push_back('\n'); }
		}

		assert(result == std::string{ "\n" } +
			std::string{ "####..##....##.###...##...##..####.#..#.\n" } +
			std::string{ "#....#..#....#.#..#.#..#.#..#.#....#.#..\n" } +
			std::string{ "###..#.......#.###..#....#....###..##...\n" } +
			std::string{ "#....#.##....#.#..#.#.##.#....#....#.#..\n" } +
			std::string{ "#....#..#.#..#.#..#.#..#.#..#.#....#.#..\n" } +
			std::string{ "####..###..##..###...###..##..#....#..#.\n" });
		std::cout << result;
	}
}

