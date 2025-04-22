using Finch;
using HDF5;
using JSON;
using BenchmarkTools;

if length(ARGS) < 2
  print(stderr, "usage: julia read_finch_benchmark.jl [file_name.tns] [file_name.bsp.h5]\n")
  exit(1)
end

filename = ARGS[1]
output = ARGS[2]

print(stderr, "Using finch_bsp to open $filename...\n")
@time tensor = fread(filename)
print(stderr, "Starting the writing process...\n")
fwrite(Tensor(CSFFormat(0.0), tensor), output)
