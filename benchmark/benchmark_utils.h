#pragma once

#include <bits/time.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cJSON/cJSON.h>

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

char* result_json(double* x, size_t n, char* filename, char* operation, long long nnz) {
  cJSON* c = cJSON_CreateObject();
  cJSON* all_times = cJSON_AddArrayToObject(c, "times");
  for (size_t i = 0; i < n; i++) {
    cJSON* time = cJSON_CreateNumber(x[i]);
    cJSON_AddItemToArray(all_times, time);
  }
  cJSON_AddStringToObject(c, "filename", filename);
  cJSON_AddStringToObject(c, "operation", operation);

  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  long long millis = (long long) (spec.tv_sec) * 1000 + spec.tv_nsec / 1000000;
  cJSON_AddNumberToObject(c, "date", millis);
  cJSON_AddNumberToObject(c, "nnz", (double) nnz);

  char* res = cJSON_Print(c);
  cJSON_Delete(c);
  return res;
}
