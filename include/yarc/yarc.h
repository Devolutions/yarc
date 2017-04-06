#ifndef YARC_API_H
#define YARC_API_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define YARC_EXTERN(_bundle, _identifier) \
extern const unsigned int yarc_ ## _bundle ## _ ## _identifier ## _size; \
extern const unsigned char* yarc_ ## _bundle ## _ ## _identifier ## _data;

#define YARC_EXTERN_BUNDLE(_identifier) \
extern yarc_bundle_t yarc_ ## _identifier ## _bundle;

#define YARC_DATA(_bundle, _identifier) \
	yarc_ ## _bundle ## _ ## _identifier ## _data

#define YARC_SIZE(_bundle, _identifier) \
	yarc_ ## _bundle ## _ ## _identifier ## _size

#define YARC_DATA_SIZE(_bundle, _identifier) \
	yarc_ ## _bundle ## _ ## _identifier ## _data, \
	yarc_ ## _bundle ## _ ## _identifier ## _size

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  const char* name;
  const unsigned int* size;
  const unsigned char** data;
  const unsigned int offset;
} yarc_resource_t;

typedef struct {
  const char* name;
  const unsigned int size;
  const unsigned char* data;
  const unsigned int zsize;
  const unsigned char* zdata;
  const yarc_resource_t* resources;
} yarc_bundle_t;

const unsigned char* yarc_bundle_find(yarc_bundle_t* bundle, const char* name, int* size);

bool yarc_bundle_open(yarc_bundle_t* bundle);
bool yarc_bundle_close(yarc_bundle_t* bundle);

#ifdef __cplusplus
}
#endif

#endif /* YARC_API_H */

