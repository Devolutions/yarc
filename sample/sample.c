
#include <yarc/yarc.h>

YARC_EXTERN_BUNDLE(sample)
YARC_EXTERN(sample, colors_json);
YARC_EXTERN(sample, squirrel_jpg);

int main(int argc, char** argv)
{
	FILE* fp;
	int size = 0;
	uint8_t* data = NULL;
	const char* name = "rfc4960.txt";

	yarc_bundle_open(&yarc_sample_bundle);

	printf("%s", YARC_DATA(sample, colors_json));

	fp = fopen("output.jpg", "w+b");

	if (fp)
	{
		fwrite(YARC_DATA(sample, squirrel_jpg), 1, YARC_SIZE(sample, squirrel_jpg), fp);
		fclose(fp);
	}

	const unsigned char* yarc_bundle_find(yarc_bundle_t* bundle, const char* name, int* size);
	
	data = (uint8_t*) yarc_bundle_find(&yarc_sample_bundle, name, &size);

	if (data)
	{
		fp = fopen(name, "w+b");

		fwrite(data, 1, size, fp);
		fclose(fp);
	}

	yarc_bundle_close(&yarc_sample_bundle);

	return 0;
}

