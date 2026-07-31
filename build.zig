const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // -----------------------------------------------------------------
    // External dependencies
    // -----------------------------------------------------------------
    const boost_dep = b.dependency("boost", .{});
    const gtest_dep = b.dependency("googletest", .{});

    // -----------------------------------------------------------------
    // Common include paths
    // -----------------------------------------------------------------
    const netlibcore_include = b.path("common/NetlibCore/include");
    const netlibcore_interfaces = b.path("common/NetlibCore/interfaces");
    const netlibapp_include = b.path("application/modules/NetlibApp/include");
    const app_include = b.path("application/include");

    const boost_include = boost_dep.path("");
    const gtest_include = gtest_dep.path("googletest/include");
    const gtest_root = gtest_dep.path("googletest");

    const cxx_flags = &.{
        "-std=c++20",
        "-Wall",
        "-Wextra",
    };

    // -----------------------------------------------------------------
    // SerialLogger executable
    // -----------------------------------------------------------------
    const exe_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libcpp = true,
    });

    const exe = b.addExecutable(.{
        .name = "SerialLogger",
        .root_module = exe_mod,
    });

    exe_mod.addCSourceFiles(.{
        .files = &.{
            "common/NetlibCore/src/Connection.cpp",
            "common/NetlibCore/src/ConnectionSupervisor.cpp",
            "application/modules/NetlibApp/src/AsioSerialConnectionFactory.cpp",
            "application/modules/NetlibApp/src/AsioSerialTextStream.cpp",
            "application/modules/NetlibApp/src/LinuxCOMPortScanner.cpp",
            "application/src/AppConnectionSupervisor.cpp",
            "application/src/application.cpp",
        },
        .flags = cxx_flags,
    });

    exe_mod.addIncludePath(netlibcore_include);
    exe_mod.addIncludePath(netlibcore_interfaces);
    exe_mod.addIncludePath(netlibapp_include);
    exe_mod.addIncludePath(app_include);
    exe_mod.addIncludePath(boost_include);

    exe_mod.linkSystemLibrary("pthread", .{});
    exe_mod.linkSystemLibrary("dl", .{});
    b.installArtifact(exe);

    // -----------------------------------------------------------------
    // Tests
    // -----------------------------------------------------------------
    const tests_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libcpp = true,
    });

    const tests = b.addExecutable(.{
        .name = "netcore_tests",
        .root_module = tests_mod,
    });

    tests_mod.addCSourceFiles(.{
        .files = &.{
            "common/NetlibCore/src/Connection.cpp",
            "common/NetlibCore/src/ConnectionSupervisor.cpp",
            "tests/Common/NetlibCore/test_tsqueue.cpp",
        },
        .flags = cxx_flags,
    });

    // GTest is third-party code: compile it without our warning flags and
    // with its own root directory in the include path so "src/..." includes
    // inside gtest-all.cc resolve correctly.
    const gtest_flags = &.{
        "-std=c++20",
        "-w",
    };
    tests_mod.addCSourceFile(.{
        .file = gtest_dep.path("googletest/src/gtest-all.cc"),
        .flags = gtest_flags,
    });
    tests_mod.addCSourceFile(.{
        .file = gtest_dep.path("googletest/src/gtest_main.cc"),
        .flags = gtest_flags,
    });

    tests_mod.addIncludePath(netlibcore_include);
    tests_mod.addIncludePath(netlibcore_interfaces);
    tests_mod.addIncludePath(netlibapp_include);
    tests_mod.addIncludePath(app_include);
    tests_mod.addIncludePath(boost_include);
    tests_mod.addIncludePath(gtest_include);
    tests_mod.addIncludePath(gtest_root);

    tests_mod.linkSystemLibrary("pthread", .{});
    tests_mod.linkSystemLibrary("dl", .{});

    const run_tests = b.addRunArtifact(tests);
    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_tests.step);
}
