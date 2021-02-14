#pragma once
#ifndef OPENSSL_USE_N
#define OPENSSL_USE_N

#include<openssl/ssl.h>
#include<openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/applink.c>
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")


#endif // !OPENSSL_USE_N
