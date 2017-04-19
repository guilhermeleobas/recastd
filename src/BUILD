cc_library (
  name = "graph",
  srcs = ["graph.cpp"],
  hdrs = ["graph.hpp"],
)

cc_library (
  name = "encounter",
  srcs = ["encounter.cpp"],
  hdrs = ["encounter.hpp"],
)


cc_library (
  name = "graph_methods",
  srcs = ["graph_methods.cpp"],
  hdrs = ["graph_methods.hpp"],
)

cc_binary(
  name = "recast",
  srcs = ["main.cpp"],
  deps = [
    ":graph",
    ":encounter",
    ":graph_methods",
  ],
  copts = ["-std=c++11", "-Wall", "-Wextra", "-O2"]
)
