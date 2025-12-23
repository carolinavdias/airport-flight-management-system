#ifndef PARSER_INPUT_H
#define PARSER_INPUT_H

#include <stddef.h>

typedef void (*LineCallback)(char **fields, size_t n_fields, void *user_data);

int parser_input_read_csv(const char *dataset_dir, const char *filename, LineCallback callback, void *user_data);

int parser_input_read_header(const char *dataset_dir, const char *filename, char *header_out, size_t size);

#endif