//
// Created by 苏畅 on 2019/1/15.
//

#ifndef SNIFFER_UTIL_H
#define SNIFFER_UTIL_H

#include <map>
#include <string>

inline bool isLegal(char s) {
    if (s == ':' || s == '\r' || s == '\n') {
        return false;
    }
    return true;
};

inline std::map<std::string, std::string> ParseHttpHeader(std::string raw) {
    enum class ParseMetaStatus {
        NONE,
        KEY,
        VALUE,
        BETWEEN
    };
    std::string tmp_key;
    std::string tmp_value;

    ParseMetaStatus status = ParseMetaStatus::NONE;

    auto m = std::map<std::string, std::string>();

    for (auto i : raw) {
        if (status == ParseMetaStatus::NONE && isalpha(i)) {
            status = ParseMetaStatus::KEY;
            tmp_key.push_back(i);
        } else if (status == ParseMetaStatus::KEY && i == ':') {
            status = ParseMetaStatus::BETWEEN;
        } else if (status == ParseMetaStatus::KEY) {
            tmp_key.push_back(i);
        } else if (status == ParseMetaStatus::BETWEEN && isLegal(i)) {
            status = ParseMetaStatus::VALUE;
            tmp_value.push_back(i);
        } else if (status == ParseMetaStatus::VALUE && i == '\r') {
            status = ParseMetaStatus::NONE;
            m.insert(std::pair<std::string, std::string>(tmp_key, tmp_value));
            tmp_key.clear();
            tmp_value.clear();
        } else if (status == ParseMetaStatus::VALUE) {
            tmp_value.push_back(i);
        }
    }
    return m;
}
#endif //SNIFFER_UTIL_H
