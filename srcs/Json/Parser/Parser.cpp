#include "Json/Parser/Parser.hpp"
#include "Toolbox.hpp"
#include <cassert>
#include <cctype> 
#include <iostream>

namespace Json
{
    Node ParseFile(const std::string &path)
    {
        if (!Toolbox::checkExtension(path, ".json"))
            std::cerr << path << " has wrong extension" << std::endl;

        std::string text = Toolbox::readFile(path);

        stringIt start = text.begin();
        return (ParseJson(text, start));
    }

    Node ParseJson(const std::string &text, stringIt &it)
    {
        assert(it != text.end());
        assert(*it == '{');
        it++;

        NodeMap json;
        while (*it != '}')
        {
            const auto [key, value] = RetrieveKeyValuePair(text, it);
            json[key] = value;

            SkipWhitespace(text, it);
        }
        it++;

        return json;
    }

    std::pair<std::string, Node> RetrieveKeyValuePair(const std::string& text, stringIt& it)
    {
        return (std::make_pair(RetrieveKey(text, it), RetrieveValue(text, it)));
    }

    std::string RetrieveKey(const std::string& text, stringIt& it)
    {
        SkipWhitespace(text, it);
        assert(it != text.end());

        stringIt currentIt;

        if (*it == '\"')
        {
            currentIt = ++it;
            while (it != text.end() && *it != '\"')
                it++;
        }

        assert(it != text.end());
        assert(*it == '\"');
        std::string key = text.substr(currentIt - text.begin(), it - currentIt);
        it++;
        return (key);
    }

    Node RetrieveValue(const std::string& text, stringIt& it)
    {
        SkipWhitespace(text, it);
        assert(it != text.end());
        if (*it == ':')
            it++;
        SkipWhitespace(text, it);
        assert(it != text.end());
        
        stringIt currentIt;
        Node value;
        if (*it == '{')
        {
            value = ParseJson(text, it);
        }
        else if (*it == '[')
        {
            it++;
            value = ParseArray(text, it);
        }
        else if (*it == '-' || isdigit(*it) || *it == 'e' || *it == 'E')
        {
            currentIt = it;
            while (it != text.end() && (*it == '-' || isdigit(*it) || *it == '.' || *it == 'e' || *it == 'E'))
                it++;
            value = ParsePrimitive(text, currentIt, it);
        }
        else if (*it == '"')
        {
            currentIt = ++it;
            while (it != text.end() && *it != '"')
                it++;
            value = ParseString(text, currentIt, it);
            it++;
        }
        else
        {
            value = ParseKeyword(text, it);
        }

        assert(it != text.end());
        if (*it == ',')
            it++;

        return (value);
    }
    
    Node ParseArray(const std::string &text, stringIt &it)
    {
        NodeArray values;

        while (it != text.end() && *it != ']')
        {
            const auto value = RetrieveValue(text, it);
            values.push_back(value);

            SkipWhitespace(text, it);
        }
        assert(it != text.end());
        it++;

        return (values);
    }

    Node ParsePrimitive(const std::string &text, stringIt &start, stringIt &end)
    {
        assert(start != text.end() && end != text.end());
        std::string substr = text.substr(start - text.begin(), end - start);
        int pointIndex = substr.find(".");

        if (pointIndex == -1) // integer
            return std::stoi(substr);
        else                  // float(double)
            return std::stod(substr);
    }

    Node ParseString(const std::string &text, stringIt &start, stringIt &end)
    {
        assert(start != text.end() && end != text.end());
        return (text.substr(start - text.begin(), end - start));
    }

    Node ParseKeyword(const std::string &text, stringIt &it)
    {
        assert(it != text.end());
        stringIt currentIt = it;
        for (int i = 0; i < 4; i++)
        {
            it++;
            assert(it != text.end());
        }

        if (text.substr(currentIt - text.begin(), it - currentIt) == "true")
            return (true);
        else if (text.substr(currentIt - text.begin(), it - currentIt) == "null")
            return (nullptr);
        else
        {
            it++;
            assert(it != text.end());
            if (text.substr(currentIt - text.begin(), it - currentIt) == "false")
                return (false);
            else
                return (nullptr);
        }
    }

    void SkipWhitespace(const std::string &text, stringIt &it)
    {
        static const std::string whitespace = "\t\n\v\f\r ";
        while (it != text.end() && whitespace.find(*it) != std::string::npos)
            it++;
        assert(it != text.end());
    }
}