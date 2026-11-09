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
    uint64_t size = GetFileSize(inputPath);
    uint32_t namesize = name.size();

    FILE* in = fopen(inputPath.c_str(), "rb");
    if (in == nullptr) return -1;

    std::vector<char> data(size);
    if (fread(data.data(), 1, size, in) != size) {
        fclose(in);
        return -1;
    }

    fwrite(&namesize, 4, 1, out);
    fwrite(name.c_str(), 1, namesize, out);
    fwrite(&size, 8, 1, out);
    fwrite(data.data(), 1, size, out);

    fclose(in);

    return 0;
}

int main() {
    FILE* out = fopen("output.txt", "wb");
    PackFile(out, "input.txt", "C:\\Users\\Brondum\\Documents\\GitHub\\Python\\CP\\stuff\\input.txt");

    fclose(out);
}
