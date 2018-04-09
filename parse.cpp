#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>

bool startswith(std::string &source, std::string compare) {
  return source.substr(0, compare.size()) == compare;
}

bool endswith(std::string const & value, std::string const & ending)
{
  if (ending.size() > value.size()) return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
  s.erase(0, s.find_first_not_of(t));
  return s;
}

inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
  return ltrim(rtrim(s, t), t);
}

void replace(std::string& str, const std::string& from, const std::string& to) {
  size_t start_pos = 0;
  do {
    start_pos = str.find(from, start_pos);
    if(start_pos == std::string::npos)
      return;
    str.replace(start_pos, from.length(), to);
    start_pos += to.size();
  } while (start_pos != std::string::npos);
}

enum LAST { CLASS, ARRAY, PROPERTY };

int main(int argc, char** argv)
{
  std::ifstream file(argv[1]);
  std::ofstream output(argv[2]);
  std::cout << "Parsing " << argv[1] << " into " << argv[2] << std::endl;
  std::string str;
  LAST last;
  output << "{";
  bool emptyClass = true;
  bool classEnd = false;
  while (std::getline(file, str))
  {
    trim(str);
    replace(str, "\\", "\\\\");
    replace(str, "\t","  ");

    if (!startswith(str, "//") && !startswith(str, "#")) {
      if (startswith(str, "class ")) {
        if (last == PROPERTY || last == ARRAY) {
          output << ", ";
        }
        if (emptyClass && classEnd) {
          output << ", ";
        }
        if (endswith(str, "{};")) {
          //Empty Class
          output << "\"" << str.substr(6, str.size() - 9) << "\": {},";
        } else {
          output << "\"" << str.substr(6, str.size() - 6) << "\":";
        }
        emptyClass = true;
        classEnd = false;
        last = CLASS;
      } else {
        std::size_t found = str.find("=");
        if (found != std::string::npos) {
          emptyClass = false;
          std::size_t array = str.find("[]");
          if (array != std::string::npos && array < found) {
            //Array
            if (last != CLASS || (last == CLASS && classEnd)) {
              output << ", ";
            }
            output << "\"" << str.substr(0, array) << "\": [";
            std::string value = str.substr(found + 3, str.size() - found - 5);
            replace(value, "{", "[");
            replace(value, "}", "]");

            {
              std::string newvalue = "";
              bool stringOpen = false;
              for (int i = 0; i < value.size(); i++) {
                if (stringOpen) {
                  if (value[i] == '"') {
                    if (value[i + 1] == '"') {
                      newvalue += "\\\"\\\"";
                      i++;
                      continue;
                    } else {
                      stringOpen = false;
                    }
                  }
                } else {
                  if (value[i] == '"') {
                    stringOpen = true;
                  }
                }
                newvalue += value[i];
              }
              value = newvalue;
            }

            output << value << "]";
            last = ARRAY;
          } else {
            //Property
            if (last != CLASS || (last == CLASS && classEnd)) {
              output << ", ";
            }
            std::string value = str.substr(found + 2, str.size() - found - 3);
            if (value != "\"\"") {
              if (startswith(value, "\"\"\"")) {
                value = value.substr(1, value.size() - 1);
                replace(value, "\"\"", "\\\"\\\"");
                value = "\"" + value;
              } else {
                replace(value, "\"\"", "\\\"\\\"");
              }
            }
            output << "\"" << str.substr(0, found - 1) << "\": " << value;
            last = PROPERTY;
          }
        } else {
          if (str == "};") {
            output << "}";
            classEnd = true;
          } else {
            output << str << std::endl;
          }
        }
      }
    }
  }
  output << "}" << std::endl;
  output.close();
}
