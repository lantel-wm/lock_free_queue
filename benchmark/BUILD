cc_library(
    name = "benchmark",
    hdrs = ["benchmark.hpp"],
)

cc_binary(
    name = "benchmark-container",
    srcs = ["benchmark_container.cpp"],
    deps = [
        ":benchmark",
        "//queues:base-queue",
        "//queues:ring-queue",
    ],
)
