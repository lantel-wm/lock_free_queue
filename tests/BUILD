cc_test(
    name = "test-base-queue",
    timeout = "short",
    srcs = ["test_base_queue.cpp"],
    copts = [
        "-Iexternal/gtest/googletest/include",
        "-Iexternal/gtest/googletest",
    ],
    deps = [
        "//queues:base-queue",
        "//utils:data",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "test-ring-queue",
    timeout = "short",
    srcs = ["test_ring_queue.cpp"],
    copts = [
        "-Iexternal/gtest/googletest/include",
        "-Iexternal/gtest/googletest",
    ],
    deps = [
        "//queues:ring-queue",
        "//utils:data",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "test-thread-safe-queue",
    timeout = "short",
    srcs = ["test_thread_safe_queue.cpp"],
    copts = [
        "-Iexternal/gtest/googletest/include",
        "-Iexternal/gtest/googletest",
    ],
    deps = [
        "//queues:ring-queue",
        "//queues:thread-safe-queue",
        "//utils:data",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "test-lock-free-queue",
    timeout = "short",
    srcs = ["test_lock_free_queue.cpp"],
    copts = [
        "-Iexternal/gtest/googletest/include",
        "-Iexternal/gtest/googletest",
    ],
    deps = [
        "//queues:lock-free-queue",
        "//utils:data",
        "@googletest//:gtest_main",
    ],
)
