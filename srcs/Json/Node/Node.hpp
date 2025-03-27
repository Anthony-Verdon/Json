#pragma once

#include <map>
#include <string>
#include <vector>
#include <variant>

namespace Json
{
    struct Node;
    typedef std::map<std::string, Node> NodeMap;
    typedef std::vector<Node> NodeArray;

    struct Node : std::variant<int, double, std::string, bool, void*, NodeArray, NodeMap> 
    {
        using variant::variant;

        Node() { *this = nullptr; }
        Node &operator[](const char *key);
        Node &operator[](int index) { return ((*this)[(size_t)index]); }
        Node &operator[](size_t index);
        operator int() const { return std::get<int>(*this); }
        operator size_t() const { return static_cast<size_t>(std::get<int>(*this)); }
        operator double() const { return std::get<double>(*this); }
        operator float() const;
        operator std::string() const { return std::get<std::string>(*this); }
        operator bool() const { return std::get<bool>(*this); }
        operator void*() const { return NULL; }
        operator NodeArray() const { return std::get<NodeArray>(*this); }
        operator NodeMap() const { return std::get<NodeMap>(*this); }

        struct Iterator
        {
            using ArrayIt = NodeArray::iterator;
            using MapIt = NodeMap::iterator;
            using VariantIt = variant<ArrayIt, MapIt>;

            Iterator(const ArrayIt &it) {this->it = it; }
            Iterator(const MapIt &it) {this->it = it; }

            const Node &operator*();
            Node *operator->();
            Iterator& operator++(); // Prefix increment
            Iterator operator++(int); // Postfix increment
            
            friend bool operator== (const Iterator& a, const Iterator& b) { return a.it == b.it; };
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.it != b.it; };
            
            std::string key() const;
            Node value() const;

            private:
                VariantIt it;
        };

        Iterator begin();
        Iterator end();

        bool KeyExist(const std::string &key) const;

    };
    
    std::ostream &operator<<(std::ostream &os, const Node &node);
}