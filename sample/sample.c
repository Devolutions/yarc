
#include <stdio.h>
#include <stdlib.h>

#include "yarc.h"

YARC_EXTERN(sample, colors_json);
YARC_EXTERN(sample, squirrel_jpg);

#ifdef YARC_LZ4
extern int LZ4_decompress_safe(const char* source, char* dest, int compressedSize, int maxDecompressedSize);
#else
int LZ4_decompress_safe(const char* source, char* dest, int compressedSize, int maxDecompressedSize) { return 0; }
#endif

extern yarc_bundle_t yarc_sample_bundle;

int yarc_bundle_open(yarc_bundle_t* bundle)
{
	int zstatus;
	yarc_resource_t* resource;

	if (bundle->zsize)
	{
		bundle->data = (unsigned char*) malloc(bundle->size);

		if (!bundle->data)
			return -1;

		zstatus = LZ4_decompress_safe(bundle->zdata, (char*) bundle->data, bundle->zsize, bundle->size);

		if (zstatus != bundle->size)
			return -1;

		resource = (yarc_resource_t*) bundle->resources;

		while (resource->data)
		{
			*(resource->data) = &bundle->data[resource->offset];
			resource++;
		}
	}

	return 1;
}

int yarc_bundle_close(yarc_bundle_t* bundle)
{
	yarc_resource_t* resource;

	if (bundle->zsize)
	{
		resource = (yarc_resource_t*) bundle->resources;

		while (resource->data)
		{
			*(resource->data) = NULL;
			resource++;
		}

		free((void*) bundle->data);
		bundle->data;
	}

	return 1;
}

int main(int argc, char** argv)
{
	FILE* fp;

	yarc_bundle_open(&yarc_sample_bundle);

	printf("%s", YARC_DATA(sample, colors_json));

	fp = fopen("output.jpg", "w+b");

	if (fp)
	{
		fwrite(YARC_DATA(sample, squirrel_jpg), 1, YARC_SIZE(sample, squirrel_jpg), fp);
		fclose(fp);
	}

	yarc_bundle_close(&yarc_sample_bundle);

	return 0;
}

