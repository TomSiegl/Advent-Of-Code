#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

namespace door {
	namespace one {
		void first() {
			std::string filename{ "inputs/1.txt" };
			std::ifstream inf{ filename };

			if (!inf) {
				std::cerr << "Could not open " << filename << std::endl;
				return;
			}

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

			std::cout << "Solution of door 01, first puzzle: " << max_calories << std::endl;
		}
		
		void second() {
			std::string filename{ "inputs/1.txt" };
			std::ifstream inf{ filename };

			if (!inf) {
				std::cerr << "Could not open " << filename << std::endl;
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
			int len = calories_sums.size();
			int top_three_sum = 0;
			for (int i{ len-1 }; i >= len-3; --i) {
				top_three_sum += calories_sums[i];
				//std::cout << calories_sums[i] << std::endl;
			}

			std::cout << "Solution of door 01, second puzzle: " << top_three_sum << std::endl;
		}
	}
}

