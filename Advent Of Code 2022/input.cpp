#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

std::ifstream get_input_stream(int door) {
	static const std::string inputs_dir{ "inputs" };
	const std::string filename{ std::to_string(door) + ".txt" };
	std::filesystem::path input_path{ std::filesystem::current_path() / inputs_dir / filename };
	std::ifstream inf = std::ifstream{ input_path.string() };

	if (!inf) {
		std::cerr << "Couldn't read " << input_path.string() << '\n';
	}

	return inf;
}

