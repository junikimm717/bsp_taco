# SPDX-FileCopyrightText: 2024 Binsparse Developers
#
# SPDX-License-Identifier: BSD-3-Clause

function(add_example example_name)
  add_executable(${example_name} ${example_name}.cpp)
  target_link_libraries(${example_name} bsp-to-taco)
endfunction()

add_example(taco_experiments)
add_example(parse_taco)
add_example(tensor_test)
add_example(read_taco_benchmark)
