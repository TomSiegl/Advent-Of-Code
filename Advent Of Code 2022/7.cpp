#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <variant>

namespace door {
	namespace seven {
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

		dir read_filesystem() {
			std::string filename{ "inputs/7.txt" };
			std::ifstream inf{ filename };

			if (!inf) {
				std::cerr << "Coudln't open " << filename << '\n';
				exit(1);
			}

			std::string root_name{ "/" };
			dir root{&root_name, nullptr};
			dir* curr_dir{ &root };

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
							curr_dir = &root;
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

			root.calc_size();
			return root;
		}

		void first() {
			dir root = read_filesystem();
			std::cout << sum_small_dirs(&root) << '\n';
		}

		void second() {
			dir root = read_filesystem();
			std::cout << get_min_deletable_size(&root) << '\n';
		}
	}
}

