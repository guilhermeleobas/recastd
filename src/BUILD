cc_library (
  name = "encounter",
  srcs = ["encounter.cpp"],
  hdrs = ["encounter.hpp"],
)

cc_library (
  name = "graph",
  srcs = ["graph.cpp"],
  hdrs = ["graph.hpp"],
  deps = [
    ":encounter",
  ]
)

cc_binary(
  name = "recast",
  srcs = ["main.cpp"],
  deps = [
    ":graph",
    ":encounter",
  ],
  copts = ["-std=c++14", "-Wall", "-Wextra", "-O2"]
)