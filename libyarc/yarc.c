
#include <yarc/yarc.h>

#ifdef YARC_LZ4
extern int LZ4_decompress_safe(const char* source, char* dest, int compressedSize, int maxDecompressedSize);
#else
int LZ4_decompress_safe(const char* source, char* dest, int compressedSize, int maxDecompressedSize) { return 0; }
#endif

#ifndef _WIN32
#define _strdup strdup
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

typedef struct {
	uint32_t offset;
	uint32_t size;
	const char* name;
	uint8_t* data;
} yarc_entry_t;

struct yarc_block_s {
	uint32_t magic;
	uint32_t offset;
	uint32_t size;
	uint32_t zsize;
	uint32_t count;
	uint32_t flags;
	const char* name;
	yarc_entry_t* entries;
	uint8_t* data;
};

size_t yarc_string_read(const uint8_t* data, size_t size, const char** str)
{
	uint16_t len;

	*str = "";

	if (size < 3)
		return 0;

	len = *((uint16_t*) data);

	if (((size_t) len) > (size - 3))
		return 0;

	if (data[2 + len] != '\0')
		return 0;

	*str = (const char*) &data[2];

	return ((2 + ((size_t) len) + 1) + 3) & ~0x3;
}

const char* yarc_block_name(yarc_block_t* block)
{
	if (!block)
		return "";

	return block->name;
}

uint32_t yarc_block_count(yarc_block_t* block)
{
	if (!block)
		return 0;

	return block->count;
}

const uint8_t* yarc_block_entry(yarc_block_t* block, uint32_t index, uint32_t* size, const char** name)
{
	yarc_entry_t* entry;

	if (index >= block->count)
		return NULL;

	entry = &block->entries[index];
	*size = entry->size;

	if (name)
		*name = entry->name;

	return entry->data;
}

const uint8_t* yarc_block_find(yarc_block_t* block, const char* name, uint32_t* size)
{
	uint32_t index;
	yarc_entry_t* entry;

	for (index = 0; index < block->count; index++)
	{
		entry = &block->entries[index];

		if (!strcmp(name, entry->name))
		{
			*size = entry->size;
			return entry->data;
		}
	}

	return NULL;
}

yarc_block_t* yarc_block_open(const uint8_t* blockData, size_t blockSize)
{
	int zstatus;
	uint32_t index;
	yarc_block_t* block;
	yarc_entry_t* entry;
	const uint8_t* body;
	const uint8_t* ptr = blockData;

	block = (yarc_block_t*) malloc(sizeof(yarc_block_t));

	if (!block)
		return NULL;

	if (blockSize < 24)
		return NULL;

	block->magic = *((uint32_t*) &ptr[0]);
	block->offset = *((uint32_t*) &ptr[4]);
	block->size = *((uint32_t*) &ptr[8]);
	block->zsize = *((uint32_t*) &ptr[12]);
	block->count = *((uint32_t*) &ptr[16]);
	block->flags = *((uint32_t*) &ptr[20]);
	ptr += 24;

	if (block->magic != YARC_MAGIC)
		return NULL;

	ptr += yarc_string_read(ptr, blockSize - (ptr - blockData), &block->name);
	block->name = _strdup(block->name);

	block->data = (unsigned char*) malloc(block->size);

	if (!block->data)
		return NULL;

	body = &blockData[block->offset];

	if (block->zsize)
	{
		if ((block->offset + block->zsize) > blockSize)
			return false;

		zstatus = LZ4_decompress_safe((const char*) body, (char*) block->data, block->zsize, block->size);

		if (zstatus != block->size)
			return NULL;
	}
	else
	{
		if ((block->offset + block->size) > blockSize)
			return NULL;

		memcpy(block->data, body, block->size);
	}

	block->entries = (yarc_entry_t*) calloc(block->count, sizeof(yarc_entry_t));

	if (!block->entries)
		return NULL;

	for (index = 0; index < block->count; index++)
	{
		entry = &block->entries[index];
		
		entry->offset = *((uint32_t*) &ptr[0]);
		entry->size = *((uint32_t*) &ptr[4]);
		ptr += 8;

		ptr += yarc_string_read(ptr, blockSize - (ptr - blockData), &entry->name);
		entry->name = _strdup(entry->name);

		entry->data = &block->data[entry->offset];
	}

	return block;
}

void yarc_block_close(yarc_block_t* block)
{
	uint32_t index;
	yarc_entry_t* entry;

	if (!block)
		return;

	free((void*) block->name);
	free(block->data);

	for (index = 0; index < block->count; index++)
	{
		entry = &block->entries[index];
		free((void*) entry->name);
	}

	free(block->entries);
	free(block);
}
