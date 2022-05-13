// Libraries
#include <iostream>
#include <bitset>
#include <cassert>


// Globals
constexpr auto SLIDE_WINDOW_SIZE = 15;
constexpr auto LOOK_AHEAD_BUFFER_SIZE = 4;
constexpr auto SEARCH_BUFFER_SIZE = 11;

// Classes
// Encoded characters
class EC {
public:
	bool isChar = false;
	uint8_t offset = 0, length = 0, size = 0;
	char character = 0;

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
void moveWindowLeft(char* window, char nextChar);
void printEncode(EC* encoding);
std::string LZSSDecode(EC* encode);
bool encodeDecode(std::string str);


// main
int main() {
	std::string testStr = "compress the compression";
	EC* encoded = LZSSEncode(testStr);

	
	printEncode(encoded);
	encodeDecode(testStr);


	/*std::bitset<8> bits;
	bits |= 0b10101010;

	std::cout << bits << std::endl;*/


	free(encoded);
	return 0;
}

EC* LZSSEncode(std::string input) {
	// Variables
	int index = LOOK_AHEAD_BUFFER_SIZE - 1, encodedIndex = 0;
	const int inputLen = input.length();

	// Buffers
	EC* encoded = (EC*)malloc(sizeof(EC) * inputLen);
	char* window = (char*)malloc(sizeof(char) * SLIDE_WINDOW_SIZE);

	

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

		if (len == 0) {
			index++;
			if (index < inputLen) {
				moveWindowLeft(window, input[index]);
			}
			else {
				moveWindowLeft(window, 0);
			}
		}
		else {
			for (int L = 0; L < len; L++) {
				index++;
				if (index < inputLen) {
					moveWindowLeft(window, input[index]);
				}
				else {
					moveWindowLeft(window, 0);
				}
			}
		}
	}

	for (int i = 0; i < encodedIndex; i++) {
		encoded[i].size = encodedIndex;
	}

	free(window);
	return encoded;
}

std::ostream& operator<< (std::ostream& out, const EC& data) {
	if (data.isChar) out << data.character;
	else {
		out << '(' << (int) data.offset << ',' << (int) data.length << ')';
		//out << data.offset << data.length;
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

void moveWindowLeft(char* window, char nextChar) {
	for (int i = 0; i < SLIDE_WINDOW_SIZE - 1; i++) {
		window[i] = window[i + 1];
	}
	window[SLIDE_WINDOW_SIZE - 1] = nextChar;
}

std::string LZSSDecode(EC* encode) {
	std::string decode = "";

	for (int index = 0; index < encode[0].size; index++) {

		if (encode[index].isChar) {
			decode = decode + encode[index].character;
		}
		else {
			int offset = encode[index].offset;
			int length = encode[index].length;
			for (int i = 0; i < length; i++) {
				decode = decode + decode.at(decode.length() - offset);
			}
		}
	}
	return decode;
}

bool encodeDecode(std::string str) {
	EC* encode = LZSSEncode(str);
	bool same = str.compare(LZSSDecode(encode)) == 0;
	free(encode);
	if (same) std::cout << "Success\n";
	else std::cout << "Failure\n";
	return same;
}

void printEncode(EC* encoding) {
	for (int i = 0; i < encoding[0].size; i++) {
		std::cout << encoding[i];
	}
	std::cout << "\n";
}