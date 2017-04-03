
#include <stdio.h>

#include "yarc.h"

YARC_EXTERN(sample, colors_json);
YARC_EXTERN(sample, squirrel_jpg);

int main(int argc, char** argv)
{
	FILE* fp;

	printf("%s", YARC_DATA(sample, colors_json));

	fp = fopen("output.jpg", "w+b");

	if (fp)
	{
		fwrite(YARC_DATA(sample, squirrel_jpg), 1, YARC_SIZE(sample, squirrel_jpg), fp);
		fclose(fp);
	}

	return 0;
}

