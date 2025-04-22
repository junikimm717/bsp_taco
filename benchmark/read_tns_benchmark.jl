using Finch;
using HDF5;
using JSON;
using BenchmarkTools;

if length(ARGS) < 3
  print(stderr, "usage: julia read_tns_benchmark.jl [file_name.tns] {iterations} [coo|csf]\n")
  exit(1)
end

filename = ARGS[1]
iterations = parse(Int, ARGS[2])

@assert ARGS[3] == "coo" || ARGS[3] == "csf"

print(stderr, "Using finch's tns reader to open $filename...\n")

@time tensor = fread(filename)
nnz = countstored(tensor)

times = [0.0 for _ in 1:iterations]

for i in 1:iterations
  res = @benchmark fread(filename) evals=1;
  times[i] = res.times[1]/1_000_000_000
end

c = Dict(
  "filename" => filename,
  "operation" => "finch_tns_" + ARGS[3],
  "date" => Int(round(time() * 1000)),
  "times" => times,
  "nnz" => nnz
)

JSON.print(stdout, c, 2)
