#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

#define unless(cond) if (!(cond))

char *parseArg(int argc, char *argv[], const char *targetArg) {
  bool isTarget = false;
  char *argument = "";
  for (int i = 0; i < argc; i++) {
    if (isTarget) {
      argument = argv[i];
      break;
    }
    if (!std::strcmp(argv[i], targetArg)) {
      isTarget = true;
    }
  }
  return argument;
}

class String {
private:
  char str[64];
  size_t ln;

public:
  String(char *initStr) { 
    this->ln = strlen(initStr); 
    strncpy(str, initStr, ln);
    str[ln] = '\0';
  }

  char operator[](int index) {
    if (index >= this->ln || index < 0)
      throw std::out_of_range("Wrong index for String");
    return str[index];
  }

  void log() const {
    std::cout << "String: " << this->str << std::endl;
    std::cout << "String Length: " << this->ln << std::endl;
  }

  char *getChars() { return str; }

  size_t getLength() const { return this->ln; }

  bool includes(String other) {
    int mask1[26];
    int mask2[26];
          
    for (int i = 0; i < 26; ++i) mask1[i] = 0;
    for (int i = 0; i < 26; ++i) mask2[i] = 0;

    for (int i = 0; i < this->getLength(); i++) {
      mask1[this->str[i] - 'a'] = 1;
    }

    for (int i = 0; i < other.getLength(); i++) {
      mask2[other[i] - 'a'] = 1;
    }

    for (int i = 0; i < 26; ++i) {
      unless (mask1[i] <= mask2[i]) {
        return false;
      }
    }
    return true;
  }
};

int main(int argc, char *argv[]) {
  String word = String(parseArg(argc, argv, "--word"));
  String filepath = String(parseArg(argc, argv, "--file"));

  char parsedStr[64];
  int lengthStr = 0;
  
  char inputChar;
  int cnt = 0;
  std::ifstream in;
  in.open(filepath.getChars());
  while (in.get(inputChar)) {
    if (inputChar >= 'a' && inputChar <= 'z') {
      parsedStr[lengthStr] = inputChar;
      ++lengthStr;
    } else {
      parsedStr[lengthStr] = '\0';
      lengthStr = 0;
      if (word.includes(String(parsedStr))) {
        ++cnt;
      };
    }
  }

  std::cout << cnt << std::endl;

  return EXIT_SUCCESS;
}
