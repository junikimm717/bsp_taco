#include "benchmark_utils.h"
#include <bsp_taco.hpp>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: ./read_tns_benchmark [file_name.tns] {num_trials} "
                    "[enable cold_cache?]\n");
    return 1;
  }
  bool cold_cache = argc >= 4;
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

  vector<taco::ModeFormatPack> modeFormats = {taco::Dense};
  for (int i = 1; i < dimensions; i++) {
    modeFormats.push_back(taco::Sparse);
  }
  taco::Format format(modeFormats);

  cerr << "Opening " << file_name << "...\n";

  for (size_t i = 0; i < num_trials; i++) {
    if (cold_cache) {
      flush_cache();
    }
    double begin = gettime();
    taco::read(std::string(file_name), format);
    double end = gettime();
    durations[i] = end - begin;
    cerr << "Run " << i << ": Took " << durations[i]
         << " seconds to parse...\n";
  }

  char* output = result_json(durations, num_trials, file_name,
                             std::string("taco_tns").data());

  qsort(durations, num_trials, sizeof(double), compar);
  double variance = compute_variance(durations, num_trials);
  double median_time = durations[num_trials / 2];
  fprintf(stderr, "Read file in %lf seconds\n", median_time);
  fprintf(stderr,
          "Variance is %lf seconds, standard devication is %lf seconds\n",
          variance, sqrt(variance));
  cout << output << endl;
  free(output);
  return 0;
}
