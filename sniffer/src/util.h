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

inline std::string & ToLower(std::string& s)
{
    int len=s.size();
    for(int i=0;i<len;i++){
        if(s[i]>='A'&&s[i]<='Z'){
            s[i]+=32;
        }
    }
    return s;
}

inline std::map<std::string, std::string> ParseHttpHeader(std::string raw) {
    enum ParseMetaStatus {
        NONE,
        KEY,
        VALUE,
        BETWEEN
    };
    std::string tmp_key;
    std::string tmp_value;
    std::string raw_header;
    std::string raw_meta;

    ParseMetaStatus status = NONE;
    unsigned long pos=raw.find("\r\n\r\n");
    if (pos != std::string::npos) {
        std::string body = raw.substr(pos+4);
    }
    unsigned long pos_header=raw.find("\r\n");
    if (pos != std::string::npos) {
        raw_header = raw.substr(pos_header + 2, pos - pos_header);
        raw_meta = raw.substr(0, pos_header);
    }

    std::map<std::string, std::string> m = std::map<std::string, std::string>();

    for (int i =0; i<raw_header.size(); i++){
        char item = raw_header[i];
        if (status == NONE && isalpha(item)) {
            status = KEY;
            tmp_key.push_back(item);
        } else if (status == KEY && item == ':') {
            status = BETWEEN;
        } else if (status == KEY) {
            tmp_key.push_back(item);
        } else if (status == BETWEEN && isLegal(item)) {
            status = VALUE;
            tmp_value.push_back(item);
        } else if (status == VALUE && item == '\r') {
            status = NONE;
            tmp_key = ToLower(tmp_key);
            m.insert(std::pair<std::string, std::string>(tmp_key, tmp_value));
            tmp_key.clear();
            tmp_value.clear();
        } else if (status == VALUE) {
            tmp_value.push_back(item);
        }
    }
    return m;
}
#endif //SNIFFER_UTIL_H
