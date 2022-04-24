#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


/*!
 * @brief Replace first occurrence of replacement[0] with replacement[1]
 * @param bytes data for replacing occurrence
 * @param nBytes amount of bytes
 * @param replacement array of 2 null terminated strings for replacing
 * @return if reallocation was happened new bytes length otherwise return old length,
 * if bytes is null return -1
 */
uint64_t replaceFirst(char* bytes, int64_t nBytes, const char* replacement[2])
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

/*!
 * @brief Applies Markov algorithm on given bytes
 * @param bytes data for replacing occurrence
 * @param nBytes amount of bytes
 * @param replacements array of arrays of 2 null terminated strings for replacing
 * @param nReplacements amount of replacements
 * @return new bytes size if any reallocation happened otherwise return old bytes size,
 * if bytes is null or replacements is null return -1
 * @see replaceFirst
 */
int64_t markov(char* bytes, int64_t nBytes,
			   const char* (*replacements)[2], const uint64_t nReplacements)
{
	if (bytes == 0 || replacements == 0)
		return -1;

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

/*!
 * @brief Add 2 to given unit number system number
 * @param src given unit number system number
 * @return if src is null return -1 otherwise given number increased by 2
 * @note returned string must be free!
 * @see markov
 */
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

	const char* replacements[2][2] = {
		{"*|", "|*"},
		{"*", "||"}
	};

	markov(data, strlen(data), replacements, 2);
	return data;
}