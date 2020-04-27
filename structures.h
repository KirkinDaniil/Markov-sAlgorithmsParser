#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct rule
{
	std::string pattern;
	std::string replacement;
	bool terminal;
	rule(std::string pat, std::string rep, bool term) : pattern(pat), replacement(rep), terminal(term) {}
};

std::string const whitespace = " \t";


bool is_whitespace(char c)
{
	return whitespace.find(c) != std::string::npos;
}

std::pair<std::vector<rule>,std::string> read_data(std::ifstream& file)
{
	std::string input;
	std::vector<rule> rules;
	std::string line;
	size_t line_number = 0;
	while (std::getline(file, line))
	{
		std::string::size_type pos;

		if (line.find_first_not_of(whitespace) == std::string::npos)
			continue;

		if (line_number == 0)
		{
			input = line;
			++line_number;
			continue;
		}

		pos = line.find("->");
		while (pos != std::string::npos && (pos == 0 || !is_whitespace(line[pos - 1])))
			pos = line.find("->", pos + 1);

		if (pos == std::string::npos || line.length() < pos + 3 || !is_whitespace(line[pos + 2]))
		{
			std::cerr << "invalid rule: " << line << "\n";
			std::exit(EXIT_FAILURE);
		}

		std::string pattern = line.substr(0, pos - 1);
		std::string replacement = line.substr(pos + 3);

		pattern.erase(pattern.find_last_not_of(whitespace) + 1);
		replacement.erase(0, replacement.find_first_not_of(whitespace));

		bool terminal = !replacement.empty() && replacement[0] == '.';
		if (terminal)
			replacement.erase(0, 1);

		rules.push_back(rule(pattern, replacement, terminal));
		++line_number;
	}

	return std::make_pair(rules,input);
}

std::string apply(std::vector<rule> rules, std::string input)
{
	std::string& output = input;
	std::vector<rule>::iterator iter = rules.begin();

	while (iter != rules.end())
	{
		std::string::size_type pos = output.find(iter->pattern);
		if (pos != std::string::npos)
		{
			output.replace(pos, iter->pattern.length(), iter->replacement);
			if (iter->terminal)
				break;
			iter = rules.begin();
		}
		else
			++iter;
	}

	return output;
}
