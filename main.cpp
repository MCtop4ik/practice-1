#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

#define unless(cond) if (!(cond))

char *parseArg(int argc, char *argv[], const char *targetArg) {
  bool isTarget = false;
  char *argument = {};
  for (int i = 0; i < argc; ++i) {
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

namespace StringUtils {
  bool isLowerCaseLetter(char ch) {
    return (ch >= 'a' && ch <= 'z');
  }

  bool isUpperCaseLetter(char ch) {
    return (ch >= 'A' && ch <= 'Z');
  }

  bool isLetter(char ch) {
    return isLowerCaseLetter(ch) || isUpperCaseLetter(ch);
  }
}

class String {
private:
  char str[64];
  size_t ln;
  const unsigned int kEnglishAlphabetLength = 26;

public:
  String(char *initStr) { 
    ln = strlen(initStr); 
    strncpy(str, initStr, ln);
    str[ln] = '\0';
  }

  char operator[](int index) const {
    return str[index];
  }

  char at(int index) const {
    if (index >= ln || index < 0)
      throw std::out_of_range("Wrong index for String");
    return str[index];
  }

  void log() const {
    std::cout << "String: " << str << std::endl;
    std::cout << "String Length: " << ln << std::endl;
  }

  char *getChars() { return str; }

  size_t getLength() const { return ln; }

  void initMask(int* mask) {
    for (int i = 0; i < kEnglishAlphabetLength; ++i) {
      mask[i] = 0;
    }
  }

  void createMask(const String& maskStr, int* mask) {
    char sym = {};
    const unsigned int kUpperLettersShift = kEnglishAlphabetLength;
    for (int i = 0; i < static_cast<int>(maskStr.getLength()); i++) {
      sym = maskStr.at(i);
      if (StringUtils::isLowerCaseLetter(sym)) {
        mask[sym - 'a'] = 1;
      }
      if (StringUtils::isUpperCaseLetter(sym)) {
        mask[sym - 'A' + kUpperLettersShift] = 1;
      }
    }
  }

  bool includes(const String& other) {
    int mask1[54];
    int mask2[54];

    initMask(mask1);
    initMask(mask2);
          
    createMask(*this, mask1);
    createMask(other, mask2);

    for (int i = 0; i < kEnglishAlphabetLength; ++i) {
      unless (mask1[i] <= mask2[i]) {
        return false;
      }
    }
    return true;
  }
};



int main(int argc, char *argv[]) {
  String kWord = String(parseArg(argc, argv, "--word"));
  String kFilepath = String(parseArg(argc, argv, "--file"));

  char parsedStr[64];
  unsigned int lengthStr = 0;

  unsigned int cnt = 0;
  
  char inputChar;
  std::ifstream in;
  in.open(kFilepath.getChars());
  while (in.get(inputChar)) {
    if (StringUtils::isLetter(inputChar)) {
      parsedStr[lengthStr] = inputChar;
      ++lengthStr;
    } else {
      parsedStr[lengthStr] = '\0';
      lengthStr = 0;
      if (kWord.includes(String(parsedStr))) {
        ++cnt;
      };
    }
  }

  std::cout << cnt << std::endl;

  return EXIT_SUCCESS;
}
