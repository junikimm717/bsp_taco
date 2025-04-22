#include <bsp_taco.hpp>
#include <fstream>
#include <sstream>
#include <string>

#include "benchmark_utils.h"
#include "formats.hpp"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 4) {
    fprintf(stderr, "usage: ./read_tns_benchmark [file_name.tns] {num_trials} "
                    "[coo|csf]\n");
    return 1;
  }
  bool cold_cache = false;
  char* file_name = argv[1];
  int num_trials = std::atoi(argv[2]);

  int dimensions = 0;
  {
    ifstream in(file_name);
    if (!in) {
      cerr << "Can't open file!";
      return 1;
    }
    string firstLine;
    if (getline(in, firstLine)) {
      istringstream iss(firstLine);
      std::string word;
      while (iss >> word) {
        ++dimensions;
      }
      --dimensions;
    }
  }

  assert(num_trials > 0 && dimensions > 0);

  double durations[num_trials];

  benchmark_format_t benchmarkFmt = getModeFormat(argv[3]);
  auto modeFormatPacks = getModeFormatPack(benchmarkFmt, dimensions);
  taco::Format format(modeFormatPacks);

  cerr << "Using taco_tns to open " << file_name << "...\n";
  cerr << "Using the " << getFormatName(benchmarkFmt) << " format with "
       << dimensions << " dimensions..." << endl;

  size_t nnz = 0;
  for (size_t i = 0; i < num_trials; i++) {
    if (cold_cache) {
      flush_cache();
    }
    // make sure you ONLY do tensor reading here.
    double begin = gettime();
    auto tensor = taco::read(std::string(file_name), format);
    double end = gettime();
    if (i == 0) {
      nnz = tensor.getStorage().getValues().getSize();
    }
    durations[i] = end - begin;
    cerr << "Run " << i << ": Took " << durations[i]
         << " seconds to parse...\n";
  }

  char* output = result_json(
      durations, num_trials, file_name,
      std::string("taco_tns_" + getFormatName(benchmarkFmt)).data(), nnz);

  qsort(durations, num_trials, sizeof(double), compar);
  double variance = compute_variance(durations, num_trials);
  double median_time = durations[num_trials / 2];
  fprintf(stderr, "Read file in %lf seconds\n", median_time);
  fprintf(stderr,
          "Variance is %lf seconds, standard deviation is %lf seconds\n",
          variance, sqrt(variance));
  cout << output << endl;
  free(output);
  return 0;
}
