/**
 * \file camellia.h
 *
 *  Copyright (C) 2009  Paul Bakker <polarssl_maintainer at polarssl dot org>
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of PolarSSL or XySSL nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef POLARSSL_CAMELLIA_H
#define POLARSSL_CAMELLIA_H

/* Fixed compatibility of XTEA and Camellia on a 64-bit system
 *   (found by Felix von Leitner).
 * Changed by Handy on 12-30-2010
 */
#ifdef _MSC_VER
#include <basetsd.h>
typedef UINT32 uint32_t;
#else
#include <inttypes.h>
#endif

#define CAMELLIA_ENCRYPT     1
#define CAMELLIA_DECRYPT     0

#define POLARSSL_ERR_CAMELLIA_INVALID_KEY_LENGTH            -0x0a00
#define POLARSSL_ERR_CAMELLIA_INVALID_INPUT_LENGTH          -0x0a10

/**
 * \brief          CAMELLIA context structure
 */
typedef struct
{
    int nr;                     /*!<  number of rounds  */
    uint32_t rk[68];            /*!<  CAMELLIA round keys    */
}
camellia_context;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          CAMELLIA key schedule (encryption)
 *
 * \param ctx      CAMELLIA context to be initialized
 * \param key      encryption key
 * \param keysize  must be 128, 192 or 256
 * 
 * \return         0 if successful, or POLARSSL_ERR_CAMELLIA_INVALID_KEY_LENGTH
 */
int camellia_setkey_enc( camellia_context *ctx, const unsigned char *key, int keysize );

/**
 * \brief          CAMELLIA key schedule (decryption)
 *
 * \param ctx      CAMELLIA context to be initialized
 * \param key      decryption key
 * \param keysize  must be 128, 192 or 256
 * 
 * \return         0 if successful, or POLARSSL_ERR_CAMELLIA_INVALID_KEY_LENGTH
 */
int camellia_setkey_dec( camellia_context *ctx, const unsigned char *key, int keysize );

/**
 * \brief          CAMELLIA-ECB block encryption/decryption
 *
 * \param ctx      CAMELLIA context
 * \param mode     CAMELLIA_ENCRYPT or CAMELLIA_DECRYPT
 * \param input    16-byte input block
 * \param output   16-byte output block
 */
int camellia_crypt_ecb( camellia_context *ctx,
                    int mode,
                    const unsigned char input[16],
                    unsigned char output[16] );

/**
 * \brief          CAMELLIA-CBC buffer encryption/decryption
 *                 Length should be a multiple of the block
 *                 size (16 bytes)
 *
 * \param ctx      CAMELLIA context
 * \param mode     CAMELLIA_ENCRYPT or CAMELLIA_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
int camellia_crypt_cbc( camellia_context *ctx,
                    int mode,
                    int length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output );

/**
 * \brief          CAMELLIA-CFB128 buffer encryption/decryption
 *
 * \param ctx      CAMELLIA context
 * \param mode     CAMELLIA_ENCRYPT or CAMELLIA_DECRYPT
 * \param length   length of the input data
 * \param iv_off   offset in IV (updated after use)
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
int camellia_crypt_cfb128( camellia_context *ctx,
                       int mode,
                       int length,
                       int *iv_off,
                       unsigned char iv[16],
                       const unsigned char *input,
                       unsigned char *output );

/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int camellia_self_test( int verbose );

#ifdef __cplusplus
}
#endif

#endif /* camellia.h */
