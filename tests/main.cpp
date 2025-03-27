#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "Json/Json.hpp"

TEST_CASE("Node")
{
    Json::Node node;
    node["number"]["int"] = 1;
    node["number"]["float"] = 2.0f;
    node["number"]["double"] = 3.0;
    node["string"] = "Hello World !";
    node["boolean"] = false;
    node["NULL"] = nullptr;
    node["array"] = Json::NodeArray();
    for (int i = 0; i < 3; i++)
        node["array"][i] = i + 5;
    node["map"]["key1"] = "value1";
    node["map"]["key2"] = 2;
    std::cout << node << std::endl;
}

// @todo: complete test cases