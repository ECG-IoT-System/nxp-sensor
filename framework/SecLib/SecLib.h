/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
* \file
*
* This is the header file for the security module that implements AES.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _SEC_LIB_H_
#define _SEC_LIB_H_


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"

#ifndef gSecLibUseMutex_c
#define gSecLibUseMutex_c   TRUE
#endif

#define mRevertEcdhKeys_d   1

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
/* AES 128 */
#define AES_128_KEY_BITS         128
#define AES_128_BLOCK_SIZE       16 /* [bytes] */

/* CCM */
#define gSecLib_CCM_Encrypt_c 0
#define gSecLib_CCM_Decrypt_c 1

/* Hashes */
#define SHA1_HASH_SIZE     20 /* [bytes] */
#define SHA1_BLOCK_SIZE    64 /* [bytes] */

#define SHA256_HASH_SIZE   32 /* [bytes] */
#define SHA256_BLOCK_SIZE  64 /* [bytes] */

#define gHmacIpad_c               0x36
#define gHmacOpad_c               0x5C

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef enum
{
    gSecSuccess_c       = 0u,
    gSecAllocError_c    = 1u,
    gSecError_c         = 2u
} secResultType_t;

typedef enum ecdhStatus_tag {
    gEcdhSuccess_c,
    gEcdhBadParameters_c,
    gEcdhOutOfMemory_c,
    gEcdhRngError_c,
    gEcdhInvalidState_c
} ecdhStatus_t;

typedef union ecdhPrivateKey_tag {
    uint8_t  raw_8bit[32];
    uint32_t raw_32bit[8];
} ecdhPrivateKey_t;

typedef PACKED_UNION ecdhPoint_tag {
    uint8_t raw[64];
    PACKED_STRUCT {
        uint8_t x[32];
        uint8_t y[32];
    } components_8bit;
    PACKED_STRUCT {
        uint32_t x[8];
        uint32_t y[8];
    } components_32bit;
} ecdhPoint_t;
    
typedef ecdhPoint_t ecdhPublicKey_t;

typedef ecdhPoint_t ecdhDhKey_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  This function performs initialization of the cryptografic HW acceleration.
*
********************************************************************************** */
void SecLib_Init (void);

/*! *********************************************************************************
* \brief  This function performs AES-128 encryption on a 16-byte block.
*
* \param[in]  pInput Pointer to the location of the 16-byte plain text block.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte ciphered output.
*
* \pre All Input/Output pointers must refer to a memory address alligned to 4 bytes!
*
********************************************************************************** */
void AES_128_Encrypt (const uint8_t*    pInput,
                      const uint8_t*    pKey,
                      uint8_t*          pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128 decryption on a 16-byte block.
*
* \param[in]  pInput Pointer to the location of the 16-byte plain text block.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte ciphered output.
*
* \pre All Input/Output pointers must refer to a memory address alligned to 4 bytes!
*
********************************************************************************** */
void AES_128_Decrypt (const uint8_t*    pInput,
                      const uint8_t*    pKey,
                      uint8_t*          pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-ECB encryption on a message block.
*         This function only acepts input lengths which are multiple
*         of 16 bytes (AES 128 block size).
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
********************************************************************************** */
void AES_128_ECB_Encrypt (uint8_t*  pInput, 
                          uint32_t  inputLen, 
                          uint8_t*  pKey, 
                          uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CBC encryption on a message block.
*         This function only acepts input lengths which are multiple
*         of 16 bytes (AES 128 block size).
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
********************************************************************************** */
void AES_128_CBC_Encrypt (uint8_t*  pInput, 
                          uint32_t  inputLen,
                          uint8_t*  pInitVector, 
                          uint8_t*  pKey, 
                          uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CBC encryption on a message block after
*         padding it with 1 bit of 1 and 0 bits trail.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes. 
*
*             IMPORTANT: User must make sure that input and output
*             buffers have at least inputLen + 16 bytes size
*
* \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
* Return value: size of output buffer (after padding)
*
********************************************************************************** */
uint32_t AES_128_CBC_Encrypt_And_Pad (uint8_t*  pInput, 
                                      uint32_t  inputLen,
                                      uint8_t*  pInitVector, 
                                      uint8_t*  pKey, 
                                      uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CBC decryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
* Return value: size of output buffer (after depadding the 0x80 0x00 ... padding sequence)
*
********************************************************************************** */
uint32_t AES_128_CBC_Decrypt_And_Depad (uint8_t*    pInput, 
                                        uint32_t    inputLen,
                                        uint8_t*    pInitVector, 
                                        uint8_t*    pKey, 
                                        uint8_t*    pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CTR encryption on a message block.
*         This function only acepts input lengths which are multiple
*         of 16 bytes (AES 128 block size).
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pCounter Pointer to the location of the 128-bit counter.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
********************************************************************************** */
void AES_128_CTR (uint8_t*  pInput, 
                  uint32_t  inputLen,
                  uint8_t*  pCounter, 
                  uint8_t*  pKey, 
                  uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-OFB encryption on a message block.
*         This function only acepts input lengths which are multiple
*         of 16 bytes (AES 128 block size).
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
********************************************************************************** */
void AES_128_OFB (uint8_t*  pInput, 
                  uint32_t  inputLen,
                  uint8_t*  pInitVector, 
                  uint8_t*  pKey, 
                  uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CMAC on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Length of the input message in bytes. The input data must be provided MSB first.
*
* \param[in]  pKey Pointer to the location of the 128-bit key. The key must be provided MSB first.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte authentication code. The code will be generated MSB first.
*
* \remarks This is public open source code! Terms of use must be checked before use!
*
********************************************************************************** */
void AES_128_CMAC (uint8_t* pInput, 
                   uint32_t inputLen, 
                   uint8_t* pKey, 
                   uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CMAC on a message block accepting input data
*         which is in LSB first format and computing the authentication code starting fromt he end of the data.
* 
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Length of the input message in bytes. The input data must be provided LSB first.
*
* \param[in]  pKey Pointer to the location of the 128-bit key. The key must be provided MSB first.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte authentication code. The code will be generated MSB first.
*
********************************************************************************** */
void AES_128_CMAC_LsbFirstInput (uint8_t*   pInput,
                                 uint32_t   inputLen,
                                 uint8_t*   pKey,
                                 uint8_t*   pOutput);

/*! *********************************************************************************
* \brief  This function performs AES 128 CMAC Pseudo-Random Function (AES-CMAC-PRF-128),
*         according to rfc4615, on a message block.
* 
* \details The AES-CMAC-PRF-128 algorithm behaves similar to the AES CMAC 128 algorithm
*          but removes 128 bit key size restriction.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Length of the input message in bytes.
*
* \param[in]  pVarKey Pointer to the location of the variable length key.
*
* \param[in]  varKeyLen Length of the input key in bytes
*
* \param[out]  pOutput Pointer to the location to store the 16-byte pseudo random variable.
*
********************************************************************************** */
void AES_CMAC_PRF_128 (uint8_t* pInput, 
                       uint32_t inputLen, 
                       uint8_t* pVarKey,
                       uint32_t varKeyLen,
                       uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-EAX encryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Length of the input message in bytes.
*
* \param[in]  pNonce Pointer to the location of the nonce.
*
* \param[in]  nonceLen Nonce length in bytes.
*
* \param[in]  pHeader Pointer to the location of header.
*
* \param[in]  headerLen Header length in bytes.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
*
* \param[out]  pTag Pointer to the location to store the 128-bit tag.
*
********************************************************************************** */
secResultType_t AES_128_EAX_Encrypt (uint8_t*   pInput, 
                                     uint32_t   inputLen,
                                     uint8_t*   pNonce,
                                     uint32_t   nonceLen,
                                     uint8_t*   pHeader,
                                     uint8_t    headerLen, 
                                     uint8_t*   pKey, 
                                     uint8_t*   pOutput,
                                     uint8_t*   pTag);

/*! *********************************************************************************
* \brief  This function performs AES-128-EAX decryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Length of the input message in bytes.
*
* \param[in]  pNonce Pointer to the location of the nonce.
*
* \param[in]  nonceLen Nonce length in bytes.
*
* \param[in]  pHeader Pointer to the location of header.
*
* \param[in]  headerLen Header length in bytes.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
*
* \param[out]  pTag Pointer to the location to store the 128-bit tag.
*
********************************************************************************** */
secResultType_t AES_128_EAX_Decrypt (uint8_t*   pInput, 
                                     uint32_t   inputLen,
                                     uint8_t*   pNonce,
                                     uint32_t   nonceLen,
                                     uint8_t*   pHeader,
                                     uint8_t    headerLen, 
                                     uint8_t*   pKey, 
                                     uint8_t*   pOutput,
                                     uint8_t*   pTag);
                                    
/*! *********************************************************************************
* \brief  This function performs AES-128-CCM on a message block.
*
* \param[in]  pInput       Pointer to the location of the input message (plaintext or cyphertext).
*
* \param[in]  inputLen     Length of the input plaintext in bytes when encrypting.
*                          Length of the input cypertext without the MAC length when decrypting.
* 
* \param[in]  pAuthData    Pointer to the additional authentication data.
*
* \param[in]  authDataLen  Length of additional authentication data.
*
* \param[in]  pNonce       Pointer to the Nonce.
*
* \param[in]  nonceSize    The size of the nonce (7-13).
*
* \param[in]  pKey         Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput     Pointer to the location to store the plaintext data when ecrypting.
*                          Pointer to the location to store the cyphertext data when ecrypting.
*
* \param[out]  pCbcMac     Pointer to the location to store the Message Authentication Code (MAC) when encrypting.
*                          Pointer to the location where the received MAC can be found when decrypting.
*
* \param[out]  macSize     The size of the MAC.
*
* \param[out]  flags       Select encrypt/decrypt operations (gSecLib_CCM_Encrypt_c, gSecLib_CCM_Decrypt_c)
*
********************************************************************************** */
uint8_t AES_128_CCM (uint8_t*   pInput,
                     uint16_t   inputLen,
                     uint8_t*   pAuthData,
                     uint16_t   authDataLen,
                     uint8_t*   pNonce,
                     uint8_t    nonceSize,
                     uint8_t*   pKey,
                     uint8_t*   pOutput,
                     uint8_t*   pCbcMac,
                     uint8_t    macSize,
                     uint32_t   flags);

/*! *********************************************************************************
* \brief  This function allocates a memory buffer for a SHA1 context structure
*
* \return    Address of the SHA1 context buffer
*            Deallocate using SHA1_FreeCtx()
*
********************************************************************************** */
void* SHA1_AllocCtx (void);

/*! *********************************************************************************
* \brief  This function deallocates the memory buffer for the SHA1 context structure
*
* \param [in]    pContext    Address of the SHA1 context buffer
*
********************************************************************************** */
void SHA1_FreeCtx (void* pContext);

/*! *********************************************************************************
* \brief  This function clones a SHA1 context.
*         Make sure the size of the allocated destination context buffer is appropriate.
*
* \param [in]    pDestCtx    Address of the destination SHA1 context
* \param [in]    pSourceCtx  Address of the source SHA1 context
*
********************************************************************************** */
void SHA1_CloneCtx (void* pDestCtx, void* pSourceCtx);

/*! *********************************************************************************
* \brief  This function initializes the SHA1 context data
*
* \param [in]    pContext    Pointer to the SHA1 context data
*                            Allocated using SHA1_AllocCtx()
*
********************************************************************************** */
void SHA1_Init (void* pContext);

/*! *********************************************************************************
* \brief  This function performs SHA1 on multiple bytes and updates the context data
*
* \param [in]    pContext    Pointer to the SHA1 context data
*                            Allocated using SHA1_AllocCtx()
* \param [in]    pData       Pointer to the input data
* \param [in]    numBytes    Number of bytes to hash
*
********************************************************************************** */
void SHA1_HashUpdate (void*     pContext, 
                      uint8_t*  pData, 
                      uint32_t  numBytes);

/*! *********************************************************************************
* \brief  This function finalizes the SHA1 hash computaion and clears the context data.
*         The final hash value is stored at the provided output location.
*
* \param [in]       pContext    Pointer to the SHA1 context data
*                               Allocated using SHA1_AllocCtx()
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void SHA1_HashFinish (void*     pContext, 
                      uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function performs all SHA1 steps on multiple bytes: initialize, 
*         update and finish.
*         The final hash value is stored at the provided output location.
*
* \param [in]       pData       Pointer to the input data
* \param [in]       numBytes    Number of bytes to hash
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void SHA1_Hash (uint8_t*    pData, 
                uint32_t    numBytes, 
                uint8_t*    pOutput);
                    
/*! *********************************************************************************
* \brief  This function allocates a memory buffer for a SHA256 context structure
*
* \return    Address of the SHA256 context buffer
*            Deallocate using SHA256_FreeCtx()
*
********************************************************************************** */
void* SHA256_AllocCtx (void);

/*! *********************************************************************************
* \brief  This function deallocates the memory buffer for the SHA256 context structure
*
* \param [in]    pContext    Address of the SHA256 context buffer
*
********************************************************************************** */
void SHA256_FreeCtx (void* pContext);

/*! *********************************************************************************
* \brief  This function clones a SHA256 context.
*         Make sure the size of the allocated destination context buffer is appropriate.
*
* \param [in]    pDestCtx    Address of the destination SHA256 context
* \param [in]    pSourceCtx  Address of the source SHA256 context
*
********************************************************************************** */
void SHA256_CloneCtx (void* pDestCtx, void* pSourceCtx);

/*! *********************************************************************************
* \brief  This function initializes the SHA256 context data
*
* \param [in]    pContext    Pointer to the SHA256 context data
*                            Allocated using SHA256_AllocCtx()
*
********************************************************************************** */
void SHA256_Init (void* pContext);

/*! *********************************************************************************
* \brief  This function performs SHA256 on multiple bytes and updates the context data
*
* \param [in]    pContext    Pointer to the SHA256 context data
*                            Allocated using SHA256_AllocCtx()
* \param [in]    pData       Pointer to the input data
* \param [in]    numBytes    Number of bytes to hash
*
********************************************************************************** */
void SHA256_HashUpdate (void*       pContext, 
                        uint8_t*    pData, 
                        uint32_t    numBytes);

/*! *********************************************************************************
* \brief  This function finalizes the SHA256 hash computaion and clears the context data.
*         The final hash value is stored at the provided output location.
*
* \param [in]       pContext    Pointer to the SHA256 context data
*                               Allocated using SHA256_AllocCtx()
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void SHA256_HashFinish (void*       pContext, 
                        uint8_t*    pOutput);

/*! *********************************************************************************
* \brief  This function performs all SHA256 steps on multiple bytes: initialize, 
*         update and finish.
*         The final hash value is stored at the provided output location.
*
* \param [in]       pData       Pointer to the input data
* \param [in]       numBytes    Number of bytes to hash
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void SHA256_Hash (uint8_t*  pData, 
                  uint32_t  numBytes, 
                  uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function allocates a memory buffer for a HMAC SHA256 context structure
*
* \return    Address of the HMAC SHA256 context buffer
*            Deallocate using HMAC_SHA256_FreeCtx()
*
********************************************************************************** */
void* HMAC_SHA256_AllocCtx (void);

/*! *********************************************************************************
* \brief  This function deallocates the memory buffer for the HMAC SHA256 context structure
*
* \param [in]    pContext    Address of the HMAC SHA256 context buffer
*
********************************************************************************** */
void HMAC_SHA256_FreeCtx (void* pContext);

/*! *********************************************************************************
* \brief  This function performs the initialization of the HMAC SHA256 context data
*
* \param [in]    pContext    Pointer to the HMAC SHA256 context data
*                            Allocated using HMAC_SHA256_AllocCtx()
* \param [in]    pKey        Pointer to the HMAC key
* \param [in]    keyLen      Length of the HMAC key in bytes
*
********************************************************************************** */
void HMAC_SHA256_Init (void*    pContext,
                       uint8_t* pKey,
                       uint32_t keyLen);

/*! *********************************************************************************
* \brief  This function performs HMAC update with the input data.
*
* \param [in]    pContext    Pointer to the HMAC SHA256 context data
*                            Allocated using HMAC_SHA256_AllocCtx()
* \param [in]    pData       Pointer to the input data
* \param [in]    numBytes    Number of bytes to hash
*
********************************************************************************** */
void HMAC_SHA256_Update (void*      pContext,
                         uint8_t*   pData,
                         uint32_t   numBytes);

/*! *********************************************************************************
* \brief  This function finalizes the HMAC SHA256 computaion and clears the context data.
*         The final hash value is stored at the provided output location.
*
* \param [in]       pContext    Pointer to the HMAC SHA256 context data
*                               Allocated using HMAC_SHA256_AllocCtx()
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void HMAC_SHA256_Finish (void*      pContext,
                         uint8_t*   pOutput);

/*! *********************************************************************************
* \brief  This function performs all HMAC SHA256 steps on multiple bytes: initialize, 
*         update, finish, and update context data.
*         The final HMAC value is stored at the provided output location.
*
* \param [in]       pKey        Pointer to the HMAC key
* \param [in]       keyLen      Length of the HMAC key in bytes
* \param [in]       pData       Pointer to the input data
* \param [in]       numBytes    Number of bytes to perform HMAC on
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void HMAC_SHA256 (uint8_t*  pKey,
                  uint32_t  keyLen,
                  uint8_t*  pData,
                  uint32_t  numBytes,
                  uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function calculates XOR of individual byte pairs in two uint8_t arrays.
*         pDst[i] := pDst[i] ^ pSrc[i] for i=0 to n-1
*
* \param[in, out]  pDst First byte array operand for XOR and destination byte array
*
* \param[in]  pSrc Second byte array operand for XOR
*
* \param[in]  n  Length of the byte arrays which will be XORed
*
********************************************************************************** */
void SecLib_XorN (uint8_t*  pDst,   /* First operand and result of XOR operation */
                  uint8_t*  pSrc,   /* Second operand. Not modified. */
                  uint8_t   n);     /* Number of bytes in input arrays. */


/************************************************************************************
* \brief Generates a new ECDH P256 Private/Public key pair
*
* \return gSecSuccess_c or error
* 
************************************************************************************/
secResultType_t ECDH_P256_GenerateKeys
(
    ecdhPublicKey_t*    pOutPublicKey,
	ecdhPrivateKey_t*   pOutPrivateKey
);

/************************************************************************************
* \brief Computes the Diffie-Hellman Key for an ECDH P256 key pair.
*
* \return gSecSuccess_c or error
* 
************************************************************************************/
secResultType_t ECDH_P256_ComputeDhKey
(
    ecdhPrivateKey_t*   pInPrivateKey,
    ecdhPublicKey_t*    pInPeerPublicKey,
    ecdhDhKey_t*        pOutDhKey
);

#endif
