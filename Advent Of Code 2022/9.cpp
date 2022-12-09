#include "doors.h"
#include "input.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <cassert>

namespace calendar {
	const std::map<char, std::pair<int, int>> directions{
		{'L', {1, 0}},
		{'R', {-1, 0}},
		{'U', {0, 1}},
		{'D', {0, -1}}
	};

	template<typename T, typename U>
	std::pair<T, U> operator+(const std::pair<T, U>& l, const std::pair<T, U>& r) {
		return { l.first + r.first, l.second + r.second };
	}

	template<typename T, typename U>
	std::pair<T, U> operator-(const std::pair<T, U>& l, const std::pair<T, U>& r) {
		return { l.first - r.first, l.second - r.second };
	}

	bool is_neighbour(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
		return std::max(abs(p1.first - p2.first), abs(p1.second - p2.second)) <= 1;
	}

	int manhattan_dist(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
		return abs(p1.first - p2.first) + abs(p1.second - p2.second);
	}

	int sign(int num) {
		if (num < 0) return -1;
		if (num > 0) return 1;
		return 0;
	}

	void normalize(std::pair<int, int>& point) {
		point.first = sign(point.first);
		point.second = sign(point.second);
	}

	template<int L>
	class Rope {
		std::set<std::pair<int, int>> visited_points{};
		std::pair<int, int> head{ 0, 0 };
		std::vector<std::pair<int, int>> tail{ L, {0, 0} };

	public:
		Rope() {
			visited_points.insert(tail[tail.size() - 1]);
		}

		int get_num_visited_points() {
			return visited_points.size();
		}

		void move(char direction_c, int step_size) {
			const std::pair<int, int>& direction{ directions.at(direction_c) };
			for (int i{ 0 }; i < step_size; ++i) {
				std::pair<int, int> last_knot_old{ head };
				std::pair<int, int>* last_knot_ptr{ &head };
				head = head + direction;
				for (int j{ 0 }; j < static_cast<int>(tail.size()); ++j) {
					if (!is_neighbour(*last_knot_ptr, tail[j])) {
						std::pair<int, int> diagonal_step = { *last_knot_ptr - tail[j] };
						normalize(diagonal_step);
						last_knot_old = tail[j];
						tail[j] = tail[j] + diagonal_step;

						last_knot_ptr = &tail[j];
						if (j == static_cast<int>(tail.size() - 1)) { visited_points.insert(tail[j]); }
					}
					else {
						break;
					}
				}
			}
		}
	};

	template<int L>
	int get_num_visited_points(std::ifstream& inf) {
		Rope<L> rope{};
		while (inf) {
			std::string curr_line{};
			std::getline(inf, curr_line);
			if (curr_line.length() == 0) { continue; }

			std::istringstream curr_line_stream{ curr_line };
			char direction_c{};
			int step_size{};
			curr_line_stream >> direction_c >> step_size;
			rope.move(direction_c, step_size);
		}

		return rope.get_num_visited_points();
	}

	template<>
	void first<9>() {
		std::ifstream inf{ get_input_stream(9) };
		if (!inf) { return; }
		int solution{ get_num_visited_points<1>(inf) };
		assert(solution == 6023);
		std::cout << solution << '\n';
	}
	
	template<>
	void second<9>() {
		std::ifstream inf{ get_input_stream(9) };
		if (!inf) { return; }
		int solution{ get_num_visited_points<9>(inf) };
		assert(solution == 2533);
		std::cout << solution << '\n';
	}
}

