#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <functional>

namespace door {
	namespace three {
		int get_prio(char c) {
			if (c < 'a') {
				// uppercase
				return c - 'A' + 27;
			}
			else {
				// lowercase
				return c - 'a' + 1;
			}
		}

		template<typename T>
		bool contains(std::set<T> set, T item) {
			return set.find(item) != set.end();
		}

		void solve_puzzle(std::function<int(std::ifstream&)> get_prio) {
			std::string filename{ "inputs/3.txt" };
			std::ifstream inf{ filename };

			if (!inf) {
				std::cout << "Couldn't open " << filename << '\n';
				return;
			}

			std::cout << get_prio(inf) << '\n';
		}

		int get_first_prio(std::ifstream& inf) {
			int prio_sum{ 0 };

			std::string curr_line{};
			while (inf) {
				std::getline(inf, curr_line);
				if (curr_line.length() == 0) { continue; }

				std::string first_half = curr_line.substr(0, curr_line.length() / 2);
				std::string second_half = curr_line.substr(curr_line.length() / 2, curr_line.length() / 2);

				std::set<char> first_chars{ first_half.begin(), first_half.end() };
				std::set<char> visited;
				char item{};
				for (int i{ 0 }; i < curr_line.length() / 2; i++) {
					item = second_half[i];
					if (contains(first_chars, item) && !contains(visited, item)) {
						prio_sum += get_prio(item);
						visited.insert(item);
					}
				}
			}

			return prio_sum;
		}

		int get_second_prio(std::ifstream& inf) {
			int prio_sum{ 0 };
			
			std::string first_line{};
			std::string second_line{};
			std::string third_line{};

			std::set<char> first_set{};
			std::set<char> second_set{};
			std::set<char> third_set{};

			std::set<char> intermediate_intersection{};
			std::vector<char> intersection{};
			while (inf) {
				std::getline(inf, first_line);
				if (first_line.length() == 0) { continue; }
				std::getline(inf, second_line);
				std::getline(inf, third_line);

				first_set = { first_line.begin(), first_line.end() };
				second_set = { second_line.begin(), second_line.end() };
				third_set = { third_line.begin(), third_line.end() };

				intermediate_intersection.clear();
				intersection.clear();

				std::set_intersection(
					first_set.begin(), first_set.end(), 
					second_set.begin(), second_set.end(), 
					std::inserter(intermediate_intersection, intermediate_intersection.begin())
				);
				std::set_intersection(
					intermediate_intersection.begin(), intermediate_intersection.end(), 
					third_set.begin(), third_set.end(), 
					std::back_inserter(intersection)
				);

				if (intersection.size() != 1) {
					std::cout << "Wrong size of intersection vector: " << intersection.size() << '\n';
					continue;
				}

				prio_sum += get_prio(intersection[0]);
			}

			return prio_sum;
		}

		void first() {
			solve_puzzle(get_first_prio);
		}

		void second() {
			solve_puzzle(get_second_prio);
		}
	}
}

