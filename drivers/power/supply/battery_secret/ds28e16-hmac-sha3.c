// SPDX-License-Identifier: GPL-2.0-only

#include <crypto/sha3.h>
#include <linux/errno.h>
#include <linux/string.h>

#include "ds28e16-hmac-sha3.h"

int ds28e16_hmac_sha3_256(const u8 *key, int key_len, const u8 *message,
			  int msg_len, u8 *mac)
{
	struct sha3_ctx ctx;
	u8 pad[SHA3_256_BLOCK_SIZE];
	u8 digest[SHA3_256_DIGEST_SIZE];
	int i;

	if (!key || !message || !mac || key_len < 0 ||
	    key_len > SHA3_256_BLOCK_SIZE || msg_len < 0 || msg_len > 512)
		return -EINVAL;

	memset(pad, 0x36, sizeof(pad));
	for (i = 0; i < key_len; i++)
		pad[i] ^= key[i];

	sha3_256_init(&ctx);
	sha3_update(&ctx, pad, sizeof(pad));
	sha3_update(&ctx, message, msg_len);
	sha3_final(&ctx, digest);

	for (i = 0; i < SHA3_256_BLOCK_SIZE; i++)
		pad[i] ^= 0x36 ^ 0x5c;

	sha3_256_init(&ctx);
	sha3_update(&ctx, pad, sizeof(pad));
	sha3_update(&ctx, digest, sizeof(digest));
	sha3_final(&ctx, mac);

	memzero_explicit(pad, sizeof(pad));
	memzero_explicit(digest, sizeof(digest));

	return 0;
}
