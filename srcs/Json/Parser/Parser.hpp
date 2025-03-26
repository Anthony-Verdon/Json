#pragma once

#include "Json/Node/Node.hpp"
#include <string>

typedef std::string::iterator stringIt;

namespace Json
{
    Node ParseFile(const std::string &path);
    Node ParseJson(const std::string &text, stringIt &it);

    std::pair<std::string, Node> RetrieveKeyValuePair(const std::string& text, stringIt& it); 
    std::string RetrieveKey(const std::string& text, stringIt& it);
    Node RetrieveValue(const std::string& text, stringIt& it);
    Node ParseArray(const std::string &text, stringIt &it);
    Node ParsePrimitive(const std::string &text, stringIt &start, stringIt &end);
    Node ParseString(const std::string &text, stringIt &start, stringIt &end);
    Node ParseKeyword(const std::string &text, stringIt &it);

    void SkipWhitespace(const std::string &text, stringIt &it);
}