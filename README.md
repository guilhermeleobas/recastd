# recastd

to compile and run (with debug)
bazel build -copt="-DDEBUG" src:recast && ./bazel-bind/src/recast

to compile and run (without debug)
bazel build src:recast && ./bazel-bind/src/recast

optionally you can pass the -t TIMESTEP flag to set a timestep
the default value is for timestep is 86400 which represents a day in seconds

for testing (i don't use the bazel test method because it removes the color from catch output)
bazel build test:methods-test && ./bazel-bind/test/methods-test

