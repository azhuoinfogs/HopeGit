#include <iostream>
#include <fstream>
#include <stdexcept>

class FileHandler {
public:
    FileHandler(const std::string& filename) {
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file.");
        }
        std::cout << "File opened successfully.\n";
    }

    ~FileHandler() {
        if (file.is_open()) {
            file.close();
            std::cout << "File closed automatically.\n";
        }
    }

    void writeToFile(const std::string& content) {
        if (file.is_open()) {
            file << content << std::endl;
        }
    }

private:
    std::ofstream file;
};

int main() {
    try {
        FileHandler fh("example.txt");
        fh.writeToFile("Hello, World!");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    // The file is closed automatically here as the FileHandler object goes out of scope.
    return 0;
}
