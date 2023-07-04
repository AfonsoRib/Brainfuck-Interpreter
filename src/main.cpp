#include <cstdlib>
#include <iostream>
#include<vector>
#include <stack>
#include <fstream>

using namespace std;

vector<int> getLoopTable(char* buffer, int fileSize){
    stack<int> stack;
    std::vector<int> loopTable(fileSize);
    int ip = 0;
    while(ip < fileSize){
      char instruction = buffer[ip];
      if(instruction == '['){
        stack.push(ip);
      }else if (instruction == ']') {
	int beginning = stack.top(); stack.pop();
	loopTable[ip] = beginning;
	loopTable[beginning] = ip;
      }

      
      ++ip;
    }
    return loopTable;
}


int main(int argc, char *argv[])
{
  std::ifstream inputFile(argv[1]);
  char* fileContent;
  std::streampos fileSize;
  if (inputFile.is_open()) {
    // Get the length of the file
    inputFile.seekg(0, std::ios::end);
    fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    // Create a char array with enough space to store the file contents
    fileContent = new char[fileSize];

    // Read the file into the buffer
    inputFile.read(fileContent, fileSize);

    // Close the file
    inputFile.close();
  } else {
    std::cout << "Failed to open the file" << std::endl;
    return -1;
  }

  vector<int> tape(1, 0);
  vector<int> loopTable = getLoopTable(fileContent, fileSize);
  int ip = 0;
  int index = 0;

  while (ip < fileSize) {
    char instruction = fileContent[ip];

    switch(instruction){
    case '+':
      ++tape[index];
      if(tape[index] == 256)
        tape[index] = 0;
      break;
    case '-':
      --tape[index];
      if(tape[index] == -1)
        tape[index] = 255;
      break;
    case '>':
      ++index;
      if (static_cast<std::vector<int>::size_type>(index) == tape.size())
	tape.push_back(0);
      break;
    case '<':
      --index;
      if(index == -1) index = 0;
      break;
    case '.':
      cout << static_cast<char>(tape[index]);
      break;
    case ',':
      char num;
      cin >> num;
      tape[index] = static_cast<int>(num);
      break;
    case '[':
      if (!tape[index])
	ip = loopTable[ip];
      break;
    case ']':
      if(tape[index])
	ip = loopTable[ip];
      break;
    }
    ++ip;
  }

  return 0;
}

