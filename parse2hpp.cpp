#include <fstream>
#include <string>
#include <iostream>

enum STATE {Empty, KeyWaiting, KeyLoading, ValueWaiting, ValueLoading, ArrayLoading};

int main(int argc, char** argv) {
  std::ifstream file(argv[1]);

  std::ofstream ofile;

  if (argc == 3) {
    ofile.open(argv[2], std::ios::out);
    std::cout << "Parsing " << argv[1] << " into " << argv[2] << std::endl;
  }
  std::ostream & output = (argc == 3) ? ofile : std::cout;

  STATE state = Empty;

  int indent = 0;

  char c;
  std::string bufferK;
  std::string bufferV;
  while (file.get(c)) {
    switch (state) {
      case Empty :
        switch (c) {
          case '{':
            state = KeyWaiting;
            std::cout << "KeyWaiting" << std::endl;
            break;
        }
        break;
      case KeyWaiting:
        switch (c) {
          case '"':
            state = KeyLoading;
            std::cout << "KeyLoading" << std::endl;
            if (bufferK != "") {
              output << std::string(indent, ' ') << "class " << bufferK << "\n" << std::string(indent, ' ') << "{\n";
              bufferK = "";
              indent += 2;
            }
            break;
          case '}':
            std::cout << "Empty Class" << std::endl;
            if (bufferK != "") {
              output << std::string(indent, ' ') << "class " << bufferK << "{};\n";
              bufferK = "";
            } else {
              if (indent != 0) {
                indent -= 2;
                output << "};";
              }
            }
        }
        break;
      case KeyLoading:
        switch (c) {
          case '"':
            std::cout << "ValueWaiting - Done Loading key" << std::endl;
            state = ValueWaiting;
            break;
          default:
            bufferK += c;
        }
        break;
      case ValueWaiting:
        switch (c) {
          case ':':
            break;
          case '{':
            std::cout << "KeyWaiting" << std::endl;
            state = KeyWaiting;
            break;
          case '[':
            std::cout << "ArrayLoading" << std::endl;
            state = ArrayLoading;
            bufferK += "[]";
          case ' ':
            break;
          default:
            std::cout << "ValueLoading" << std::endl;
            state = ValueLoading;
            bufferV += c;
        }
        break;
      case ValueLoading:
        switch (c) {
          case '}':
            std::cout << "END class" << std::endl;
            state = KeyWaiting;
            output << std::string(indent, ' ') << bufferK << " = " << bufferV << ";\n";
            indent -= 2;
            output << std::string(indent, ' ') << "};\n";
            bufferK = "";
            bufferV = "";
            break;
          case ',':
            std::cout << "NEXT class" << std::endl;
            state = KeyWaiting;
            output << std::string(indent, ' ') << bufferK << " = " << bufferV << ";\n";
            bufferK = "";
            bufferV = "";
            break;
          default:
            bufferV += c;
        }
        break;
      case ArrayLoading:
        switch (c) {
          case '}':
            std::cout << "END class" << std::endl;
            state = KeyWaiting;
            output << std::string(indent, ' ') << bufferK << " = {" << bufferV << "};\n";
            indent -= 2;
            output << std::string(indent, ' ') << "};\n";
            bufferK = "";
            bufferV = "";
            break;
          case ']':
            std::cout << "NEXT class" << std::endl;
            state = KeyWaiting;
            output << std::string(indent, ' ') << bufferK << " = {" << bufferV << "};\n";
            bufferK = "";
            bufferV = "";
            break;
          case '[':
            break;
          default:
            bufferV += c;
        }
        break;
    }
  }
  output << "\n";
}
