
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <filename> <size>" << std::endl;
        return 1;
    }
    std::ofstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Could not open file \"" << argv[1] << "\"" << std::endl;
        return 2;
    }
    size_t sz = std::stoull(argv[2]);
    for (size_t i = 0; i < sz; i++)
        file << 'a';
    file.close();
    return 0;
}