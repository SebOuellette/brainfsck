#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/poll.h>
#include <vector>

// In the original implementation, the array was 30000 characters long
#define ARRAY_SIZE 30000
typedef unsigned char byte;

std::string readFile () {
	std::string retString;

	struct pollfd stdinPoll;
	stdinPoll.fd = 0;
	stdinPoll.events = POLLIN;

	// Check if there is input in stdin
	if (poll(&stdinPoll, 1, 0) == 1) {
		while (std::cin >> retString) continue;
	}

	return retString;
}

void updateCallStack(char data, int *callStack) {
	*callStack += (data == '[') ? 1 : ((data == ']') ? -1 : 0); 
}

int main(int argc, char* argv[]) {
	std::string bfSourceCode;

	bfSourceCode = readFile();

	// Taking input from arguments
	for (int i=1;i<argc;i++)
		bfSourceCode += argv[i];


	// Create the array, initialize all elements to 0
	byte* array = (byte*) std::calloc(ARRAY_SIZE, sizeof(byte));
	unsigned int pointer = 0;
	int callStack = 0;

	// Loop through input
	for (int i=0;i<bfSourceCode.length();i++) {
		// Keeps pointer between 0 and ARRAY_SIZE if pointer went above or below bounds
		pointer = (pointer + ARRAY_SIZE) % ARRAY_SIZE;

		//std::cout << bfSourceCode[i] << " : " << callStack << std::endl;

		// Update the call stack with the current character (+1 if in a loop, -1 if out of a loop)
		updateCallStack(bfSourceCode[i], &callStack);

		int callStackStatus = callStack;

		switch (bfSourceCode[i]) {
			case '>': pointer++; break;
			case '<': pointer--; break;
			case '+': array[pointer]++; break;
			case '-': array[pointer]--; break;
			case '.': std::putc(array[pointer], stdout); break;
			//case '.': std::cout << +array[pointer]; break;
			case ',': array[pointer] = std::getc(stdin); break;
			// If we reached termination, skip to end of loop
			case '[': 
				if (array[pointer] == 0)
					while (!(bfSourceCode[i] == ']' && callStack == callStackStatus - 1)) {
						//std::cout << "Finding end of loop : " << i << " : " << bfSourceCode[i] << " : " << callStack << " : " << callStackStatus  << std::endl;
						i++; 
						updateCallStack(bfSourceCode[i], &callStack);
					}
				break; 
			// If we have not reached termination, skip to beginning of loop
			case ']': 
				if (array[pointer] != 0)
					while (!(bfSourceCode[i] == '[' && callStack == callStackStatus + 1)) {
						//std::cout << "Finding beginning of loop : " << i << " : " << bfSourceCode[i] << " : " << callStack << " : " << callStackStatus  << std::endl;
						i--; 
						updateCallStack(bfSourceCode[i], &callStack);
					}

				break;

			default: break;
		}

		
	}

	// std::cout << "\nPointer: " << pointer << std::endl;
	// std::cout << "Value: " << +array[pointer] << std::endl;
	// std::cout << "callStack: " << callStack << std::endl;

	free(array);
	return 0; 
}