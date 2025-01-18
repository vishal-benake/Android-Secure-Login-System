#include "Includes.h"
#include "Substrate/CydiaSubstrate.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/md5.h>

namespace Tools
{
    

    std::string CalcMD5(std::string s);
    std::string CalcSHA256(std::string s);
}
