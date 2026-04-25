#include "Json/Node/Node.hpp"
#include <stdexcept>
#include <iostream>

namespace Json
{
Node &Node::operator[](const char *key)
{
    if (const void *ptr = std::get_if<void *>(this))
        *this = NodeMap();
    return std::get<NodeMap>(*this)[key];
}

Node &Node::operator[](size_t index)
{
    if (const void *ptr = std::get_if<void *>(this))
        *this = NodeArray();
    auto &array = std::get<NodeArray>(*this);
    if (array.size() <= index)
        array.resize(index + 1);
    return array[index];
}

Node::operator float() const
{
    if (const int *ptr = std::get_if<int>(this))
        return static_cast<float>(*ptr);
    if (const double *ptr = std::get_if<double>(this))
        return static_cast<float>(*ptr);
    return (0);
}

bool Node::operator==(const Node &node) const
{
    if (const int *ptr = std::get_if<int>(&node))
        return *this == *ptr;
    else if (const double *ptr = std::get_if<double>(this))
        return *this == *ptr;
    else if (const std::string *ptr = std::get_if<std::string>(this))
        return *this == *ptr;
    else if (const bool *ptr = std::get_if<bool>(this))
        return *this == *ptr;
    else if (const void *ptr = std::get_if<void *>(&node))
        return *this == ptr;
    else if (const NodeArray *ptr = std::get_if<NodeArray>(this))
        return *this == *ptr;
    else if (const NodeMap *ptr = std::get_if<NodeMap>(this))
        return *this == *ptr;
    else
        return false;
}

bool Node::operator==(const int value) const
{
    if (const int *ptr = std::get_if<int>(this))
        return *ptr == value;

    return false;
}

bool Node::operator==(const double value) const
{
    if (const double *ptr = std::get_if<double>(this))
        return *ptr == value;

    return false;
}

bool Node::operator==(const std::string value) const
{
    if (const std::string *ptr = std::get_if<std::string>(this))
        return *ptr == value;

    return false;
}

bool Node::operator==(const bool value) const
{
    if (const bool *ptr = std::get_if<bool>(this))
        return *ptr == value;

    return false;
}

// absolutely not sure about this, but there can't be another type of void* than null, so we just need to check if we are a void* and if the ptr given is nullptr
bool Node::operator==(const void *ptr) const
{
    if (const void *ownPtr = std::get_if<void *>(this))
        return ptr == nullptr;

    return false;
}

bool Node::operator==(const NodeArray value) const
{
    if (const NodeArray *ptr = std::get_if<NodeArray>(this))
        return *ptr == value;

    return false;
}

bool Node::operator==(const NodeMap value) const
{
    if (const NodeMap *ptr = std::get_if<NodeMap>(this))
        return *ptr == value;

    return false;
}

const Node &Node::Iterator::operator*()
{
    if (const ArrayIt *ptr = std::get_if<ArrayIt>(&it))
        return (*(*ptr));
    if (const MapIt *ptr = std::get_if<MapIt>(&it))
        return (*(*ptr)).second;
    throw(std::runtime_error("Node::Iterator::operator*, invalid value"));
}

Node *Node::Iterator::operator->()
{
    if (const ArrayIt *ptr = std::get_if<ArrayIt>(&it))
        return &(**ptr);
    if (const MapIt *ptr = std::get_if<MapIt>(&it))
        return &((**ptr).second);
    return (NULL);
}

Node::Iterator &Node::Iterator::operator++()
{
    if (ArrayIt *ptr = std::get_if<ArrayIt>(&it))
        (*ptr)++;
    if (MapIt *ptr = std::get_if<MapIt>(&it))
        (*ptr)++;
    return *this;
}

Node::Iterator Node::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

std::string Node::Iterator::key() const
{
    if (const MapIt *ptr = std::get_if<MapIt>(&it))
        return ((*ptr)->first);
    else
        throw(std::runtime_error("Node::Iterator::key(), invalid value"));
}

Node Node::Iterator::value() const
{
    if (const MapIt *ptr = std::get_if<MapIt>(&it))
        return ((*ptr)->second);
    else
        throw(std::runtime_error("Node::value(), invalid value"));
}

Node::Iterator Node::begin()
{
    if (NodeArray *ptr = std::get_if<NodeArray>(this))
        return (Iterator(ptr->begin()));
    else if (NodeMap *ptr = std::get_if<NodeMap>(this))
        return (Iterator(ptr->begin()));
    else
        throw(std::runtime_error("Node::begin(), invalid value"));
}

Node::Iterator Node::end()
{
    if (NodeArray *ptr = std::get_if<NodeArray>(this))
        return (Iterator(ptr->end()));
    else if (NodeMap *ptr = std::get_if<NodeMap>(this))
        return (Iterator(ptr->end()));
    else
        throw(std::runtime_error("Node::end(), invalid value"));
}

bool Node::KeyExist(const std::string &key) const
{
    if (const NodeMap *ptr = std::get_if<NodeMap>(this))
        return (ptr->find(key) != ptr->end());

    return (false);
}

std::ostream &operator<<(std::ostream &os, const Node &node)
{
    static size_t level = 0;
    if (const int *ptr = std::get_if<int>(&node))
        os << *ptr;
    else if (const double *ptr = std::get_if<double>(&node))
        os << *ptr;
    else if (const std::string *ptr = std::get_if<std::string>(&node))
        os << '"' << *ptr << '"';
    else if (const bool *ptr = std::get_if<bool>(&node))
        os << (*ptr ? "true" : "false");
    else if (const void *ptr = std::get_if<void *>(&node))
        os << "null";
    else if (const NodeArray *ptr = std::get_if<NodeArray>(&node))
    {
        os << "[";
        level++;
        for (size_t i = 0; i < ptr->size();)
        {
            os << '\n';
            for (size_t i = 0; i < level; i++)
                os << '\t';
            os << (*ptr)[i];
            i++;
            if (i < ptr->size())
                os << ", ";
        }
        level--;
        os << '\n';
        for (size_t i = 0; i < level; i++)
            os << '\t';
        os << "]";
    }
    else if (const NodeMap *ptr = std::get_if<NodeMap>(&node))
    {
        os << '{';
        level++;
        for (auto it = ptr->begin(); it != ptr->end();)
        {
            os << '\n';
            for (size_t i = 0; i < level; i++)
                os << '\t';
            os << '"' << it->first << '"' << ": " << it->second;
            it++;
            if (it != ptr->end())
                os << ',';
        }
        level--;
        os << '\n';
        for (size_t i = 0; i < level; i++)
            os << '\t';
        os << '}';
    }
    return (os);
}
} // namespace Json