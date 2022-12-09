#include "doors.h"
#include "input.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

namespace calendar {
	template<>
	void first<1>() {
		std::ifstream inf{ get_input_stream(1) };
		if (!inf) { return; }

		std::string input{};
		int curr_calories = 0;
		int max_calories = 0;
		while (inf) {
			std::getline(inf, input);
			
			if (input.length() != 0) {
				curr_calories += std::stoi(input);
			} else {
				max_calories = std::max(max_calories, curr_calories);
				curr_calories = 0;
			}
		}

		max_calories = std::max(max_calories, curr_calories);

		std::cout << max_calories << std::endl;
	}
	
	template<>
	void second<1>() {
		std::string filename{ "inputs/1.txt" };
		std::ifstream inf{ filename };

		if (!inf) {
			std::cerr << "Couldn't open " << filename << std::endl;
			return;
		}

		std::string input{};
		std::vector<int> calories_sums{ 0 };
		while (inf) {
			std::getline(inf, input);
			
			if (input.length() == 0) {
				calories_sums.push_back(0);
			}
			else {
				calories_sums[calories_sums.size() - 1] += std::stoi(input);
			}
		}

		std::sort(calories_sums.begin(), calories_sums.end());
		int len{ static_cast<int>(calories_sums.size()) };
		int top_three_sum = 0;
		for (int i{ len-1 }; i >= len-3; --i) {
			top_three_sum += calories_sums[i];
			//std::cout << calories_sums[i] << std::endl;
		}

		std::cout << top_three_sum << std::endl;
	}
}

