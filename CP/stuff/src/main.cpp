#include <iostream>
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>

uint64_t GetFileSize(const std::string& inputPath) {
    FILE* f = fopen(inputPath.c_str(), "rb");
    if (f == nullptr) return -1;

    fseek(f, 0L, SEEK_END);

    uint64_t size = ftell(f);

    fclose(f);

    return size;
}

int PackFile(FILE* out, const std::string& name, const std::string& inputPath) {
    uint64_t datasize = GetFileSize(inputPath);
    uint32_t namesize = name.size();

    FILE* in = fopen(inputPath.c_str(), "rb");
    if (in == nullptr) return -1;

    std::vector<char> data(datasize);
    if (fread(data.data(), 1, datasize, in) != datasize) {
        fclose(in);
        return -1;
    }

    fwrite(&namesize, 4, 1, out);
    fwrite(name.c_str(), 1, namesize, out);
    fwrite(&datasize, 8, 1, out);
    fwrite(data.data(), 1, datasize, out);

    fclose(in);

    return 0;
}

int UnpackFile(const std::string& inputPath) {
    void* name;
    void* data;

    uint64_t datasize;
    uint32_t namesize;

    FILE* in = fopen(inputPath.c_str(), "rb");
    if (in == nullptr) return -1;

    std::cout << "Opened input path\n";

    fread(&namesize, 4, 1, in);
    fread(&datasize, 8, 1, in);
    fread(name, 1, namesize, in);
    fread(data, 1, datasize, in);

    fclose(in);

    std::cout << "Read data\n";

    FILE* out = fopen(reinterpret_cast<const char*>(name), "wb");
    if (out == nullptr) return -1;

    std::cout << "Opening output path\n";

    fwrite(data, 1, datasize, out);
    fclose(out);

    return 0;
}

int main() {
    FILE* out = fopen("output.dns", "wb");

    //PackFile(out, "input.txt", "C:\\GitHub\\Python\\CP\\stuff\\input.txt");
    int code = UnpackFile("C:\\GitHub\\Python\\CP\\stuff\\output.dns");
    if (code == -1) std::cout << "Error\n";

    fclose(out);
}
