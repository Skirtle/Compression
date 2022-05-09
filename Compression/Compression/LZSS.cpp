// Libraries
#include <iostream>


// Globals
constexpr auto SLIDE_WINDOW_SIZE = 15;
constexpr auto LOOK_AHEAD_BUFFER_SIZE = 4;
constexpr auto SEARCH_BUFFER_SIZE = 11;

// Classes
// Encoded characters
class EC {
public:
	bool isChar = NULL;
	int offset = NULL, length = NULL;
	char character = NULL;

	EC() {
	}
	EC(char c) {
		isChar = true;
		character = c;
	}
	EC(int off, int len) {
		offset = off;
		length = len;
		isChar = false;
	}
};

// Prototypes
EC* LZSSEncode(std::string input);
std::ostream& operator<< (std::ostream& out, const EC& data);
void printWindow(char* window);
void moveWindowRight(char* window, char nextChar);


// main
int main() {
	std::string testStr = "repetitive repeat";
	LZSSEncode(testStr);

	return 0;
}

EC* LZSSEncode(std::string input) {
	// Buffers
	EC* encoded = (EC*)malloc(sizeof(EC) * input.length());
	char* window = (char*)malloc(sizeof(char) * SLIDE_WINDOW_SIZE);

	// Other variables
	int index = LOOK_AHEAD_BUFFER_SIZE - 1, encodedIndex = 0;

	// Make sure buffers allocated
	if (!(encoded && window)) return NULL;

	for (int i = 0, j = 0; i < SLIDE_WINDOW_SIZE; i++) {
		if (i < SEARCH_BUFFER_SIZE) window[i] = 0;
		else {
			window[i] = input.at(j);
			j++;
		}
	}

	// While characters to look for
	while (window[SEARCH_BUFFER_SIZE] != 0) {
		char lookFor = window[SEARCH_BUFFER_SIZE];
		bool found = false;
		int len = 0, offset = 0;

		// Search for character
		for (int i = SEARCH_BUFFER_SIZE - 1; i >= 0; i--) {

			// Character found
			if (window[i] == lookFor) {
				found = true;
				offset = SEARCH_BUFFER_SIZE - i;

				// Find longest string in look ahead buffer that is in window
				int j = SEARCH_BUFFER_SIZE;
				while (window[i] == window[j]) {
					len++;
					i++;
					j++;
				}
			}
			if (found) break;
		}

		if (!found) {
			encoded[encodedIndex] = EC(lookFor);
			encodedIndex++;
		}
		else {
			encoded[encodedIndex] = EC(offset, len);
			encodedIndex++;
		}

		printWindow(window);

		if (len == 0) {
			index++;
			moveWindowRight(window, input[index]);
		}
		else {
			for (int L = 0; L < len; L++) {
				index++;
				moveWindowRight(window, input[index]);
			}
		}


		for (int i = 0; i < encodedIndex; i++) {
			std::cout << encoded[i];
		}
		std::cout << "\n\n";

		
	}


	free(encoded);
	free(window);
	return NULL;
}

std::ostream& operator<< (std::ostream& out, const EC& data) {
	if (data.isChar) out << data.character;
	else {
		out << '(' << data.offset << ',' << data.length << ')';
	}
	return out;
}

void printWindow(char* window) {
	std::cout << '|';
	for (int i = 0; i < SLIDE_WINDOW_SIZE; i++) {
		if (i == SEARCH_BUFFER_SIZE) std::cout << '|';
		if (window[i] == 0) std::cout << '_';
		else std::cout << window[i];
	}
	std::cout << "|\n";
}

void moveWindowRight(char* window, char nextChar) {
	for (int i = 0; i < SLIDE_WINDOW_SIZE - 1; i++) {
		window[i] = window[i + 1];
	}
	window[SLIDE_WINDOW_SIZE - 1] = nextChar;
}