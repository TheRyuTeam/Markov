#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef
struct _Replacement
{
	const char* from;
	const int64_t fromLen;
	const char* to;
	const int64_t toLen;
} Replacement;


uint64_t replaceFirst(char* bytes, int64_t nBytes, const Replacement replacement)
{
	if (bytes == 0 || nBytes < replacement.fromLen)
		return -1;

	if (replacement.fromLen == replacement.toLen)
	{
		for (int64_t bi = 0; bi + replacement.fromLen < nBytes + 1; ++bi)
		{
			if (memcmp(bytes + bi, replacement.from, replacement.fromLen) == 0)
			{
				memcpy(bytes + bi, replacement.to, replacement.toLen);
				return nBytes;
			}
		}
	}
	else if (replacement.fromLen > replacement.toLen)
	{
		for (int64_t bi = 0; bi + replacement.fromLen < nBytes + 1; ++bi)
		{
			if (memcmp(bytes + bi, replacement.from, replacement.fromLen) == 0)
			{
				bytes = realloc(bytes, nBytes -= replacement.fromLen - replacement.toLen);
				memcpy(bytes + bi + replacement.toLen, bytes + bi + replacement.fromLen,nBytes - bi - replacement.toLen);
				memcpy(bytes + bi, replacement.to, replacement.toLen);
				bytes[nBytes] = '\0';
				return nBytes;
			}
		}
	}
	else
	{
		for (int64_t bi = 0; bi + replacement.fromLen < nBytes + 1; ++bi)
		{
			if (memcmp(bytes + bi, replacement.from, replacement.fromLen) == 0)
			{
				bytes = realloc(bytes, nBytes += replacement.toLen - replacement.fromLen);
				for (int64_t k = nBytes - 1; k > bi + replacement.toLen - replacement.fromLen; --k)
					bytes[k] = bytes[k - replacement.toLen + replacement.fromLen];
				memcpy(bytes + bi, replacement.to, replacement.toLen);
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

Replacement strReplacement(const char* from, const char* to)
{
	Replacement r = {.from = from, .fromLen = strlen(from), .to = to, .toLen = strlen(to)};
	return r;
}

char* add2markov(const char* src)
{
	char* data = malloc(strlen(src) + 2);
	*data = '*';
	strcpy(data + 1, src);

	Replacement replacements[] = {
		strReplacement("*|", "|*"),
		strReplacement("*", "||")
	};

	markov(data, strlen(data), replacements, 2);
	return data;
}