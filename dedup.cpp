#include "dedup.hpp"


std::tuple<std::string, std::string, size_t> dedup(const std::string &src, const std::string &tgt){
    #if defined(_WIN32) || defined(_WIN64)
    HANDLE file_handle = CreateFile(src.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Unable to open " + src);
    }

    LARGE_INTEGER li;
    GetFileSizeEx(file_handle, &li);
    SIZE_T file_size = li.QuadPart;
    HANDLE file_mapping = CreateFileMapping(file_handle, NULL, PAGE_READONLY, 0, 0, NULL);
    if (file_mapping == NULL) {
        CloseHandle(file_handle);
        throw std::runtime_error("File mapping failed.");
    }

    const char* file_data = static_cast<const char*>(MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, file_size));
    if (file_data == NULL) {
        CloseHandle(file_mapping);
        CloseHandle(file_handle);
        throw std::runtime_error("Memory mapping failed.");
    }
#else
    int fd = open(src.c_str(), O_RDONLY);
    if (fd == -1) throw std::runtime_error("Unable to open " + src);

    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        close(fd);
        throw std::runtime_error("Unable to get file size for " + src);
    }

    off_t file_size = file_stat.st_size;
    const char* file_data = static_cast<const char*>(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (file_data == MAP_FAILED) {
        close(fd);
        throw std::runtime_error("Memory mapping failed.");
    }

#endif

    std::unordered_multimap<std::uint32_t, std::string_view> lines;

    FILE* tgt_fp = fopen(tgt.c_str(), "rb");
    if (tgt_fp == NULL) throw std::runtime_error("Cannot open " + tgt);
    
    std::string src_out = src + ".dedup";
    std::string tgt_out = tgt + ".dedup";

    std::ofstream src_of(src_out, std::ios::trunc);
    std::ofstream tgt_of(tgt_out, std::ios::trunc);

    const char* line_start = file_data;
    const char* line_end = file_data;

    const int BUF_SIZE = 4096;
    char line_buf[BUF_SIZE];
    size_t removed = 0;

    for (const char* ptr = file_data; ptr < file_data + file_size; ++ptr) {
        if (*ptr == '\n' || ptr == file_data + file_size - 1) {
            fgets(line_buf, BUF_SIZE, tgt_fp);

            line_end = ptr + 1;
            std::string_view line_view(line_start, line_end - line_start);

            std::uint32_t hash = xxh32::hash(line_start, static_cast<uint32_t>(line_end - line_start), 0);
            auto range = lines.equal_range(hash);

            bool line_exists = false;
            for (auto it = range.first; it != range.second; ++it) {
                if (it->second == line_view) {
                    line_exists = true;
                    break;
                }
            }

            if (!line_exists) {
                lines.emplace(hash, line_view);
                if (*(ptr - 1) == '\r'){
                    std::string_view lv(line_start, line_end - line_start - 2);
                    src_of << lv << "\r\n";
                }else{
                    src_of << line_view;
                }
                tgt_of << line_buf;
            }else{
                removed++;
            }

            line_start = line_end;
        }
    }

#if defined(_WIN32) || defined(_WIN64)
    UnmapViewOfFile(file_data);
    CloseHandle(file_mapping);
    CloseHandle(file_handle);
#else
    munmap(const_cast<char*>(file_data), file_stat.st_size);
    close(fd);
#endif

    src_of.close();
    tgt_of.close();
    fclose(tgt_fp);

    return std::make_tuple(src_out, tgt_out, removed);
}