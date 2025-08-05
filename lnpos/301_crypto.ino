#include <mbedtls/aes.h>
#include <mbedtls/md5.h>

void encrypt(unsigned char *key, unsigned char *iv, int length, const char *plainText, unsigned char *outputBuffer)
{
  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, key, 256); // AES-256 requires a 32-byte key
  mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, length, iv, (const unsigned char *)plainText, outputBuffer);
  mbedtls_aes_free(&aes);
}

void deriveKeyAndIV(const char *secret, unsigned char *salt, unsigned char *outputBuffer)
{
  mbedtls_md5_context md5_ctx;
  unsigned char data[24];      // 16 bytes key + 8 bytes salt
  unsigned char md5Output[16]; // 16 bytes for MD5 output

  memcpy(data, secret, 16);
  memcpy(data + 16, salt, 8);

  // first iteration
  mbedtls_md5_init(&md5_ctx);
  mbedtls_md5_update(&md5_ctx, data, sizeof(data));
  mbedtls_md5_finish(&md5_ctx, md5Output);

  // Copy the first 16 bytes to the output buffer for the key
  memcpy(outputBuffer, md5Output, 16);

  unsigned char data_md5[16 + 16 + 8]; // 16 bytes md5 output + 16 bytes key + 8 bytes salt

  for (int i = 16; i <= 48; i += 16)
  {
    memcpy(data_md5, md5Output, 16);
    memcpy(data_md5 + 16, data, 24);
    mbedtls_md5_init(&md5_ctx);
    mbedtls_md5_update(&md5_ctx, data_md5, sizeof(data_md5));
    mbedtls_md5_finish(&md5_ctx, md5Output);
    // Copy the next 16 bytes to the output buffer
    memcpy(outputBuffer + i, md5Output, 16);
  }

  mbedtls_md5_free(&md5_ctx);
}
