#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using std::vector;
using std::string;
using std::cout;
using std::endl;
//function to split the string 'line' into 'words' by space character
void SplitLine(const string &line, vector<string> &words) {
  std::istringstream str(line);
  std::istream_iterator<string> begin(str), end;

  std::copy(begin, end, std::back_inserter(words));
}
//function to print 'lines' centred in a field of size 'size'
void Print(const vector<string> &lines, unsigned int size) {
  for (auto& l : lines) {
    cout.width((size - l.length()) / 2 + l.length());
    cout << l << endl;
  }
}
//function for preprocessing a string 'line' and its centred output in a field of size 'size'
void Alignment(const string &line, unsigned int size) {
  vector<string> words, lines;
  string str = "";

  if (line.length() <= size)
    lines.push_back(line);//suitable length
  else {
    SplitLine(line, words);
    for (auto& w : words) {
      if (str.length() + w.length() <= size) {
        str += w + " ";
      }
      else {
        //a new word does not fit
        if (str.length() > 0) {
          str.erase(str.length() - 1, 1);
          lines.push_back(str);
        }
        if (w.length() <= size) {
          str = w + " ";
        }
        else {
          unsigned int length = w.length();
          //too long word
          while (length > size) {
            str = w.substr(w.length() - length, size);
            lines.push_back(str);
            length -= size;
          }
          str = w.substr(w.length() - length, length) + " ";
        }
      }
    }
    str.erase(str.length() - 1, 1);
    lines.push_back(str);
  }
  Print(lines, size);
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cout << "Wrong format! Format: <programm name> <file name> <size>. <size> must be positive integer" << endl;
    return 1;
  }

  std::ifstream in(argv[1]);
  if (!in.is_open()) {
    cout << "File open error" << endl;
    return 2;
  }

  int n = atoi(argv[2]);
  if (n <= 0) {
    cout << "Wrong format! Format: <programm name> <file name> <size>. <size> must be positive integer" << endl;
    return 3;
  }

  string line;
  unsigned int size = static_cast<unsigned int>(n);

  while (std::getline(in, line)) {
    Alignment(line, size);
  }

  in.close();

  return 0;
}