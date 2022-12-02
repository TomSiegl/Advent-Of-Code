#include <iostream>
#include <fstream>
#include <string>
#include <functional>

namespace door {
	namespace two {
		int opponent_to_int(char opponent) {
			return static_cast<int>(opponent) - 65;
		}

		int own_to_int(char own) {
			return static_cast<int>(own) - 88;
		}

		int outcome_to_int(char outcome) {
			return own_to_int(outcome);
		}

		int rps_outcome_score(int opponent, int own) {
			if (opponent == own) return 3;

			if ((opponent + 1) % 3 == own) {
				return 6;
			}

			return 0;
		}

		int get_own(int opponent, int outcome) {
			if (outcome == 1) return opponent;
			if (outcome == 0) return (opponent - 1 + 3) % 3;
			return (opponent + 1) % 3;
		}

		void solve_puzzle(std::function<int(std::string)> score_update) {
			std::string filename{ "inputs/2.txt" };
			std::ifstream input_file{ filename };

			if (!input_file) {
				std::cerr << "Couldn't open " << filename << '\n';
				return;
			}

			int score = 0;

			std::string curr_line{};
			int first_input{};
			int second_input{};
			while (input_file) {
				std::getline(input_file, curr_line);
				if (curr_line.length() == 0) { break; }

				score += score_update(curr_line);
			}

			std::cout << score << '\n';
		}

		int score_update_first(std::string line) {
			int update = 0;

			int opponent{ opponent_to_int(line[0]) };
			int own{ own_to_int(line[2]) };

			update += own + 1; // score for shape
			update += rps_outcome_score(opponent, own); // score for outcome

			return update;
		}

		int score_update_second(std::string line) {
			int update = 0;

			int opponent{ opponent_to_int(line[0]) };
			int outcome{ outcome_to_int(line[2]) };

			update += get_own(opponent, outcome) + 1;
			update += outcome * 3;

			return update;
		}

		void first() {
			solve_puzzle(score_update_first);
		}

		void second() {
			solve_puzzle(score_update_second);
		}
	}
}

