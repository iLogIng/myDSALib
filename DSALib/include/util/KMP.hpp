#pragma once
#include <string>
#include "../Linear/Array/DynArray.hpp"

namespace myDSALib
{
namespace util
{

class StringCompare
{
    template<typename Ty>
    using Array = myDSALib::Linear::DynArray<Ty>;
private:
    std::string mainStr;
    std::string subStr;
    Array<size_t> next;

private:
    // create next array
    void createNext() {
        size_t len = 0; // The main string pointer
        size_t pos = 1; // The sub/comp/mod string pointer
        while(pos != subStr.size()) {
            if(subStr[pos] == subStr[len]) {
                ++len;
                next[pos] = len;
                ++pos;
            }
            else {
                if(len != 0) {
                    len = next[len - 1];
                }   // main string pointer back to the head
                else {
                    next[pos] = 0;
                    ++pos;
                }
            }
        }
    }

    // renew the next array
    void renewNext() {
        next.clear();
        if(!subStr.empty()) {
            next.resize(subStr.size());
            return;
        }
    }

public:
    StringCompare(const std::string& comp, const std::string& sub)
        : mainStr(comp), subStr(sub), next(sub.size(), 0) { createNext(); }

    StringCompare(const StringCompare&) = delete;
    StringCompare& operator=(const StringCompare&) = delete;

    StringCompare(StringCompare&& other)
        : mainStr(std::move(other.mainStr)), subStr(std::move(other.subStr)), next(std::move(other.next)) { }
    StringCompare& operator=(StringCompare&& other) {
        if(this != &other) {
            this->mainStr = std::move(other.mainStr);
            this->subStr = std::move(other.subStr);
            this->next = std::move(other.next);
        }
        return *this;
    }

public:
    // reset mainStr
    void setMainStr(const std::string& str) {
        this->mainStr = str;
    }
    // get mainStr
    std::string& getMainStr() {
        return mainStr;
    }
    // get mainStr const.Ver
    const std::string& getMainStr() const {
        return mainStr;
    }

    // reset subStr
    void setSubStr(const std::string& sub) {
        this->subStr = sub;
        renewNext();
    }
    // get subStr
    std::string& getSubStr() {
        return subStr;
    }
    // get subStr const.Ver
    const std::string& getSubStr() const {
        return subStr;
    }

    const Array<size_t>& getNext() const {
        return next;
    }

    // compare index
    size_t compareIndex() {
        if(mainStr.size() == 0) { return 0; }
        if(subStr.size() == 0) { return std::string::npos; }

        size_t mpos = 0; // mainStr pos
        size_t spos = 0; // subStr pos

        while(mpos != mainStr.size()) {
            if(mainStr[mpos] == subStr[spos]) {
                ++mpos;
                ++spos;

                if(spos == subStr.size()) {
                    return mpos - spos;
                }
            }
            else {
                if(spos != 0) {
                    spos = next[spos - 1];
                }
                else {
                    ++mpos;
                }
            }
        }

        return std::string::npos;
    }

    // all matches
    Array<size_t> findAllMatches() {
        Array<size_t> matches;
        if(subStr.empty() || mainStr.empty()) {
            return matches;
        }

        size_t mpos = 0;
        size_t spos = 0;

        while(mpos != mainStr.size()) {
            if(mainStr[mpos] == subStr[spos]) {
                ++mpos;
                ++spos;

                if(spos == subStr.size()) {
                    matches.push_back(mpos - spos);
                    spos = next[spos - 1];
                }
            }
            else {
                if(spos != 0) {
                    spos = next[spos - 1];
                }
                else {
                    ++mpos;
                }
            }
        }
        return matches;
    }

    // count all matches
    size_t countMatches() {
        return findAllMatches().getSize();
    }

    // contain
    bool contains() {
        return compareIndex() != std::string::npos;
    }

};

}
}

