
/**
 * YARC: Yet Another Resource Compiler
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _WIN32
#define _strdup strdup
#endif

typedef struct
{
	FILE* fp;
	size_t size;
	uint8_t* data;
	char* filename;
	char* basename;
	char* identifier;
} yarc_file_t;

typedef struct {
	const char* name;
	unsigned int size;
	unsigned char* data;
} yarc_resource_t;

static int yarc_width = 16;
static int yarc_padding = 2;
static bool yarc_upper = false;
static char* yarc_prefix = "yarc";
static char* yarc_bundle = "default";
static char* yarc_output = "resources.c";
static char* yarc_version = "1.0.0";
static bool yarc_static = false;
static bool yarc_verbose = false;

const char* yarc_get_base_name(const char* filename)
{
	size_t length;
	char* separator;

	if (!filename)
		return NULL;

	separator = strrchr(filename, '\\');

	if (!separator)
		separator = strrchr(filename, '/');

	if (!separator)
		return filename;

	length = strlen(filename);

	if ((length - (separator - filename)) > 1)
		return separator + 1;

	return filename;
}

int yarc_file_construct_names(yarc_file_t* yf)
{
	char* p;
	size_t length;
	size_t size;
	const char* basename = yarc_get_base_name(yf->filename);

	length = strlen(basename);
	yf->basename = _strdup(basename);

	if (!yf->basename)
		return -1;

	size = length + strlen(yarc_bundle) + strlen(yarc_prefix) + 3;
	yf->identifier = malloc(size);

	if (!yf->identifier)
		return -1;

	snprintf(yf->identifier, size, "%s_%s_%s", yarc_prefix, yarc_bundle, yf->basename);

	p = yf->identifier;

	while (*p)
	{
		if (*p == '.')
			*p = '_';

		p++;
	}

	return 1;
}

int yarc_file_write(yarc_file_t* out, yarc_file_t* in)
{
	int i;
	int width = yarc_width;
	const uint8_t* data = in->data;
	const char* identifier = in->identifier;
	size_t size = in->size + yarc_padding;

	fprintf(out->fp, "/* %s */\n", in->basename);
	fprintf(out->fp, "%sconst unsigned int %s_size = %d;\n",
		yarc_static ? "static " : "", identifier, (int) in->size);
	fprintf(out->fp, "%sconst unsigned char %s[%d+%d] = {\n",
		yarc_static ? "static " : "", identifier, (int) in->size, yarc_padding);

	for (i = 0; i < size; i++)
	{
		if ((i % width) == 0)
			fprintf(out->fp, "  ");

		fprintf(out->fp, yarc_upper ? "0x%02X" : "0x%02x", (int) data[i]);

		if (i != (size - 1))
			fprintf(out->fp, ", ");

		if (((i % width) == (width - 1)) || (i == (size - 1)))
			fprintf(out->fp, "\n");
	}

	fprintf(out->fp, "};\n");
	fprintf(out->fp, "%sconst unsigned char* %s_data = (unsigned char*) %s;\n\n",
		yarc_static ? "static " : "", identifier, identifier);

	return 1;
}

int yarc_file_open(yarc_file_t* yf, bool write)
{
	if (write)
	{
		yf->fp = fopen(yf->filename, "w+b");

		if (!yf->fp)
			return -1;

		return 1;
	}

	yf->fp = fopen(yf->filename, "rb");

	if (!yf->fp)
		return -1;

	fseek(yf->fp, 0, SEEK_END);
	yf->size = (int) ftell(yf->fp);
	fseek(yf->fp, 0, SEEK_SET);

	yf->data = (uint8_t*) malloc(yf->size + yarc_padding);

	if (!yf->data)
		return -1;

	if (fread(yf->data, 1, yf->size, yf->fp) != yf->size)
		return -1;

	memset(&yf->data[yf->size], 0, yarc_padding);

	fclose(yf->fp);
	yf->fp = NULL;

	return 1;
}

void yarc_file_close(yarc_file_t* yf)
{
	if (yf->fp)
	{
		fclose(yf->fp);
		yf->fp = NULL;
	}

	if (yf->filename)
	{
		free(yf->filename);
		yf->filename = NULL;
	}

	if (yf->basename)
	{
		free(yf->basename);
		yf->basename = NULL;
	}

	if (yf->identifier)
	{
		free(yf->identifier);
		yf->identifier = NULL;
	}
}

void yarc_print_help()
{
	printf(
		"YARC: Yet Another Resource Compiler\n"
		"\n"
		"Usage:\n"
		"    yarc [options] <input files>\n"
		"\n"
		"Options:\n"
		"    -o <output file>  output file (default is \"resources.c\")\n"
		"    -p <prefix>       name prefix (default is \"yarc\")\n"
		"    -b <bundle>       bundle name (default is \"default\")\n"
		"    -w <width>        hex dump width (default is 16)\n"
		"    -a <padding>      append zero padding (default is 2)\n"
		"    -u                use uppercase hex (default is lowercase)\n"
		"    -s                use static keyword on resources\n"
		"    -h                print help\n"
		"    -v                print version (%s)\n"
		"    -V                verbose mode\n"
		"\n", yarc_version
	);
}

void yarc_print_version()
{
	printf("yarc version %s\n", yarc_version);
}

int main(int argc, char** argv)
{
	int status;
	int index;
	char* arg;
	int nfiles = 0;
	yarc_file_t* out;
	yarc_file_t* file = NULL;
	yarc_file_t* files = NULL;

	if (argc < 2)
	{
		yarc_print_help();
		return 1;
	}

	files = (yarc_file_t*) calloc(argc, sizeof(yarc_file_t));

	if (!files)
		return 1;

	for (index = 1; index < argc; index++)
	{
		arg = argv[index];

		if ((strlen(arg) == 2) && (arg[0] == '-'))
		{
			switch (arg[1])
			{
				case 'o':
					if ((index + 1) < argc)
					{
						yarc_output = argv[index + 1];
						index++;
					}
					break;

				case 'p':
					if ((index + 1) < argc)
					{
						yarc_prefix = argv[index + 1];
						index++;
					}
					break;

				case 'b':
					if ((index + 1) < argc)
					{
						yarc_bundle = argv[index + 1];
						index++;
					}
					break;

				case 'w':
					if ((index + 1) < argc)
					{
						yarc_width = atoi(argv[index + 1]);
						index++;
					}
					break;

				case 'd':
					if ((index + 1) < argc)
					{
						yarc_padding = atoi(argv[index + 1]);
						index++;
					}
					break;

				case 'u':
					yarc_upper = true;
					break;

				case 's':
					yarc_static = true;
					break;

				case 'h':
					yarc_print_help();
					break;

				case 'v':
					yarc_print_version();
					break;

				case 'V':
					yarc_verbose = true;
					break;
			}

			continue;
		}

		file = &files[nfiles++];
		file->filename = _strdup(argv[index]);
		yarc_file_construct_names(file);
	}

	if (yarc_verbose)
	{
		printf("generating %s from ", yarc_output);

		for (index = 0; index < nfiles; index++)
		{
			file = &files[index];
			printf("%s%s", file->basename, index != (nfiles - 1) ? ", " : "\n");
		}
	}

	out = &files[nfiles];
	out->filename = _strdup(yarc_output);
	status = yarc_file_open(out, true);

	if (status < 1)
	{
		fprintf(stderr, "could not open file \"%s\"\n", out->filename);
		return 1;
	}

	fprintf(out->fp, "\n");

	for (index = 0; index < nfiles; index++)
	{
		file = &files[index];

		status = yarc_file_open(file, false);

		if (status < 1)
		{
			fprintf(stderr, "could not open file \"%s\"\n", file->filename);
			return 1;
		}

		status = yarc_file_write(out, file);

		if (status < 1)
			return 1;
	}

	fprintf(out->fp,
		"typedef struct {\n"
		"  const char* name;\n"
		"  const unsigned int* size;\n"
		"  const unsigned char** data;\n"
		"} %s_resource_t;\n\n", yarc_prefix);

	fprintf(out->fp, "%sconst %s_resource_t %s_%s_resources[] = {\n",
		yarc_static ? "static " : "",
		yarc_prefix, yarc_prefix, yarc_bundle);

	for (index = 0; index < nfiles; index++)
	{
		file = &files[index];
		fprintf(out->fp, "  { \"%s\", &%s_size, &%s_data },\n",
			file->basename, file->identifier, file->identifier);
	}

	fprintf(out->fp, "  { \"\", 0, 0 }\n};\n");
	fprintf(out->fp, "\n");

	yarc_file_close(out);
	free(files);

	return 0;
}

