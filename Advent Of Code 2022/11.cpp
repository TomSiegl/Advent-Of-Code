#include "doors.h"
#include "input.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cassert>
#include <boost/multiprecision/cpp_int.hpp>

using big_int = boost::multiprecision::checked_int128_t;

namespace calendar {
	class InspectionWorryOperation {
		int first{};
		int second{};
		std::function<big_int(big_int, big_int)> op;
		big_int run_count{ 0 };

	public:
		InspectionWorryOperation(int arg_first, int arg_second, std::function<big_int(big_int, big_int)> arg_op) : 
			first{ arg_first }, 
			second{ arg_second }, 
			op{ arg_op }
		{}

		big_int operator()(big_int arg) {
			++run_count;
			big_int arg1{ first == -1 ? arg : first };
			big_int arg2{ second == -1 ? arg : second };
			return op(arg1, arg2);
		}

		big_int get_run_count() {
			return run_count;
		}
	};

	class MonkeyDecisionOperation {
		int test_num{};
		int monkey1_index{};
		int monkey2_index{};

	public:
		MonkeyDecisionOperation(int arg_test_num, int arg_monkey1_index, int arg_monkey2_index) : 
			test_num{ arg_test_num }, 
			monkey1_index{ arg_monkey1_index }, 
			monkey2_index{ arg_monkey2_index }
		{}

		int operator()(big_int test_worry_level) {
			if (test_worry_level % test_num == 0) { return monkey1_index; }
			else { return monkey2_index; }
		}

		int get_test_num() {
			return test_num;
		}
	};

	class Monkey {
		std::vector<big_int> items{};
		InspectionWorryOperation inspection_op;
		MonkeyDecisionOperation decision_op;
		bool relief{ true };
		big_int value_cap{};

	public:
		Monkey(std::vector<big_int> arg_items, InspectionWorryOperation arg_inspection_op, MonkeyDecisionOperation arg_decision_op, bool arg_relief) :
			items{ arg_items },
			inspection_op{ arg_inspection_op },
			decision_op{ arg_decision_op },
			relief{ arg_relief }
		{}
		
		void take_turn(std::vector<Monkey>& monkeys) {
			for (big_int& item : items) {
				big_int new_item_value{ inspection_op(item) };
				if (relief) new_item_value /= 3;
				else new_item_value %= value_cap;
				int target_monkey_index{ decision_op(new_item_value) };
				monkeys[target_monkey_index].receive_item(new_item_value);
			}
			items.clear();
		}

		void receive_item(big_int item) {
			items.push_back(item);
		}

		big_int get_inspection_count() {
			return inspection_op.get_run_count();
		}

		void set_value_cap(big_int arg_value_cap) {
			value_cap = arg_value_cap;
		}

		int get_test_num() {
			return decision_op.get_test_num();
		}
	};

	void read_monkeys(std::ifstream& inf, std::vector<Monkey>& monkeys, bool relief) {
		std::string ignore_string{};

		while (inf) {
			std::string curr_line{};
			std::getline(inf, curr_line);
			if (curr_line.length() == 0) { continue; }

			// read items
			std::getline(inf, curr_line);
			int colon_index{ static_cast<int>(curr_line.find(':')) };
			std::stringstream curr_line_stream{ curr_line.substr(colon_index + 2, curr_line.length() - colon_index - 2) };
			std::vector<big_int> items{};
			while (curr_line_stream) {
				int curr_item{};
				char ignore_comma{};
				curr_line_stream >> curr_item >> ignore_comma;
				items.push_back(curr_item);
			}

			// read InspectionWorryOperation
			std::getline(inf, curr_line);
			int equals_sign_index{ static_cast<int>(curr_line.find('=')) };
			curr_line_stream = std::stringstream{ curr_line.substr(equals_sign_index + 2, curr_line.length() - equals_sign_index - 2) };
			std::string first_operand_str{};
			std::string operator_symbol{};
			std::string second_operand_str{};

			curr_line_stream >> first_operand_str >> operator_symbol >> second_operand_str;
			
			int first_operand_int{ first_operand_str == "old" ? -1 : std::stoi(first_operand_str) };
			std::function<big_int(big_int, big_int)> inspection_op_function{ nullptr };
			if (operator_symbol == "*") { inspection_op_function = std::multiplies<big_int>{}; }
			if (operator_symbol == "+") { inspection_op_function = std::plus<big_int>{}; }
			int second_operand_int{ second_operand_str == "old" ? -1 : std::stoi(second_operand_str) };
			InspectionWorryOperation inspection_op{ first_operand_int, second_operand_int, inspection_op_function };

			// read MonkeyDecisionOperation
			std::getline(inf, curr_line);
			int by_index{ static_cast<int>(curr_line.find("by")) };
			curr_line_stream = std::stringstream{ curr_line.substr(by_index + 3, curr_line.length() - by_index - 3) };
			int test_num{};
			curr_line_stream >> test_num;

			std::getline(inf, curr_line);
			int monkey_text_index{ static_cast<int>(curr_line.find("monkey")) };
			curr_line_stream = std::stringstream{ curr_line.substr(monkey_text_index + 7, curr_line.length() - monkey_text_index - 7) };
			int monkey1_index{};
			curr_line_stream >> monkey1_index;

			std::getline(inf, curr_line);
			monkey_text_index = static_cast<int>(curr_line.find("monkey"));
			curr_line_stream = std::stringstream{ curr_line.substr(monkey_text_index + 7, curr_line.length() - monkey_text_index - 7) };
			int monkey2_index{};
			curr_line_stream >> monkey2_index;

			MonkeyDecisionOperation decision_op{ test_num, monkey1_index, monkey2_index };

			// create and save Monkey obj
			Monkey monkey{ items, inspection_op, decision_op, relief };
			monkeys.push_back(monkey);
		}
	}

	void set_value_caps(std::vector<Monkey>& monkeys) {
		big_int value_cap{ 1 };
		for (Monkey& monkey : monkeys) {
			value_cap *= monkey.get_test_num();
		}

		for (Monkey& monkey : monkeys) {
			monkey.set_value_cap(value_cap);
		}
	}

	void simulate_round(std::vector<Monkey>& monkeys) {
		for (Monkey& monkey : monkeys) {
			monkey.take_turn(monkeys);
		}
	}

	void simulate_rounds(int num_rounds, std::vector<Monkey>& monkeys) {
		for (int curr_round{ 1 }; curr_round <= num_rounds; ++curr_round) {
			simulate_round(monkeys);
		}
	}

	big_int get_monkey_business_level(std::vector<Monkey>& monkeys) {
		std::vector<big_int> inspection_counts{};
		for (Monkey& monkey : monkeys) {
			inspection_counts.push_back(monkey.get_inspection_count());
		}

		std::sort(inspection_counts.begin(), inspection_counts.end());

		return inspection_counts[inspection_counts.size() - 1] * inspection_counts[inspection_counts.size() - 2];
	}

	template<>
	void first<11>() {
		bool relief{ true };

		std::ifstream inf{ get_input_stream(11) };
		if (!inf) { return; }

		std::vector<Monkey> monkeys{};
		read_monkeys(inf, monkeys, relief);

		simulate_rounds(20, monkeys);

		big_int monkey_business_level = get_monkey_business_level(monkeys);

		assert(monkey_business_level == 56595);
		std::cout << monkey_business_level << '\n';
	}

	template <>
	void second<11>() {
		bool relief{ false };

		std::ifstream inf{ get_input_stream(11) };
		if (!inf) { return; }

		std::vector<Monkey> monkeys{};
		read_monkeys(inf, monkeys, relief);
		set_value_caps(monkeys);

		simulate_rounds(10000, monkeys);

		big_int monkey_business_level = get_monkey_business_level(monkeys);

		assert(monkey_business_level == 15693274740);
		std::cout << monkey_business_level << '\n';
	}
}


