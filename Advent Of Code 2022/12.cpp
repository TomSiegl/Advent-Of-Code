#include "doors.h"
#include "input.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>

namespace calendar {
	std::pair<int, int> operator+(std::pair<int, int> a, std::pair<int, int> b) {
		return { a.first + b.first, a.second + b.second };
	}

	void read_grid(std::ifstream& inf, std::vector<std::vector<char>>& grid, std::pair<int, int>& start, std::pair<int, int>& end) {
		int row_index{ 0 };
		while (inf) {
			std::string curr_line{};
			std::getline(inf, curr_line);
			if (curr_line.length() == 0) { continue; }

			int col_index{ 0 };
			std::vector<char> grid_row{};
			for (char c : curr_line) {
				if (c == 'S') {
					start = { row_index, col_index };
					grid_row.push_back('a');
				}
				else if (c == 'E') {
					end = { row_index, col_index };
					grid_row.push_back('z');
				}
				else {
					grid_row.push_back(c);
				}

				++col_index;
			}
			grid.push_back(grid_row);

			++row_index;
		}
	}

	int get_node_index(int row_index, int col_index, int col_count) {
		return row_index * col_count + col_index;
	}

	bool is_inside_grid(std::pair<int, int>& position, int row_count, int col_count) {
		return position.first >= 0 && position.first < row_count&& position.second >= 0 && position.second < col_count;
	}

	void get_neighbor_indices(int row_index, int col_index, int row_count, int col_count, std::vector<std::pair<int, int>>& neighbors, std::vector<std::vector<char>>& grid) {
		std::vector<std::pair<int, int>> directions{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
		std::pair<int, int> position{ row_index, col_index };
		for (std::pair<int, int> direction : directions) {
			std::pair<int, int> candidate{ position + direction };
			if (is_inside_grid(candidate, row_count, col_count)) {
				neighbors.push_back({ get_node_index(candidate.first, candidate.second, col_count), grid[candidate.first][candidate.second] - grid[position.first][position.second]});
			}
		}
	}

	void construct_graph(std::vector<std::vector<char>>& grid, std::vector<std::vector<std::pair<int, int>>>& graph) {
		for (int curr_row{ 0 }; curr_row < grid.size(); ++curr_row) {
			for (int curr_col{ 0 }; curr_col < grid[0].size(); ++curr_col) {
				get_neighbor_indices(curr_row, curr_col, grid.size(), grid[0].size(), graph[get_node_index(curr_row, curr_col, grid[0].size())], grid);
			}
		}
	}

	void filter_graph_by_edge_cost(std::vector<std::vector<std::pair<int, int>>>& graph, int max_allowed_cost) {
		for (std::vector<std::pair<int, int>>& curr_node_edges : graph) {
			for (std::vector<std::pair<int, int>>::iterator it{ curr_node_edges.begin() }; it != curr_node_edges.end();) {
				if ((*it).second > max_allowed_cost) {
					it = curr_node_edges.erase(it);
				}
				else {
					++it;
				}
			}
		}
	}

	void get_shortest_path(std::vector<std::vector<std::pair<int, int>>>& graph, int start_index, int end_index, std::vector<int>& shortest_path) {
		std::queue<int> next_nodes{};
		next_nodes.push(start_index);

		std::set<int> visited{};

		std::vector<int> distances(static_cast<int>(graph.size()), -1);
		distances[start_index] = 0;

		std::vector<int> previous(static_cast<int>(graph.size()), -1);
		previous[start_index] = -1;

		while (!next_nodes.empty()) {
			int curr_node{ next_nodes.front() };
			next_nodes.pop();

			if (visited.find(curr_node) != visited.end()) { continue; }
			visited.insert(curr_node);

			for (std::pair<int, int>& edge : graph[curr_node]) {
				if (visited.find(edge.first) != visited.end()) { continue; }

				distances[edge.first] = distances[curr_node] + 1;
				previous[edge.first] = curr_node;

				if (edge.first == end_index) { break; }

				next_nodes.push(edge.first);
			}
		}

		shortest_path.push_back(end_index);
		while (previous[shortest_path[shortest_path.size() - 1]] != -1) {
			shortest_path.push_back(previous[shortest_path[shortest_path.size() - 1]]);
		}
		std::reverse(shortest_path.begin(), shortest_path.end());
	}

	void print_path(std::vector<int>& shortest_path, int row_count, int col_count) {
		std::set<int> path_nodes{ shortest_path.begin(), shortest_path.end() };

		std::cout << '\n';
		for (int row{ 0 }; row < row_count; ++row) {
			for (int col{ 0 }; col < col_count; ++col) {
				int node_index{ get_node_index(row, col, col_count) };
				if (path_nodes.find(node_index) != path_nodes.end()) {
					std::cout << 'X';
				}
				else {
					std::cout << '.';
				}
			}
			std::cout << '\n';
		}
	}

	template<>
	void first<12>() {
		std::string filename{ "12.txt" };
		std::ifstream inf{ get_input_stream(filename) };
		if (!inf) { return; }

		std::vector<std::vector<char>> grid{};
		std::pair<int, int> start{};
		std::pair<int, int> end{};
		read_grid(inf, grid, start, end);

		int start_index{ get_node_index(start.first, start.second, grid[0].size()) };
		int end_index{ get_node_index(end.first, end.second, grid[0].size()) };

		std::vector<std::vector<std::pair<int, int>>> graph{ grid.size() * grid[0].size(), std::vector<std::pair<int, int>>{} };
		construct_graph(grid, graph);
		filter_graph_by_edge_cost(graph, 1);

		std::vector<int> shortest_path{};
		get_shortest_path(graph, start_index, end_index, shortest_path);
		print_path(shortest_path, grid.size(), grid[0].size());

		std::cout << shortest_path.size() - 1 << '\n';
	}

	template<>
	void second<12>() {
		std::cout << "Not yet implemented.\n";
	}
}

