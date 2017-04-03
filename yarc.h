#ifndef YARC_H
#define YARC_H

#define YARC_EXTERN(_bundle, _identifier) \
extern const unsigned int yarc_ ## _bundle ## _ ## _identifier ## _size; \
extern const unsigned char* yarc_ ## _bundle ## _ ## _identifier ## _data;

#define YARC_DATA(_bundle, _identifier) \
	yarc_ ## _bundle ## _ ## _identifier ## _data

#define YARC_SIZE(_bundle, _identifier) \
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

#ifdef __cplusplus
}
#endif

#endif /* YARC_H */

