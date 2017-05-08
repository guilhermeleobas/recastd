cc_library (
  name = "main",
  srcs = ["main.cpp"],
  hdrs = ["catch.hpp"],
)

cc_library (
  name = "pqueue-test",
  srcs = ["pqueue.cpp"],
  hdrs = ["catch.hpp"],
  deps = [
    ":main",
    "//src:pqueue",
  ],
 )

cc_library (
  name = "merge-test",
  srcs = ["merge.cpp"],
  hdrs = ["catch.hpp"],
  deps = [
    ":main",
    "//src:methods",
    "//src:encounter",
  ],
 )

cc_library (
  name = "linesweep-test",
  srcs = ["linesweep.cpp"], 
  hdrs = ["catch.hpp"],
  deps = [
    ":main",
    "//src:linesweep",
    "//src:methods",
  ],
)

cc_test (
  name = "test",
  srcs = ["main.cpp"],
  deps = [
    ":main",
    "pqueue-test",
    "linesweep-test",
    "merge-test",
  ],
)
