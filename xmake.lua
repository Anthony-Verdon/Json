set_languages("cxx17")

set_toolchains("clang")
set_warnings("allextra", "error")
set_optimize("fastest")
set_symbols("debug")

add_requires("doctest")

namespace("Toolbox", function ()
    includes("submodules/Toolbox")
end)

target("Json")
    set_targetdir("./")
    set_kind("static")
    add_files("srcs/**.cpp")
    add_includedirs("srcs", {public = true})
    add_deps("Toolbox::Toolbox")

target("Tester")
    set_targetdir("./")
    set_kind("binary")
    add_files("tests/**.cpp")
    add_includedirs("tests")
    add_deps("Json")
    add_packages("doctest")