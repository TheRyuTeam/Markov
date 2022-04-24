#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef const char *(Replacement[2]);


uint64_t replaceFirst(char* bytes, int64_t nBytes, const Replacement replacement)
{
	const char* from = replacement[0];
	const char* to = replacement[1];

	const int64_t fromLen = strlen(from);

	if (bytes == 0 || replacement == 0 || nBytes < fromLen)
		return -1;

	const int64_t toLen = strlen(to);

	if (fromLen == toLen)
	{
		for (int64_t bi = 0; bi + fromLen < nBytes + 1; ++bi)
		{
			if (memcmp(bytes + bi, from, fromLen) == 0)
			{
				memcpy(bytes + bi, to, toLen);
				return nBytes;
			}
		}
	}
	else if (fromLen > toLen)
	{
		for (int64_t bi = 0; bi + fromLen < nBytes + 1; ++bi)
		{
			if (memcmp(bytes + bi, from, fromLen) == 0)
			{
				bytes = realloc(bytes, nBytes -= fromLen - toLen);
				memcpy(bytes + bi + toLen, bytes + bi + fromLen,nBytes - bi - toLen);
				memcpy(bytes + bi, to, toLen);
				bytes[nBytes] = '\0';
				return nBytes;
			}
		}
	}
	else
	{
		for (int64_t bi = 0; bi + fromLen < nBytes + 1; ++bi)
		{
			if (memcmp(bytes + bi, from, fromLen) == 0)
			{
				bytes = realloc(bytes, nBytes += toLen - fromLen);
				for (int64_t k = nBytes - 1; k > bi + toLen - fromLen; --k)
					bytes[k] = bytes[k - toLen + fromLen];
				memcpy(bytes + bi, to, toLen);
				bytes[nBytes] = '\0';
				return nBytes;
			}
		}
	}
	return -1;
}

int64_t markov(char* bytes, int64_t nBytes,
			   const Replacement* replacements, const uint64_t nReplacements)
{
	bool isChanged = true;
	while (isChanged)
	{
		isChanged = false;
		for (int64_t ri = 0; ri != nReplacements; ++ri)
		{
			int64_t ret = replaceFirst(bytes, nBytes, replacements[ri]);
			if (ret != -1)
			{
				nBytes = ret;
				isChanged = true;
				ri = -1;
			}
		}
	}
	return nBytes;
}

char* add2markov(const char* src)
{
	if (src == 0)
		return 0;

	for (const char *ptr = src; *ptr; ++ptr)
	{
		if (*ptr != '|')
			return 0;
	}

	const uint64_t len = strlen(src);
	char* data = malloc(len + 2);
	*data = '*';
	strcpy(data + 1, src);

	Replacement replacements[2] = {
		{"*|", "|*"},
		{"*", "||"}
	};

	markov(data, strlen(data), replacements, 2);
	return data;
}