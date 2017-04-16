#include "base64.h"

#include <malloc.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	const char* input_message = "here is a large haystack";

	char* encoded_message = (char*)malloc(base64_encoded_length(input_message));
	base64_encode(encoded_message, input_message);

	char* decoded_message = (char*)malloc(base64_decoded_length(encoded_message));
	base64_decode(decoded_message, encoded_message);

	printf("%s -> %s -> %s\n", input_message, encoded_message, decoded_message);

	base64_find("is a", encoded_message);

	getchar();
}