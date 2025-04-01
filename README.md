# bsp_taco library

This is a library that allows binsparse files to be read as, and read into,
[taco](https://github.com/tensor-compiler/taco) objects. This is a standard
cmake project, and the main header file to include for all intents and purposes
is `<bsp_taco.hpp>`

# Building

`cmake -B build && cmake --build build` should just work.

To test, you will need a version of julia on your system with the 
[finch compiler](https://github.com/willow-ahrens/Finch.jl)
installed.
