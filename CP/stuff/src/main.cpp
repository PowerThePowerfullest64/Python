#include <iostream>
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

constexpr std::string_view archive_ext = ".dns";

uint64_t GetFileSize(const std::string& inputPath) {
    FILE* f = fopen(inputPath.c_str(), "rb");
    if (f == nullptr) return -1;

    fseek(f, 0L, SEEK_END);

    uint64_t size = ftell(f);

    fclose(f);

    return size;
}

int PackFile(FILE* out, const std::string& name) {
    uint64_t datasize = GetFileSize(name);
    uint16_t namesize = name.size();

    FILE* in = fopen(name.c_str(), "rb");
    if (in == nullptr) return -1;

    std::vector<char> data(datasize);
    if (fread(data.data(), 1, datasize, in) != datasize) {
        fclose(in);
        return -1;
    }

    fwrite(&namesize, 2, 1, out);
    fwrite(name.c_str(), 1, namesize, out);
    fwrite(&datasize, 8, 1, out);
    fwrite(data.data(), 1, datasize, out);

    fclose(in);

    return 0;
}

int UnpackFile(FILE* in, const fs::path& root_dir) {
    uint64_t datasize;
    uint16_t namesize;

    if (fread(&namesize, 2, 1, in) != 1) {
        if (feof(in)) return 1;

        return -1;
    }

    std::string relative_name(namesize, '\0');
    if (fread(&relative_name[0], 1, namesize, in) != namesize) return -1;

    fread(&datasize, 8, 1, in);

    std::vector<char> data(datasize);
    if (datasize > 0) if (fread(data.data(), 1, datasize, in) != datasize) return -1;

    fs::path final_output_path = root_dir / relative_name;

    fs::path parent_folder = final_output_path.parent_path();
    if (!parent_folder.empty()) {
        fs::create_directories(parent_folder);
    }

    std::cout << "Unpacking: " << final_output_path.string() << "\n";

    FILE* out = fopen(final_output_path.string().c_str(), "wb");
    if (out == nullptr) {
        std::cerr << "Failed to open output file: " << final_output_path.string() << "\n";
        return -1;
    }

    if (datasize > 0)
        fwrite(data.data(), 1, datasize, out);

    fclose(out);
    return 0;
}

int PackData(const std::string& name) {
    fs::path root_dir = name;
    fs::path root_name = root_dir.filename();

    fs::path archive_path = fs::path(name);
    archive_path += archive_ext;

    FILE* out = fopen(archive_path.string().c_str(), "wb");
    if (out == nullptr) return -1;

    for (const auto& entry : fs::recursive_directory_iterator(root_dir)) {
        if (entry.is_regular_file()) {
            fs::path full_path = entry.path();

            fs::path relative_inside_path = fs::relative(full_path, root_dir);
            fs::path relative_with_root = root_name / relative_inside_path;

            if (PackFile(out, relative_with_root.string()) == 0)
                std::cout << "Archived file: '" << relative_with_root << "' successfully\n";
            else {
                std::cout << "Didn't archive file: " << relative_with_root.string() << " successfully\n";
                fclose(out);
                return -1;
            }
        }
    }

    fclose(out);
    return 0;
}

int UnpackData(const std::string& name) {
    // remove archive extension
    fs::path root_dir = name.substr(0, name.size() - archive_ext.size());

    FILE* in = fopen(name.c_str(), "rb");
    if (in == nullptr) return -1;

    while (true) {
        int code = UnpackFile(in, root_dir);
        if (code == 1) break;
        if (code == -1) {
            std::cout << "Error unpacking file\n";
            fclose(in);
            return -1;
        }
    }

    fclose(in);
    return 0;
}

int main() {
    //PackData("folder");
    UnpackData("folder.dns");
}
