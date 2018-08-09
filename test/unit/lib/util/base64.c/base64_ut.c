/*-
 *   BSD LICENSE
 *
 *   Copyright (c) Intel Corporation.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "spdk/stdinc.h"

#include "spdk_cunit.h"

#include "util/base64.c"

char text_A[] = "FZB3";
uint8_t raw_A[] = {0x15, 0x90, 0x77};
char text_B[] = "AbC/1+c=";
char text_urlsafe_B[] = "AbC_1-c=";
uint8_t raw_B[] = {0x01, 0xB0, 0xBF, 0xD7, 0xE7};
char text_C[] = "AbC/1+cC";
char text_urlsafe_C[] = "AbC_1-cC";
uint8_t raw_C[] = {0x01, 0xB0, 0xBF, 0xD7, 0xE7, 0x02};
char text_D[] = "AbC/1w==";
char text_urlsafe_D[] = "AbC_1w==";
uint8_t raw_D[] = {0x01, 0xB0, 0xBF, 0xD7};
char text_E[] = "AbC12===";
char text_F[] = "AbCd112";
char text_G[] = "AbCd12";
char text_H[] = "AbC12";

static void
test_base64_get_encoded_strlen(void)
{
	uint32_t raw_lens[4] = {8, 9, 10, 11};
	uint32_t text_strlens[4] = {12, 12, 16, 16};
	uint32_t text_strlen;
	int i;

	for (i = 0; i < 4; i++) {
		text_strlen = spdk_base64_get_encoded_strlen(raw_lens[i]);
		CU_ASSERT_EQUAL(text_strlen, text_strlens[i]);
	}
}

static void
test_base64_get_decoded_len(void)
{
	uint32_t text_strlens[4] = {8, 10, 11, 12};
	uint32_t raw_lens[4] = {6, 7, 8, 9};
	uint32_t bin_len;
	int i;

	for (i = 0; i < 4; i++) {
		bin_len = spdk_base64_get_decoded_len(text_strlens[i]);
		CU_ASSERT_EQUAL(bin_len, raw_lens[i]);
	}
}

static void
test_base64_encode(void)
{
	char text[100];
	int ret;

	ret = spdk_base64_encode(text, raw_A, sizeof(raw_A));
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT(strcmp(text, text_A) == 0);
	CU_ASSERT_EQUAL(strlen(text), strlen(text_A));

	ret = spdk_base64_encode(text, raw_B, sizeof(raw_B));
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT(strcmp(text, text_B) == 0);
	CU_ASSERT_EQUAL(strlen(text), strlen(text_B));

	ret = spdk_base64_encode(text, raw_C, sizeof(raw_C));
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT(strcmp(text, text_C) == 0);

	ret = spdk_base64_encode(text, raw_D, sizeof(raw_D));
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT(strcmp(text, text_D) == 0);

	ret = spdk_base64_encode(NULL, raw_A, sizeof(raw_A));
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_encode(text, NULL, sizeof(raw_A));
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_encode(text, raw_A, 0);
	CU_ASSERT_EQUAL(ret, -EINVAL);
}

static void
test_base64_decode(void)
{
	char raw_buf[100];
	void *raw = (void *)raw_buf;
	size_t raw_len;
	int ret;

	ret = spdk_base64_decode(raw, &raw_len, text_A);
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT_EQUAL(raw_len, sizeof(raw_A));
	CU_ASSERT(memcmp(raw, raw_A, sizeof(raw_A)) == 0);

	ret = spdk_base64_decode(raw, &raw_len, text_B);
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT_EQUAL(raw_len, sizeof(raw_B));
	CU_ASSERT(memcmp(raw, raw_B, sizeof(raw_B)) == 0);

	ret = spdk_base64_decode(raw, &raw_len, text_C);
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT_EQUAL(raw_len, sizeof(raw_C));
	CU_ASSERT(memcmp(raw, raw_C, sizeof(raw_C)) == 0);

	ret = spdk_base64_decode(raw, &raw_len, text_D);
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT_EQUAL(raw_len, sizeof(raw_D));
	CU_ASSERT(memcmp(raw, raw_D, sizeof(raw_D)) == 0);

	ret = spdk_base64_decode(raw, &raw_len, text_E);
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_decode(raw, &raw_len, text_F);
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_decode(raw, &raw_len, text_G);
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_decode(raw, &raw_len, text_H);
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_decode(NULL, &raw_len, text_H);
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_decode(raw, &raw_len, NULL);
	CU_ASSERT_EQUAL(ret, -EINVAL);
}

static void
test_base64_urlsafe_encode(void)
{
	char text[100];
	int ret;

	ret = spdk_base64_urlsafe_encode(text, raw_A, sizeof(raw_A));
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT(strcmp(text, text_A) == 0);
	CU_ASSERT_EQUAL(strlen(text), strlen(text_A));

	ret = spdk_base64_urlsafe_encode(text, raw_B, sizeof(raw_B));
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT(strcmp(text, text_urlsafe_B) == 0);
	CU_ASSERT_EQUAL(strlen(text), strlen(text_urlsafe_B));

	ret = spdk_base64_urlsafe_encode(text, raw_C, sizeof(raw_C));
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT(strcmp(text, text_urlsafe_C) == 0);

	ret = spdk_base64_urlsafe_encode(text, raw_D, sizeof(raw_D));
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT(strcmp(text, text_urlsafe_D) == 0);

	ret = spdk_base64_urlsafe_encode(NULL, raw_A, sizeof(raw_A));
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_urlsafe_encode(text, NULL, sizeof(raw_A));
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_urlsafe_encode(text, raw_A, 0);
	CU_ASSERT_EQUAL(ret, -EINVAL);
}

static void
test_base64_urlsafe_decode(void)
{
	char raw_buf[100];
	void *raw = (void *)raw_buf;
	size_t raw_len;
	int ret;

	ret = spdk_base64_urlsafe_decode(raw, &raw_len, text_A);
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT_EQUAL(raw_len, sizeof(raw_A));
	CU_ASSERT(memcmp(raw, raw_A, sizeof(raw_A)) == 0);

	ret = spdk_base64_urlsafe_decode(raw, &raw_len, text_urlsafe_B);
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT_EQUAL(raw_len, sizeof(raw_B));
	CU_ASSERT(memcmp(raw, raw_B, sizeof(raw_B)) == 0);

	ret = spdk_base64_urlsafe_decode(raw, &raw_len, text_urlsafe_C);
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT_EQUAL(raw_len, sizeof(raw_C));
	CU_ASSERT(memcmp(raw, raw_C, sizeof(raw_C)) == 0);

	ret = spdk_base64_urlsafe_decode(raw, &raw_len, text_urlsafe_D);
	CU_ASSERT_EQUAL(ret, 0);
	CU_ASSERT_EQUAL(raw_len, sizeof(raw_D));
	CU_ASSERT(memcmp(raw, raw_D, sizeof(raw_D)) == 0);

	ret = spdk_base64_urlsafe_decode(raw, &raw_len, text_E);
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_urlsafe_decode(raw, &raw_len, text_F);
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_urlsafe_decode(raw, &raw_len, text_G);
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_urlsafe_decode(raw, &raw_len, text_H);
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_urlsafe_decode(NULL, &raw_len, text_H);
	CU_ASSERT_EQUAL(ret, -EINVAL);
	ret = spdk_base64_urlsafe_decode(raw, &raw_len, NULL);
	CU_ASSERT_EQUAL(ret, -EINVAL);
}

int
main(int argc, char **argv)
{
	CU_pSuite	suite = NULL;
	unsigned int	num_failures;

	if (CU_initialize_registry() != CUE_SUCCESS) {
		return CU_get_error();
	}

	suite = CU_add_suite("base64", NULL, NULL);
	if (suite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (
		CU_add_test(suite, "test_base64_get_encoded_strlen", test_base64_get_encoded_strlen) == NULL ||
		CU_add_test(suite, "test_base64_get_decoded_len",
			    test_base64_get_decoded_len) == NULL ||
		CU_add_test(suite, "test_base64_encode", test_base64_encode) == NULL ||
		CU_add_test(suite, "test_base64_decode", test_base64_decode) == NULL ||
		CU_add_test(suite, "test_base64_urlsafe_encode", test_base64_urlsafe_encode) == NULL ||
		CU_add_test(suite, "test_base64_urlsafe_decode", test_base64_urlsafe_decode) == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);

	CU_basic_run_tests();

	num_failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	return num_failures;
}