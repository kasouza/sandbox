#ifndef KASOUZA_SANDBOX_INCLUDE_UTILS_H
#define KASOUZA_SANDBOX_INCLUDE_UTILS_H

#define BOX_LOG(...) fprintf(stderr, __VA_ARGS__)

#define BOX_MIN(a, b) ((a) < (b) ? (a) : (b))
#define BOX_MAX(a, b) ((a) > (b) ? (a) : (b))
#define BOX_CLAMP(n, min, max) (BOX_MIN(BOX_MAX(n, min), max))

char *box_read_file(const char *path);

#endif
