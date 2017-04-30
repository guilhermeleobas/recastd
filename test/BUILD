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
    "//src:methods",
  ],
 )

cc_library (
  name = "linesweep-test",
  srcs = ["linesweep.cpp"], 
  hdrs = ["catch.hpp"],
  deps = [
    ":main",
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
    "//src:methods",
  ],
)
