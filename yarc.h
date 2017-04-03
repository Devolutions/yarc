#ifndef YARC_H
#define YARC_H

#define YARC_EXTERN(_bundle, _identifier) \
extern const unsigned int yarc_ ## _bundle ## _ ## _identifier ## _size; \
extern const unsigned char* yarc_ ## _bundle ## _ ## _identifier ## _data;

#define YARC_DATA(_bundle, _identifier) \
	yarc_ ## _bundle ## _ ## _identifier ## _data

#define YARC_SIZE(_bundle, _identifier) \
	yarc_ ## _bundle ## _ ## _identifier ## _size

#endif /* YARC_H */

