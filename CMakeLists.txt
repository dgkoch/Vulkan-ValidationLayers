# ~~~
# Copyright (c) 2014-2023 Valve Corporation
# Copyright (c) 2014-2023 LunarG, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ~~~
cmake_minimum_required(VERSION 3.17.2)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(VVL LANGUAGES CXX C)

option(BUILD_TESTS "Build the tests" OFF)

if (UPDATE_DEPS)
    find_package(PythonInterp 3 REQUIRED)

    if (CMAKE_GENERATOR_PLATFORM)
        set(_target_arch ${CMAKE_GENERATOR_PLATFORM})
    else()
        if (MSVC_IDE)
            message(WARNING "CMAKE_GENERATOR_PLATFORM not set. Using x64 as target architecture.")
        endif()
        set(_target_arch x64)
    endif()

    if (NOT CMAKE_BUILD_TYPE)
        message(WARNING "CMAKE_BUILD_TYPE not set. Using Debug for dependency build type")
        set(_build_type Debug)
    else()
        set(_build_type ${CMAKE_BUILD_TYPE})
    endif()

    message("********************************************************************************")
    message("* NOTE: Adding target vvl_update_deps to run as needed for updating            *")
    message("*       dependencies.                                                          *")
    message("********************************************************************************")

    set(optional_args)
    if (NOT BUILD_TESTS)
        set(optional_args "--optional=tests")
    endif()

    if (UPDATE_DEPS_SKIP_EXISTING_INSTALL)
        set(optional_args ${optional_args} "--skip-existing-install")
    endif()

    if (DEFINED CMAKE_TOOLCHAIN_FILE)
        set(optional_args ${optional_args} "--cmake_var")
        set(optional_args ${optional_args} "CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
    endif()

    # Add a target so that update_deps.py will run when necessary
    # NOTE: This is triggered off of the timestamps of known_good.json and helper.cmake
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_LIST_DIR}/external/helper.cmake
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/scripts/update_deps.py 
            --dir ${CMAKE_CURRENT_LIST_DIR}/external --arch ${_target_arch} --config ${_build_type} --generator "${CMAKE_GENERATOR}" ${optional_args}
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/scripts/known_good.json
    )

    add_custom_target(vvl_update_deps ALL DEPENDS ${CMAKE_CURRENT_LIST_DIR}/external/helper.cmake)

    # Check if update_deps.py needs to be run on first cmake run
    if (${CMAKE_CURRENT_LIST_DIR}/scripts/known_good.json IS_NEWER_THAN ${CMAKE_CURRENT_LIST_DIR}/external/helper.cmake)
        execute_process(
            COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/scripts/update_deps.py
                --dir ${CMAKE_CURRENT_LIST_DIR}/external --arch ${_target_arch} --config ${_build_type} --generator "${CMAKE_GENERATOR}" ${optional_args}
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            RESULT_VARIABLE _update_deps_result
        )
        if (NOT (${_update_deps_result} EQUAL 0))
            message(FATAL_ERROR "Could not run update_deps.py which is necessary to download dependencies.")
        endif()
    endif()
    include(${CMAKE_CURRENT_LIST_DIR}/external/helper.cmake)
else()
    message("********************************************************************************")
    message("* NOTE: Not adding target to run update_deps.py automatically.                 *")
    message("********************************************************************************")
    find_package(PythonInterp 3 QUIET)
endif()
# Dependencies can be installed in arbitrary locations. This is done by update_deps.py / users.
if (ROBIN_HOOD_HASHING_INSTALL_DIR)
    list(APPEND CMAKE_PREFIX_PATH ${ROBIN_HOOD_HASHING_INSTALL_DIR})
endif()
if (GLSLANG_INSTALL_DIR)
    list(APPEND CMAKE_PREFIX_PATH ${GLSLANG_INSTALL_DIR})
endif()
if (SPIRV_HEADERS_INSTALL_DIR)
    list(APPEND CMAKE_PREFIX_PATH ${SPIRV_HEADERS_INSTALL_DIR})
endif()
if (SPIRV_TOOLS_INSTALL_DIR)
    list(APPEND CMAKE_PREFIX_PATH ${SPIRV_TOOLS_INSTALL_DIR})
endif()
if (GOOGLETEST_INSTALL_DIR)
    list(APPEND CMAKE_PREFIX_PATH ${GOOGLETEST_INSTALL_DIR})
endif()
if (VULKAN_HEADERS_INSTALL_DIR)
    list(APPEND CMAKE_PREFIX_PATH ${VULKAN_HEADERS_INSTALL_DIR})
endif()
if (MIMALLOC_INSTALL_DIR)
    list(APPEND CMAKE_PREFIX_PATH ${MIMALLOC_INSTALL_DIR})
endif()

set(VVL_CPP_STANDARD 17 CACHE STRING "Set the C++ standard to build against.")
set(CMAKE_CXX_STANDARD ${VVL_CPP_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_C_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN "YES")

include(GNUInstallDirs)

if(WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    # Windows: if install locations not set by user, set install prefix to "<build_dir>\install".
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE)
endif()

# Enable GUI folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# "Helper" targets that don't have interesting source code should set their FOLDER property to this
set(LAYERS_HELPER_FOLDER "Helper Targets")

find_package(VulkanHeaders REQUIRED CONFIG QUIET)

target_compile_definitions(Vulkan::Headers INTERFACE VK_ENABLE_BETA_EXTENSIONS) # Enable beta Vulkan extensions
if(WIN32)
    target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_WIN32_KHR)
elseif(ANDROID)
    target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_ANDROID_KHR)
elseif(APPLE)
    target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_MACOS_MVK VK_USE_PLATFORM_METAL_EXT)
else()
    option(BUILD_WSI_XCB_SUPPORT "Build XCB WSI support" ON)
    option(BUILD_WSI_XLIB_SUPPORT "Build Xlib WSI support" ON)
    option(BUILD_WSI_WAYLAND_SUPPORT "Build Wayland WSI support" ON)

    find_package(PkgConfig REQUIRED QUIET) # Use PkgConfig to find Linux system libraries

    if(BUILD_WSI_XCB_SUPPORT)
        pkg_check_modules(XCB REQUIRED QUIET IMPORTED_TARGET xcb)
        target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_XCB_KHR)
    endif()

    if(BUILD_WSI_XLIB_SUPPORT)
        pkg_check_modules(X11 REQUIRED QUIET IMPORTED_TARGET x11)
        target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_XLIB_KHR VK_USE_PLATFORM_XLIB_XRANDR_EXT)
    endif()

    if(BUILD_WSI_WAYLAND_SUPPORT)
        target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_WAYLAND_KHR)
    endif()
endif()

if (ANNOTATED_SPEC_LINK)
    message("ANNOTATED_SPEC_LINK is ${ANNOTATED_SPEC_LINK}")
    add_compile_definitions(ANNOTATED_SPEC_LINK=${ANNOTATED_SPEC_LINK})
endif()

# NOTE: The idiom for open source projects is to not enable warnings as errors.
# This reduces problems for users who simply want to build the repository.
option(BUILD_WERROR "Treat compiler warnings as errors" OFF)
if (BUILD_WERROR)
    add_compile_options("$<IF:$<CXX_COMPILER_ID:MSVC>,/WX,-Werror>")
endif()

option(VVL_ENABLE_ASAN "Use address sanitization")
option(VVL_ENABLE_TSAN "Use thread sanitizer")
if (VVL_ENABLE_ASAN)
    add_compile_options(-fsanitize=address)
    add_compile_options(-fno-omit-frame-pointer) # Get nicer stack traces in error messages
    add_link_options(-fsanitize=address)
elseif (VVL_ENABLE_TSAN)
    # Thread sanitizer cannot be used in combination with address sanitizer
    add_compile_options(-fsanitize=thread)
    add_compile_options(-fno-omit-frame-pointer) # Get nicer stack traces in error messages
    add_link_options(-fsanitize=thread)
endif()

# Platform-specific compiler switches
if(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    add_compile_options(-Wconversion
                        -Wimplicit-fallthrough

                        # TODO These warnings also get turned on with -Wconversion in some versions of clang.
                        #      Leave off until further investigation.
                        -Wno-sign-conversion
                        -Wno-shorten-64-to-32
                        -Wno-implicit-int-conversion
                        -Wno-enum-enum-conversion
                        -Wstring-conversion)

endif()
if(${CMAKE_C_COMPILER_ID} MATCHES "(GNU|Clang)")
    add_compile_options(-Wall
                        -Wextra
                        -Wno-unused-parameter
                        -Wno-missing-field-initializers
                        -fno-strict-aliasing
                        -fno-builtin-memcmp)

    set(CMAKE_C_STANDARD 99)
elseif(MSVC)
    # Warn about nested declarations
    add_compile_options("/w34456")
    # Warn about potentially uninitialized variables
    add_compile_options("/w34701")
    add_compile_options("/w34703")
    # Warn about different indirection types.
    add_compile_options("/w34057")
    # Warn about signed/unsigned mismatch.
    add_compile_options("/w34245")

    # PDBs aren't generated on Release builds by default.
    add_compile_options("$<$<CONFIG:Release>:/Zi>")
    add_link_options("$<$<CONFIG:Release>:/DEBUG:FULL>")

    # Enable /LTCG (Link-time code generation)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    # Remove unreferenced code/data.
    add_link_options("$<$<CONFIG:Release>:/OPT:REF>")
    # Merge duplicate data/functions
    add_link_options("$<$<CONFIG:Release>:/OPT:ICF>")

    # Allow usage of unsafe CRT functions and minimize what Windows.h leaks
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX WIN32_LEAN_AND_MEAN)

    add_compile_options($<$<BOOL:${MSVC_IDE}>:/MP>) # Speed up Visual Studio builds
endif()

if(MINGW)
    add_compile_definitions(_WIN32_WINNT=0x0600) # Must be declared before including Windows.h
    add_compile_options("-Wa,-mbig-obj")
endif()

option(INSTALL_TESTS "Install tests" OFF)
option(BUILD_LAYERS "Build layers" ON)
option(BUILD_LAYER_SUPPORT_FILES "Generate layer files" OFF) # For generating files when not building layers

# uninstall target ---------------------------------------------------------------------------------------------------------------
if(NOT TARGET uninstall)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                   IMMEDIATE
                   @ONLY)
    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
    set_target_properties(uninstall PROPERTIES FOLDER ${LAYERS_HELPER_FOLDER})
endif()

# Add subprojects ----------------------------------------------------------------------------------------------------------------
if(BUILD_LAYERS OR BUILD_LAYER_SUPPORT_FILES)
    add_subdirectory(layers)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
