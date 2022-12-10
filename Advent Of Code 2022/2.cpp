#include "doors.h"
#include "input.h"
#include "abort_reason.h"
#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <cassert>

namespace calendar {
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

	int solve_puzzle(std::function<void(std::string, int&)> score_update) {
		std::ifstream inf{ get_input_stream(2) };
		if (!inf) { return abort_reason::no_file_stream; }

		int score = 0;

		std::string curr_line{};
		while (inf) {
			std::getline(inf, curr_line);
			if (curr_line.length() == 0) { break; }

			score_update(curr_line, score);
		}

		return score;
	}

	void score_update_first(std::string line, int& score) {
		int opponent{ opponent_to_int(line[0]) };
		int own{ own_to_int(line[2]) };

		score += own + 1; // score for shape
		score += rps_outcome_score(opponent, own); // score for outcome
	}

	void score_update_second(std::string line, int& score) {
		int opponent{ opponent_to_int(line[0]) };
		int outcome{ outcome_to_int(line[2]) };

		score += get_own(opponent, outcome) + 1;
		score += outcome * 3;
	}

	template<>
	void first<2>() {
		int result{ solve_puzzle(score_update_first) };
		if (result == abort_reason::no_file_stream) { return; }
		assert(result == 15691);
		std::cout << result << '\n';
	}

	template<>
	void second<2>() {
		int result{ solve_puzzle(score_update_second) };
		if (result == abort_reason::no_file_stream) { return; }
		assert(result == 12989);
		std::cout << result << '\n';
	}
}

