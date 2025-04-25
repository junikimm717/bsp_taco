using Finch;
using HDF5;
using JSON;
using BenchmarkTools;
using TensorMarket;

if length(ARGS) < 3
  print(stderr, "usage: julia read_finch_benchmark.jl [file_name.tns] [file_name.bsp.h5] [coo|csf]\n")
  exit(1)
end

filename = ARGS[1]
output = ARGS[2]
@assert ARGS[3] == "coo" || ARGS[3] == "csf"

print(stderr, "Using finch_bsp to open $filename...\n")
@time tensor = fread(filename)
print(stderr, "Starting the writing process...\n")

if ARGS[3] == "coo"
  @time fwrite(output, Tensor(COOFormat(ndims(tensor), 0.0), tensor))
else
  @time fwrite(output, Tensor(CSFFormat(ndims(tensor), 0.0), tensor))
end
