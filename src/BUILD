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
  name = "simple_graph",
  srcs = [],
  hdrs = ["simple_graph.hpp"],
)

cc_binary(
  name = "recast",
  srcs = ["main.cpp"],
  deps = [
    ":graph",
    ":encounter",
    ":simple_graph"
  ],
  copts = ["-std=c++11", "-Wall", "-Wextra", "-O2"]
)
