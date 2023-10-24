#include "parser.cpp"
#include <fstream>
#include <string>
#include <cerrno>

std::string get_file_contents(const char *filename) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in) {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

int main(int argc, char* argv[]) {
    char *a = argv[1];
    std::string test = get_file_contents(a);
    Paragraph p = parse(test);
    if (p.is_valid()) {
        std::cout << "The paragraph is valid\n\n";
    }

    else std::cout << "The paragraph is invalid\n\n";

    std::cout << "The sentences are:\n\n";

    for (int i = 0; i < p.sentences.size(); i++) {
        p.sentences[i].show();
        p.sentences[i].display_tokens();
        std::cout << std::endl;
    }

    return 0;
}
