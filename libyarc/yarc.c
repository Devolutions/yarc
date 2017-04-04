
#include <yarc/yarc.h>

#ifdef YARC_LZ4
extern int LZ4_decompress_safe(const char* source, char* dest, int compressedSize, int maxDecompressedSize);
#else
int LZ4_decompress_safe(const char* source, char* dest, int compressedSize, int maxDecompressedSize) { return 0; }
#endif

const unsigned char* yarc_bundle_find(yarc_bundle_t* bundle, const char* name, int* size)
{
	const unsigned char* data;
	yarc_resource_t* resource;

	resource = (yarc_resource_t*) bundle->resources;

	while (resource->data)
	{
		if (!strcmp(name, resource->name))
		{
			data = *(resource->data);

			if (size)
				*size = *(resource->size);

			return data;
		}

		resource++;
	}

	return 0;
}

bool yarc_bundle_open(yarc_bundle_t* bundle)
{
	int zstatus;
	yarc_resource_t* resource;

	if (bundle->zsize)
	{
		bundle->data = (unsigned char*) malloc(bundle->size);

		if (!bundle->data)
			return false;

		zstatus = LZ4_decompress_safe((const char*) bundle->zdata, (char*) bundle->data, bundle->zsize, bundle->size);

		if (zstatus != bundle->size)
			return false;

		resource = (yarc_resource_t*) bundle->resources;

		while (resource->data)
		{
			*(resource->data) = &bundle->data[resource->offset];
			resource++;
		}
	}

	return true;
}

bool yarc_bundle_close(yarc_bundle_t* bundle)
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
		bundle->data = NULL;
	}

	return true;
}

