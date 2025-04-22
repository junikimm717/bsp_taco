using Finch;
using HDF5;
using JSON;
using BenchmarkTools;

if length(ARGS) < 2
  print(stderr, "usage: julia read_bsp_benchmark.jl [file_name.bsp.h5] {num_trials}\n")
  exit(1)
end

filename = ARGS[1]
iterations = parse(Int, ARGS[2])

print(stderr, "Using finch_bsp to open $filename...\n")

tensor = fread(filename)
nnz = countstored(tensor)

times = [0.0 for _ in 1:iterations]

for i in 1:iterations
  res = @benchmark fread(filename) evals=1;
  times[i] = res.times[1]/1_000_000_000
end

c = Dict(
  "filename" => filename,
  "operation" => "finch_bsp",
  "date" => Int(round(time() * 1000)),
  "times" => times,
  "nnz" => nnz
)

JSON.print(stdout, c, 2)
