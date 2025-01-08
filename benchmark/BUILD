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

cc_binary(
    name = "benchmark-spsc-queue",
    srcs = ["benchmark_spsc_queue.cpp"],
    deps = [
        ":benchmark",
        "//queues:lock-free-queue",
        "//queues:thread-safe-queue",
    ],
)
