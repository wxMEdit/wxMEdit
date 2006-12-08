#include "chardetect.h"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define LIB_PREFIX	"../"
#define LIB_NAME	"chardetect.so"
#define LIB_PATH	LIB_PREFIX LIB_NAME

static void (*chardet_func)(chardet_t *);
static void (*chardet_func2)(chardet_t);
static int (*chardet_func_with_param)(chardet_t, char *, int);
static void *handle;
static const char *error;

static void perform_charset_detection(FILE* fp)
{
	char buf[4096];
	char encoding[CHARDET_MAX_ENCODING_NAME];
	size_t len;
	int res = 0;
	chardet_t det = NULL;

	handle = dlopen(LIB_PATH, RTLD_NOW);
	if (! handle) {
		fputs(dlerror(), stderr);
		return;
	}

	chardet_func = dlsym(handle, "chardet_create");
	if ((error = dlerror()) != NULL)  {
		fputs(error, stderr);
		return;
	}
	(*chardet_func)(&det);

	chardet_func_with_param = dlsym(handle, "chardet_handle_data");
	if ((error = dlerror()) != NULL)  {
		fputs(error, stderr);
		return;
	}
	do {
		len = fread(buf, 1, sizeof(buf), fp);
		res = (*chardet_func_with_param)(det, buf, len);
	} while ((res == CHARDET_RESULT_OK) && (feof(fp) == 0));

	chardet_func2 = dlsym(handle, "chardet_data_end");
	if ((error = dlerror()) != NULL)  {
		fputs(error, stderr);
		return;
	}
	(*chardet_func2)(det);

	chardet_func_with_param = dlsym(handle, "chardet_get_charset");
	if ((error = dlerror()) != NULL)  {
		fputs(error, stderr);
		return;
	}
	(*chardet_func_with_param)(det, encoding, CHARDET_MAX_ENCODING_NAME);
	printf("Charset = %s\n", encoding);

	chardet_func2 = dlsym(handle, "chardet_destroy");
	if ((error = dlerror()) != NULL)  {
		fputs(error, stderr);
		return;
	}
	(*chardet_func2)(det);

	dlclose(handle);
}

int main(int argc, char* argv[])
{
	FILE* fp = NULL;

	if (argc > 1) {
		fp = fopen(argv[1], "rb");
		if (fp) {
			printf("File %s ...\n", argv[1]);
			perform_charset_detection(fp);
			fclose(fp);
		}
		else {
			printf("Can't open %s\n", argv[1]);
			return 1;
		}
	}
	else {
		printf("Usage: %s filename\n", argv[0]);
		return 1;
	}

	return 0;
}

