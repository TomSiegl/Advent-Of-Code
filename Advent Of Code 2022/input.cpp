#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

std::ifstream get_input_stream(std::string filename) {
	static const std::string inputs_dir{ "inputs" };
	std::filesystem::path input_path{ std::filesystem::current_path() / inputs_dir / filename };
	std::ifstream inf{ input_path.string() };

	if (!inf) {
		std::cerr << "Couldn't read " << input_path.string() << '\n';
	}

	return inf;
}

std::ifstream get_input_stream(int door) {
	return get_input_stream(std::to_string(door) + ".txt");
}

