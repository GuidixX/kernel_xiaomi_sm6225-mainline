/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _DS28E16_HMAC_SHA3_H
#define _DS28E16_HMAC_SHA3_H

#include <linux/types.h>

int ds28e16_hmac_sha3_256(const u8 *key, int key_len, const u8 *message,
			  int msg_len, u8 *mac);

#endif
