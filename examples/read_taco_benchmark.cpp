#include <bsp_taco.hpp>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>

using namespace std;

double gettime() {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  return ((double) time.tv_sec) + ((double) 1e-9) * time.tv_nsec;
}

int compar(const void* a, const void* b) {
  double x = *((const double*) a);
  double y = *((const double*) b);

  double diff = x - y;

  if (diff > 0) {
    return 1;
  } else if (diff < 0) {
    return -1;
  } else {
    return 0;
  }
}

double compute_variance(double* x, size_t n) {
  double sum = 0;

  for (size_t i = 0; i < n; i++) {
    sum += x[i];
  }

  double mean = sum / n;

  double sum_of_squares = 0;
  for (size_t i = 0; i < n; i++) {
    sum_of_squares += (x[i] - mean) * (x[i] - mean);
  }

  return sum_of_squares / (n - 1);
}

void flush_cache() {
#ifdef __APPLE__
  int rv = system("bash -c \"sync && sudo purge\"");
#elif __linux__
  int rv = system("bash -c \"sync\" && sudo sh -c \"/usr/bin/echo 3 > "
                  "/proc/sys/vm/drop_caches\"");
#else
  static_assert(false);
#endif
  usleep(100000);
}

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: ./read_to_taco [file_name.h5] {num_trials} [enable cold_cache?]\n");
    return 1;
  }
  bool cold_cache = argc >= 4;
  char* file_name = argv[1];
  int num_trials = std::atoi(argv[2]);
  assert(num_trials >= 0);

  double durations[num_trials];

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

  cout << "Opening " << file_name << "...\n";
  taco::TensorBase taco = bsp_taco::readBinSparse(argv[1]);
  return 0;
}
