#include "structures.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	std::cout << "������� ��� ����� ��� ���������� �������� � ������" << std::endl;
	std::string filename;
	std::cin >> filename;
	std::ifstream rulefile(filename);
	auto data = read_data(rulefile);

	std::string output = apply(data.first,data.second);

	std::cout <<"���������: "<< output << "\n";
}