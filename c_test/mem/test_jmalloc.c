#include <stdlib.h>
#include <stdio.h>
#include <jemalloc/jemalloc.h>

void do_something(size_t i) {
        // Leak some memory.
        malloc(i * 100);
}

int main(int argc, char **argv) {
        size_t i, sz;
        for (size_t i = 0; i < 1000; i++) {
                do_something(i);
                uint64_t epoch = 1;
                sz = sizeof(epoch);
                mallctl("epoch", &epoch, &sz, &epoch, sz);

                // Get basic allocation statistics.  Take care to check for
                // errors, since --enable-stats must have been specified at
                // build time for these statistics to be available.
                size_t sz, allocated, active, metadata, resident, mapped;
                sz = sizeof(size_t);
                if (mallctl("stats.allocated", &allocated, &sz, NULL, 0) == 0
                    && mallctl("stats.active", &active, &sz, NULL, 0) == 0
                    && mallctl("stats.metadata", &metadata, &sz, NULL, 0) == 0
                    && mallctl("stats.resident", &resident, &sz, NULL, 0) == 0
                    && mallctl("stats.mapped", &mapped, &sz, NULL, 0) == 0) {
                        fprintf(stderr,
                            "Current allocated/active/metadata/resident/mapped: %zu/%zu/%zu/%zu/%zu\n",
                            allocated, active, metadata, resident, mapped);
                }
        }
        // Dump allocator statistics to stderr.
        // malloc_stats_print(NULL, NULL, NULL);
        return 0;
}