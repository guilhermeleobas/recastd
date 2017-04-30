package(default_visibility = ["//visibility:public"])

cc_library (
  name = "encounter",
  srcs = ["encounter.cpp"],
  hdrs = ["encounter.hpp"],
)

cc_library (
  name = "methods",
  srcs = ["methods.cpp"],
  hdrs = ["methods.hpp"],
  deps = [
    ":encounter",
  ],
  visibility = ["//test:__pkg__",],
)

cc_library (
  name = "graph",
  srcs = ["graph.cpp"],
  hdrs = ["graph.hpp"],
  deps = [
    ":encounter",
  ],
)

cc_binary(
  name = "recast",
  srcs = ["main.cpp"],
  deps = [
    ":graph",
    ":encounter",
    ":methods",
  ],
  copts = ["-std=c++14", "-Wall", "-Wextra", "-O2"]
)