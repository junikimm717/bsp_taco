#include <bsp_taco.hpp>
#include "benchmark_utils.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 4) {
    fprintf(stderr, "usage: ./read_bsp_benchmark [file_name.h5] {dimensions} {num_trials} [enable cold_cache?]\n");
    return 1;
  }
  bool cold_cache = argc >= 5;
  char* file_name = argv[1];
  int dimensions = std::atoi(argv[2]);
  int num_trials = std::atoi(argv[3]);
  assert(num_trials >= 0 && dimensions >= 0);

  double durations[num_trials];

  vector<taco::ModeFormatPack> modeFormats = {taco::Dense};
  for (int i = 1; i < dimensions; i++) {
    modeFormats.push_back(taco::Sparse);
  }
  taco::Format format(modeFormats);

  cout << "Opening " << file_name << "...\n";

  for (size_t i = 0; i < num_trials; i++) {
    if (cold_cache) {
      flush_cache();
    }
    double begin = gettime();
    taco::read(std::string(file_name), format);
    double end = gettime();
    durations[i] = end - begin;
    cout << "Run " << i << ": Took " << durations[i] << " seconds to parse...\n";
  }

  qsort(durations, num_trials, sizeof(double), compar);
  double variance = compute_variance(durations, num_trials);
  double median_time = durations[num_trials / 2];
  printf("Read file in %lf seconds\n", median_time);
  printf("Variance is %lf seconds, standard devication is %lf seconds\n",
         variance, sqrt(variance));
  return 0;
}
