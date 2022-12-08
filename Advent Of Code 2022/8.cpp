#include "doors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

namespace calendar {
	using int_2d_vec = std::vector<std::vector<int>>;
	using bool_2d_vec = std::vector<std::vector<bool>>;

	int_2d_vec get_forest() {
		const std::string filename{ "inputs/8.txt" };
		std::ifstream inf{ filename };

		int_2d_vec forest{};
		if (!inf) {
			std::cerr << "Couldn't read " << filename << '\n';
			return forest;
		}

		while (inf) {
			std::string curr_line{};
			std::getline(inf, curr_line);

			if (curr_line.length() == 0) { continue; }

			std::vector<int> forest_row{};
			for (auto curr_char_it{ curr_line.begin() }; curr_char_it != curr_line.end(); ++curr_char_it) {
				forest_row.push_back((*curr_char_it) - '0');
			}
			forest.push_back(forest_row);
		}

		return forest;
	}

	void write_directional_forest_visibilities(bool_2d_vec& visibility_map, int_2d_vec forest, int x_step, int y_step) {
		bool front_move_direction_x{ x_step != 0 };

		int x_size = static_cast<int>(visibility_map[0].size());
		int y_size = static_cast<int>(visibility_map.size());

		int x_start{ x_step == -1 ? x_size - 1 : 0 };
		int x_end{ x_step == -1 ? -1 : x_size };
		int y_start{ y_step == -1 ? y_size - 1 : 0 };
		int y_end{ y_step == -1 ? -1 : y_size };

		x_step = x_step == 0 ? 1 : x_step;
		y_step = y_step == 0 ? 1 : y_step;

		int front_size{ front_move_direction_x ? y_size : x_size };
		std::vector<int> highest_seen_trees(front_size, -1);
		for (int y{ y_start }; y != y_end; y += y_step) {
			for (int x{ x_start }; x != x_end; x += x_step) {
				int front_index{ front_move_direction_x ? y : x };
				visibility_map[y][x] = visibility_map[y][x] || (highest_seen_trees[front_index] < forest[y][x]);
				highest_seen_trees[front_index] = std::max(highest_seen_trees[front_index], forest[y][x]);
				assert(!(((front_move_direction_x && (x == x_start || x == x_end)) || (!front_move_direction_x && (y == y_start || y == y_end))) && !visibility_map[y][x]));
			}
		}
	}

	void write_forest_visibilities(bool_2d_vec& visibility_map, int_2d_vec& forest) {
		const std::vector<std::pair<int, int>> directions{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

		for (std::pair<int, int> direction : directions) {
			write_directional_forest_visibilities(visibility_map, forest, direction.first, direction.second);
		}
	}

	template<>
	void first<8>() {
		int_2d_vec forest{ get_forest() };
		bool_2d_vec visibility_map{ forest.size(), std::vector<bool>( forest[0].size(), false )};
		write_forest_visibilities(visibility_map, forest);
		
		int visible_count{ 0 };
		std::for_each(visibility_map.begin(), visibility_map.end(),
			[&](std::vector<bool> visibility_row) {
				std::for_each(visibility_row.begin(), visibility_row.end(),
					[&](bool visibility) {
						visible_count += visibility;
					}
				);
			}
		);
		std::cout << visible_count << '\n';
	}

	template<>
	void second<8>() {
		std::cout << "Not yet implemented.\n";
	}
}

