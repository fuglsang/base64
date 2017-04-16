#pragma once
#include <stddef.h>

size_t base64_encoded_length(size_t message_length);
size_t base64_encoded_length(const char* message);

void base64_encode(char* encoded_message, const char* message, size_t message_length);
void base64_encode(char* encoded_message, const char* message);

size_t base64_decoded_length(size_t encoded_message_length);
size_t base64_decoded_length(const char* encoded_message);

void base64_decode(char* message, const char* encoded_message, size_t encoded_message_length);
void base64_decode(char* message, const char* encoded_message);

bool base64_find(const char* substring, size_t substring_length, const char* encoded_message, size_t encoded_message_length);
bool base64_find(const char* substring, const char* encoded_message);