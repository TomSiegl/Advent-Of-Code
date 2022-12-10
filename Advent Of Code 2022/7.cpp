#include "doors.h"
#include "input.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <variant>
#include <cassert>

namespace calendar {
	// This has to be possible with less pointers...

	struct file {
		std::string* name{};
		int* size{};
	};

	struct dir {
		std::string* name;
		dir* superdir;
		std::vector<dir*> subdirs{};
		std::vector<file*> files{};
		int size{};

		dir(std::string* name, dir* superdir) : name{ name }, superdir{ superdir } {}

		void calc_size() {
			size = 0;
			for (file* f : files) {
				size += *(*f).size;
			}
			for (dir* d : subdirs) {
				(*d).calc_size();
				size += (*d).size;
			}
		}
	};

	int sum_small_dirs(dir* start_dir) {
		int size_sum{};
		if ((*start_dir).size <= 100000) {
			size_sum += (*start_dir).size;
		}

		for (dir* d : (*start_dir).subdirs) {
			size_sum += sum_small_dirs(d);
		}
		
		return size_sum;
	}

	int get_min_size_above(dir* curr_dir, const int& threshold) {
		if ((*curr_dir).size < threshold) {
			return 0;
		}

		int min{ (*curr_dir).size };
		for (dir* d : (*curr_dir).subdirs) {
			int d_min_size{ get_min_size_above(d, threshold) };
			if (d_min_size < threshold) { continue; }
			min = std::min(min, d_min_size);
		}

		return min;
	}

	int get_min_deletable_size(dir* root) {
		constexpr int total_storage{ 70000000 };
		constexpr int needed_storage{ 30000000 };

		const int free_storage{ total_storage - (*root).size };
		const int missing_storage{ needed_storage - free_storage };

		return get_min_size_above(root, missing_storage);
	}

	dir* read_filesystem() {
		std::ifstream inf{ get_input_stream(7) };
		if (!inf) { return nullptr; }

		std::string root_name{ "/" };
		dir* root{ new dir{&root_name, nullptr} };
		dir* curr_dir{ root };

		while (inf) {
			std::string curr_line{};
			std::getline(inf, curr_line);

			if (curr_line.length() == 0) {
				break;
			}

			if (curr_line[0] == '$') {
				// command
				if (curr_line[2] == 'c') {
					// change directory
					std::string ignore{};
					std::string target_dir_name{};
					std::stringstream curr_line_stream{ curr_line };
					curr_line_stream >> ignore >> ignore >> target_dir_name;

					if (target_dir_name == "..") {
						curr_dir = (*curr_dir).superdir;
					}
					else if (target_dir_name == "/") {
						curr_dir = root;
					}
					else {
						curr_dir = *std::find_if(
							(*curr_dir).subdirs.begin(), 
							(*curr_dir).subdirs.end(), 
							[=](dir* d) {
								return *(*d).name == target_dir_name; 
							}
						);
					}

					continue;
				}
				else {
					// list content
					continue;
				}
			}
			else if (curr_line[0] == 'd') {
				// directory
				std::string ignore{};
				std::string* new_dir_name{ new std::string };
				std::stringstream curr_line_stream{ curr_line };
				curr_line_stream >> ignore >> *new_dir_name;
				dir* new_dir{ new dir{new_dir_name, curr_dir}  };
				(*curr_dir).subdirs.push_back(&*new_dir);
			}
			else {
				// file
				int* size{ new int };
				std::string* new_file_name{ new std::string };
				std::stringstream curr_line_stream{ curr_line };
				curr_line_stream >> *size >> *new_file_name;
				file* new_file{ new file{new_file_name, size} };
				(*curr_dir).files.push_back(&*new_file);
			}
		}

		(*root).calc_size();
		return root;
	}

	template<>
	void first<7>() {
		dir* root = read_filesystem();
		if (root == nullptr) return;
		int result{ sum_small_dirs(root) };
		assert(result == 1583951);
		std::cout << result << '\n';
	}

	template<>
	void second<7>() {
		dir* root = read_filesystem();
		if (root == nullptr) return;
		int result{ get_min_deletable_size(root) };
		assert(result == 214171);
		std::cout << result << '\n';
	}
}

