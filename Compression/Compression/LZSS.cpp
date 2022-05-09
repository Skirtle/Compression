// Libraries
#include <iostream>


// Globals
constexpr auto SLIDE_WINDOW = 15;
constexpr auto LOOK_AHEAD_BUFFER = 4;
constexpr auto SEARCH_BUFFER = 11;

// Prototypes
char LZSS(std::string input);

// main
int main() {
	std::string helloStr = "repetitive repeat";
	std::cout << helloStr;

	return 0;
}

char LZSS(std::string input) {
	int len = input.size();
	for (int i = 0; i < len; i++) {
		std::cout << input.at(i);
	}
	std::cout << "\n";

	return '0';
}