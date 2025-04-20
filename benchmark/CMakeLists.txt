# SPDX-FileCopyrightText: 2024 Binsparse Developers
#
# SPDX-License-Identifier: BSD-3-Clause
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

FetchContent_Declare(
  splatt
  GIT_REPOSITORY https://github.com/junikimm717/splatt
  GIT_TAG master
)
FetchContent_MakeAvailable(splatt)

function(add_example example_name)
  add_executable(${example_name} ${example_name}.cpp)
  target_link_libraries(${example_name} bsp-to-taco splatt)
  target_include_directories(${example_name} PUBLIC ${splatt_SOURCE_DIR}/include)
endfunction()

add_example(read_bsp_benchmark)
add_example(read_tns_benchmark)
add_example(read_splatt_benchmark)
add_example(frostt_tns_bsp)
