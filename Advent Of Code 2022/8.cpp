#include "doors.h"
#include "input.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <set>

namespace calendar {
	using int_2d_vec = std::vector<std::vector<int>>;
	using bool_2d_vec = std::vector<std::vector<bool>>;
	using Direction = std::pair<int, int>;

	struct Directions {
		static constexpr Direction x_pos{ 1, 0 };
		static constexpr Direction x_neg{ -1, 0 };
		static constexpr Direction y_pos{ 0, -1 };
		static constexpr Direction y_neg{ 0, 1 };
	};

	struct ViewDistances {
		int top{};
		int bottom{};
		int left{};
		int right{};
	};

	using dist_2d_vec = std::vector<std::vector<ViewDistances>>;

	class Front {
	public:
		virtual void step(int x, int y) = 0;
	};

	class VisibilityFront {
	private:
		std::vector<int> highest_seen_trees;
		bool_2d_vec& visibility_map;
		int_2d_vec& forest;
		bool front_move_direction_x;

	public:
		VisibilityFront(int front_size, bool_2d_vec& visibility_map, int_2d_vec& forest, Direction direction) :
			highest_seen_trees(front_size, -1),
			visibility_map{ visibility_map },
			forest{ forest },
			front_move_direction_x{ direction.first != 0 } {}

		void step(int x, int y) {
			int front_index{ front_move_direction_x ? y : x };
			visibility_map[y][x] = visibility_map[y][x] || (highest_seen_trees[front_index] < forest[y][x]);
			highest_seen_trees[front_index] = std::max(highest_seen_trees[front_index], forest[y][x]);
		}
	};

	class ScenicScoreFront {
	private:
		int_2d_vec seen_trees;
		dist_2d_vec& view_distance_map;
		int_2d_vec& forest;
		Direction direction;
		bool front_move_direction_x;

	public:
		ScenicScoreFront(int front_size, dist_2d_vec& view_distance_map, int_2d_vec& forest, Direction direction) :
			seen_trees(front_size, std::vector<int>{}),
			view_distance_map{ view_distance_map },
			forest{ forest },
			direction{ direction },
			front_move_direction_x{ direction.first != 0 } {}

		void step(int x, int y) {
			int front_index{ front_move_direction_x ? y : x };
			int curr_tree_height{ forest[y][x] };

			if (seen_trees[front_index].size() == 0) {
				seen_trees[front_index].push_back(forest[y][x]);
				return;
			}

			auto blocking_tree_it = std::find_if(seen_trees[front_index].rbegin(), seen_trees[front_index].rend(), [&](int e) { return e >= curr_tree_height; });
			int dist = static_cast<int>(std::distance(seen_trees[front_index].rbegin(), blocking_tree_it)) + 1;
			dist = std::min(dist, static_cast<int>(seen_trees[front_index].size()));
			if (direction == Directions::x_pos) { view_distance_map[y][x].left = dist; }
			else if (direction == Directions::x_neg) { view_distance_map[y][x].right = dist; }
			else if (direction == Directions::y_pos) { view_distance_map[y][x].bottom = dist; }
			else if (direction == Directions::y_neg) { view_distance_map[y][x].top = dist; }
			seen_trees[front_index].push_back(forest[y][x]);
		}
	};

	template<typename Front_T>
	class MapTypeSelector;

	template <> class MapTypeSelector<VisibilityFront> { public: using MapType = int_2d_vec; };
	template <> class MapTypeSelector<ScenicScoreFront> { public: using MapType = dist_2d_vec; };

	void get_forest(int_2d_vec* forest) {
		std::ifstream inf{ get_input_stream(8) };
		if (!inf) { return; }

		while (inf) {
			std::string curr_line{};
			std::getline(inf, curr_line);

			if (curr_line.length() == 0) { continue; }

			std::vector<int> forest_row{};
			for (auto curr_char_it{ curr_line.begin() }; curr_char_it != curr_line.end(); ++curr_char_it) {
				forest_row.push_back((*curr_char_it) - '0');
			}
			(*forest).push_back(forest_row);
		}
	}

	template<typename Front_T, typename Map_T = typename MapTypeSelector<Front_T>::MapType>
	void apply_front_directional(Map_T& tree_value_map, int_2d_vec& forest, Direction direction) {
		int x_size = static_cast<int>(tree_value_map[0].size());
		int y_size = static_cast<int>(tree_value_map.size());

		int x_start{ direction.first == -1 ? x_size - 1 : 0 };
		int x_end{ direction.first == -1 ? -1 : x_size };
		int y_start{ direction.second == -1 ? y_size - 1 : 0 };
		int y_end{ direction.second == -1 ? -1 : y_size };

		int x_step = direction.first == 0 ? 1 : direction.first;
		int y_step = direction.second == 0 ? 1 : direction.second;

		bool front_move_direction_x{ direction.first != 0 };
		int front_size{ front_move_direction_x ? y_size : x_size };
		Front_T front{ front_size, tree_value_map, forest, direction };
		for (int y{ y_start }; y != y_end; y += y_step) {
			for (int x{ x_start }; x != x_end; x += x_step) {
				front.step(x, y);
			}
		}
	}

	template<typename Front_T, typename Map_T = typename MapTypeSelector<Front_T>::MapType>
	void apply_front_all_directions(Map_T& tree_value_map, int_2d_vec& forest) {
		const std::vector<Direction> directions{ Directions::x_pos, Directions::x_neg, Directions::y_pos, Directions:: y_neg };

		for (Direction direction : directions) {
			apply_front_directional<Front_T>(tree_value_map, forest, direction);
		}
	}

	int get_scenic_score(ViewDistances distances) {
		return distances.top * distances.bottom * distances.left * distances.right;
	}

	template<>
	void first<8>() {
		int_2d_vec* forest_ptr{};
		get_forest(forest_ptr);
		if (forest_ptr == nullptr) { return; }

		int_2d_vec forest{ *forest_ptr };
		if (forest.size() == 0) { return; }

		bool_2d_vec visibility_map{ forest.size(), std::vector<bool>( forest[0].size(), false )};
		apply_front_all_directions<VisibilityFront>(visibility_map, forest);
		
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
		int_2d_vec* forest_ptr{};
		get_forest(forest_ptr);
		if (forest_ptr == nullptr) { return; }

		int_2d_vec forest{ *forest_ptr };
		if (forest.size() == 0) { return; }

		dist_2d_vec view_distance_map(forest.size(), std::vector<ViewDistances>(forest[0].size(), ViewDistances{}));
		apply_front_all_directions<ScenicScoreFront>(view_distance_map, forest);

		int highest_scenic_score{};
		for (std::vector<ViewDistances> view_distance_row : view_distance_map) {
			for (ViewDistances view_distances : view_distance_row) {
				highest_scenic_score = std::max(highest_scenic_score, get_scenic_score(view_distances));
			}
		}

		std::cout << highest_scenic_score << '\n';
	}
}

