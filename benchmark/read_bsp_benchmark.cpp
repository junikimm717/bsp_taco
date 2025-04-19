#include <bsp_taco.hpp>
#include "benchmark_utils.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: ./read_tns_benchmark [file_name.tns] {num_trials} [enable cold_cache?]\n");
    return 1;
  }
  bool cold_cache = argc >= 4;
  char* file_name = argv[1];
  int num_trials = std::atoi(argv[2]);
  assert(num_trials >= 0);

  double durations[num_trials];

  cout << "Opening " << file_name << "...\n";

  for (size_t i = 0; i < num_trials; i++) {
    if (cold_cache) {
      flush_cache();
    }
    double begin = gettime();
    bsp_taco::readBinSparse(argv[1]);
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
