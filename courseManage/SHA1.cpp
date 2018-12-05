#include "stdafx.h"
#include "SHA1.h"
#include <openssl\sha.h>
#include <openssl\crypto.h>
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
int SHA1File(char *filename, unsigned char digest[16])
{
	FILE *file;
	SHA_CTX context;
	int len;
	unsigned char buffer[1024];
	errno_t err;
	if ((err = fopen_s(&file, filename, "rb")) != 0)
		return -1;
	else {
		SHA1_Init(&context);
		while (len = fread(buffer, 1, 1024, file))
			SHA1_Update(&context, buffer, len);
		SHA1_Final(digest, &context);
		OPENSSL_cleanse(&context, sizeof(context));
		fclose(file);
	}
	return 0;
}