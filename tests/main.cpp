#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "Json/Json.hpp"

TEST_CASE("Node")
{
    Json::Node node = Json::NodeMap();
    node["number"] = Json::NodeMap();
    node["number"]["int"] = 1;
    node["number"]["float"] = 2.0f;
    node["number"]["double"] = 3.0;
    node["string"] = "Hello World !";
    node["boolean"] = false;
    node["NULL"] = nullptr;
    node["array"] = Json::NodeArray({1, "here", false});
    node["map"] = Json::NodeMap({{"key1", "value1"},{"key2", "value2"}});
    std::cout << node << std::endl;
}

// @todo: complete test cases