cc_library (
  name = "main",
  srcs = ["main.cpp"],
)

cc_test (
  name = "methods-test",
  srcs = ["methods.cpp", "catch.hpp"],
  deps = [
    ":main",
    "//src:methods",
  ],
 )

