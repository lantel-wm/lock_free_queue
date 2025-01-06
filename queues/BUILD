cc_library(
    name = "base-queue",
    hdrs = ["base_queue.hpp"],
    visibility = [
        "//benchmark:__pkg__",
        "//tests:__pkg__",
    ],
)

cc_library(
    name = "ring-queue",
    hdrs = ["ring_queue.hpp"],
    visibility = [
        "//benchmark:__pkg__",
        "//tests:__pkg__",
    ],
)

cc_library(
    name = "thread-safe-queue",
    hdrs = ["thread_safe_queue.hpp"],
    visibility = [
        "//benchmark:__pkg__",
        "//tests:__pkg__",
    ],
    deps = [":base-queue"],
)
