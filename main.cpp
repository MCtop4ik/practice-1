#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

char *ParseArg(int argc, char *argv[], const char *targetArg) {
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
  bool IsLowerCaseLetter(char ch) {
    return (ch >= 'a' && ch <= 'z');
  }

  bool IsUpperCaseLetter(char ch) {
    return (ch >= 'A' && ch <= 'Z');
  }

  bool IsLetter(char ch) {
    return IsLowerCaseLetter(ch) || IsUpperCaseLetter(ch);
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

  char At(int index) const {
    if (index >= ln || index < 0)
      throw std::out_of_range("Wrong index for String");
    return str[index];
  }

  void Log() const {
    std::cout << "String: " << str << std::endl;
    std::cout << "String Length: " << ln << std::endl;
  }

  char *GetChars() { return str; }

  size_t GetLength() const { return ln; }

  void InitMask(int* mask) {
    for (int i = 0; i < kEnglishAlphabetLength; ++i) {
      mask[i] = 0;
    }
  }

  void CreateMask(const String& maskStr, int* mask) {
    char sym = {};
    const unsigned int kUpperLettersShift = kEnglishAlphabetLength;
    for (int i = 0; i < static_cast<int>(maskStr.GetLength()); i++) {
      sym = maskStr.At(i);
      if (StringUtils::IsLowerCaseLetter(sym)) {
        mask[sym - 'a'] = 1;
      }
      if (StringUtils::IsUpperCaseLetter(sym)) {
        mask[sym - 'A' + kUpperLettersShift] = 1;
      }
    }
  }

  bool Includes(const String& other) {
    int mask1[54];
    int mask2[54];

    InitMask(mask1);
    InitMask(mask2);
          
    CreateMask(*this, mask1);
    CreateMask(other, mask2);

    for (int i = 0; i < kEnglishAlphabetLength; ++i) {
      if (!(mask1[i] <= mask2[i])) {
        return false;
      }
    }
    return true;
  }
};

int main(int argc, char *argv[]) {
  String kWord = String(ParseArg(argc, argv, "--word"));
  String kFilepath = String(ParseArg(argc, argv, "--file"));

  unsigned int cnt = 0;
  
  char inputWord[64];
  std::ifstream in;
  in.open(kFilepath.GetChars());
  while (in.getline(inputWord, 64, ' ')) {
    if (kWord.Includes(String(inputWord))) {
        ++cnt;
    };
  }

  std::cout << cnt << std::endl;

  return EXIT_SUCCESS;
}
