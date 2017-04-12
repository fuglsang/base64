#include <string.h>

#if 0
#define DEBUG_FIND
#include <stdio.h>
#endif

//--------
// TABLES
//

static const char base64_char[64] =
{
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/',
};

static const char base64_bits[128] =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1,  0, -1, -1,// second from right is '='
	-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
};

//--------
// ENCODE
//

size_t base64_encoded_length(size_t message_length)
{
	int num_blocks = message_length / 3;
	int num_excess = message_length % 3;
	if (num_excess > 0)
		num_blocks++;

	return (4 * num_blocks) + 1;
}

size_t base64_encoded_length(const char* message)
{
	return base64_encoded_length(strlen(message));
}

void base64_encode(char* encoded_message, const char* message, size_t message_length)
{
	int num_blocks = message_length / 3;
	int num_excess = message_length % 3;

	for (int block = 0; block != num_blocks; block++)
	{
		const char* message_block = &message[3 * block];

		*(encoded_message++) = base64_char[63 & ((message_block[0] >> 2))];
		*(encoded_message++) = base64_char[63 & ((message_block[0] << 4) | (message_block[1] >> 4))];
		*(encoded_message++) = base64_char[63 & ((message_block[1] << 2) | (message_block[2] >> 6))];
		*(encoded_message++) = base64_char[63 & ((message_block[2]))];
	}

	if (num_excess > 0)
	{
		const char* message_block = &message[3 * num_blocks];

		*(encoded_message++) = base64_char[63 & ((message_block[0] >> 2))];

		if (num_excess > 1)
		{
			*(encoded_message++) = base64_char[63 & ((message_block[0] << 4) | (message_block[1] >> 4))];
			*(encoded_message++) = base64_char[63 & ((message_block[1] << 2))];
			*(encoded_message++) = '=';
		}
		else
		{
			*(encoded_message++) = base64_char[63 & ((message_block[0] << 4))];
			*(encoded_message++) = '=';
			*(encoded_message++) = '=';
		}
	}

	*encoded_message = '\0';
}

void base64_encode(char* encoded_message, const char* message)
{
	base64_encode(encoded_message, message, strlen(message));
}

//--------
// DECODE
//

size_t base64_decoded_length(size_t encoded_message_length)
{
	int num_blocks = encoded_message_length / 4;
	int num_excess = encoded_message_length % 4;
	if (num_excess > 0)
		num_blocks++;

	return (3 * num_blocks) + 1;
}

size_t base64_decoded_length(const char* encoded_message)
{
	return base64_decoded_length(strlen(encoded_message));
}

void base64_decode(char* message, const char* encoded_message, size_t encoded_message_length)
{
	int num_blocks = encoded_message_length / 4;
	int num_excess = encoded_message_length % 4;

	for (int block = 0; block != num_blocks; block++)
	{
		const char* encoded_message_block = &encoded_message[4 * block];

		char block0 = base64_bits[encoded_message_block[0]];
		char block1 = base64_bits[encoded_message_block[1]];
		char block2 = base64_bits[encoded_message_block[2]];
		char block3 = base64_bits[encoded_message_block[3]];

		*(message++) = 127 & ((block0 << 2) | (block1 >> 4));
		*(message++) = 127 & ((block1 << 4) | (block2 >> 2));
		*(message++) = 127 & ((block2 << 6) | (block3));
	}

	if (num_excess > 1)
	{
		const char* encoded_message_block = &encoded_message[4 * num_blocks];

		if (num_excess > 2)
		{
			char block0 = base64_bits[encoded_message_block[0]];
			char block1 = base64_bits[encoded_message_block[1]];
			char block2 = base64_bits[encoded_message_block[2]];

			*(message++) = ((block0 << 2) | (block1 >> 4));
			*(message++) = ((block1 << 4) | (block2 >> 2));
		}
		else
		{
			char block0 = base64_bits[encoded_message_block[0]];
			char block1 = base64_bits[encoded_message_block[1]];

			*(message++) = ((block0 << 2) | (block1 >> 4));
		}
	}

	*message = '\0';
}

void base64_decode(char* message, const char* encoded_message)
{
	base64_decode(message, encoded_message, strlen(encoded_message));
}

//--------
// SEARCH
//

bool base64_find(const char* substring, size_t substring_length, const char* encoded_message, size_t encoded_message_length)
{
	int num_substring_indices = substring_length;
	if (num_substring_indices == 0)
		return false;

	int num_message_indices = encoded_message_length;
	if (num_message_indices > 0 && encoded_message[num_message_indices - 1] == '=')
		num_message_indices--;
	if (num_message_indices > 0 && encoded_message[num_message_indices - 1] == '=')
		num_message_indices--;

	int substring_start = 0;
	int substring_index = 0;

	int stream_buffer = 0;
	int stream_length = 0;

	for (int message_index = 0; message_index != num_message_indices; message_index++)
	{
		stream_buffer |= base64_bits[encoded_message[message_index]];
		stream_length += 6;

		int stream_excess = stream_length - 8;
		if (stream_excess >= 0)
		{
			stream_length = stream_excess;

			char symbol = 127 & (stream_buffer >> stream_excess);
#ifdef DEBUG_FIND
			printf("decoded: %c  stream_length %d stream_excess %d\n", symbol, stream_length, stream_excess);
#endif
			if (symbol == substring[substring_index])
			{
				substring_index++;
				if (substring_index == 1)
					substring_start = message_index;
				if (substring_index >= num_substring_indices)
					return true;
			}
			else if (substring_index > 0)
			{
				substring_index = stream_length = 0;
				message_index = substring_start;
			}
		}

		stream_buffer <<= 6;
	}

	return false;
}

bool base64_find(const char* substring, const char* encoded_message)
{
	return base64_find(substring, strlen(substring), encoded_message, strlen(encoded_message));
}