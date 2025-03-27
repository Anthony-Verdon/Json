#include "Json/Node/Node.hpp"
#include <stdexcept>
#include <iostream>

namespace Json
{
    Node &Node::operator[](size_t index)
    {
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

    Node::Iterator& Node::Iterator::operator++()
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
        else if (const void *ptr = std::get_if<void*>(&node))
            os << "NULL";
        else if (const NodeArray *ptr = std::get_if<NodeArray>(&node))
        {
            os << "[";
            level++;
            for (size_t i = 0; i < ptr->size(); )
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
            for (auto it = ptr->begin(); it != ptr->end(); )
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
}