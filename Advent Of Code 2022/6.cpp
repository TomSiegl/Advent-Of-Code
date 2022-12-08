#include "doors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <algorithm>

namespace calendar {
	void solve_puzzle(int num_different_chars) {
		std::string filename{ "inputs/6.txt" };
		std::ifstream inf{ filename };

		if (!inf) {
			std::cerr << "Couldn't open " << filename << "\n";
			return;
		}

		int result{ 0 };
		char current_char{};
		std::deque<char> last_chars{};
		bool done{ false };
		while (inf) {
			++result;
			inf >> current_char;

			std::deque<char>::iterator finding = std::find(last_chars.begin(), last_chars.end(), current_char);
			if (finding != last_chars.end()) {
				last_chars.erase(last_chars.begin(), finding + 1);
			}
			else if (static_cast<int>(last_chars.size()) == num_different_chars - 1) {
				done = true;
				break;
			}
			
			last_chars.push_back(current_char);
		}

		if (done) {
			std::cout << result << "\n";
		}
		else {
			std::cout << "No starting sequence found.\n";
		}
	}

	template<>
	void first<6>() {
		solve_puzzle(4);
	}

	template<>
	void second<6>() {
		solve_puzzle(14);
	}
}

