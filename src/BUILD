package(default_visibility = ["//visibility:public"])

cc_library (
  name = "encounter",
  srcs = ["encounter.cpp"],
  hdrs = ["encounter.hpp", "debug.hpp"],
)

cc_library (
  name = "small_queue",
  hdrs = ["small_queue.hpp", "debug.hpp"],
  deps = [
    ":encounter",
  ]
)

cc_library(
  name = "parser",
  srcs = ["parser.cpp"],
  deps = [
    ":encounter",
  ]
)

cc_library (
  name = "methods",
  srcs = ["methods.cpp"],
  hdrs = ["methods.hpp", "debug.hpp"],
  deps = [
    ":encounter",
  ],
)

cc_library(
  name = "pqueue",
  srcs = ["pqueue.cpp"],
  hdrs = ["pqueue.hpp", "debug.hpp"],
  deps = [
    ":encounter",
  ]
)

cc_library(
  name = "linesweep",
  srcs = ["linesweep.cpp"],
  hdrs = ["linesweep.hpp", "debug.hpp"],
  deps = [
    ":encounter",
    ":pqueue",
  ]
)

cc_library (
  name = "graph",
  srcs = ["graph.cpp"],
  hdrs = ["roaring.h", "roaring.hh", "roaring.c", "graph.hpp", "debug.hpp", "methods.hpp"],
  deps = [
    ":encounter",
    ":small_queue",
  ],
  copts = ["-march=native", "-O3", "-std=c++14"],
)

cc_binary(
  name = "filter",
  srcs = ["filter.cpp"],
  deps = [
    ":encounter",
    ":methods",
  ],
  copts = ["-march=native", "-std=c++14", "-Wall", "-Wextra", "-O2"]
)

cc_binary(
  name = "recastd",
  srcs = ["main.cpp", "argh.h"],
  deps = [
    "pqueue",
    "linesweep",
    ":graph",
    ":encounter",
    ":methods",
  ],
  copts = ["-march=native", "-std=c++14", "-Wall", "-Wextra", "-O2"]
)
