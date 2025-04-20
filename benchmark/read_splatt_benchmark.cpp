#include "benchmark_utils.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <splatt.h>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr,
            "usage: ./read_splatt_benchmark [file_name.tns] {num_trials} "
            "[enable cold_cache?]\n");
    return 1;
  }
  bool cold_cache = argc >= 4;
  char* file_name = argv[1];
  int num_trials = std::atoi(argv[2]);
  assert(num_trials >= 0);

  double durations[num_trials];

  cerr << "Opening " << file_name << "...\n";

  size_t nnz = 0;
  for (size_t i = 0; i < num_trials; i++) {
    if (cold_cache) {
      flush_cache();
    }
    splatt_idx_t nmodes;
    splatt_csf* tt;
    double * cpd_opts = splatt_default_opts();

    double begin = gettime();
    int ret = splatt_csf_load(file_name, &nmodes, &tt, cpd_opts);
    double end = gettime();

    assert (ret == SPLATT_SUCCESS);
    if (i == 0) {
      nnz = tt->nnz;
    }
    durations[i] = end - begin;
    cerr << "Run " << i << ": Took " << durations[i]
         << " seconds to parse...\n";

    splatt_free_csf(tt, cpd_opts);
    splatt_free_opts(cpd_opts);
  }

  char* output = result_json(durations, num_trials, file_name,
                             std::string("splatt_tns").data(), nnz);

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
