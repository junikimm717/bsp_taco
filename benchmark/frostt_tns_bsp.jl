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
dims = ndims(tensor);
print(stderr, "Starting the writing process...\n")

if ARGS[3] == "coo"
  tensor_fmt = Element{0.0, Float64, Int32}();
  tensor_fmt = SparseCOO{dims, NTuple{dims, Int32}}(tensor_fmt);
  @time fwrite(output, Tensor(tensor_fmt, tensor))
else
  tensor_fmt = Element{0.0, Float64, Int32}();
  for i in 1:dims-1
    global tensor_fmt = SparseList{Int32}(tensor_fmt);
  end
  tensor_fmt = Dense(tensor_fmt)
  @time fwrite(output, Tensor(tensor_fmt, tensor))
end

