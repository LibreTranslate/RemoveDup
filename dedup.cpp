#include "dedup.hpp"

std::tuple<std::string, std::string, size_t> dedup(const std::string &src, const std::string &tgt){
    std::ifstream src_if(src, std::ios_base::in | std::ios_base::binary);
    if (!src_if.is_open()) throw std::runtime_error("Cannot open " + src);
    
    std::ifstream tgt_if(tgt, std::ios_base::in | std::ios_base::binary);
    if (!tgt_if.is_open()) throw std::runtime_error("Cannot open " + tgt);
    
    std::unordered_map<std::uint32_t, bool> seen;

    std::string src_out = src + ".dedup";
    std::string tgt_out = tgt + ".dedup";

    std::ofstream src_of(src_out, std::ios::trunc);
    std::ofstream tgt_of(tgt_out, std::ios::trunc);

    std::string line_s = "";
    std::string line_t = "";
    size_t removed = 0;
    
    while(true) {
        std::getline(src_if, line_s);
        std::getline(tgt_if, line_t);
        if (src_if.eof()) break;

        std::uint32_t hash = xxh32::hash(line_s.c_str(), static_cast<uint32_t>(line_s.size()), 0);

        bool line_exists = seen.count(hash) > 0;
        if (!line_exists) {
            seen[hash] = true;
            src_of << line_s << "\n";
            tgt_of << line_t << "\n";
        }else{
            removed++;
        }

        
    }

    src_of.close();
    tgt_of.close();
    src_if.close();
    tgt_if.close();

    return std::make_tuple(src_out, tgt_out, removed);
}