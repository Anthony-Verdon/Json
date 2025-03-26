set_targetdir("./")
set_languages("cxx17")

set_toolchains("clang")
set_warnings("allextra", "error")
set_optimize("fastest")
set_symbols("debug")

includes("submodules/Toolbox")

target("Json")
    set_kind("static")
    add_files("srcs/**.cpp")
    add_includedirs("srcs", {public = true})
    add_deps("Toolbox")
