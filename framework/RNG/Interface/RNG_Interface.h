/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
* \file
*
* RNG implementation header file for the ARM CORTEX-M4 processor
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

#ifndef _RNG_INTERFACE_H_
#define _RNG_INTERFACE_H_

#include <stddef.h>

#include "EmbeddedTypes.h"


/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define gRngSuccess_d       (0x00)
#define gRngInternalError_d (0x01)
#define gRngNullPointer_d   (0x80)
#define gRngMaxRequests_d   (100000)


/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

/*! Generic PRNG function pointer type definition. */
typedef int (*fpRngPrng_t) (void *,
                            unsigned char *,
                            size_t);

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public function prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  Initialize the RNG Software Module
*         Please call SecLib_Init() befoare calling this function to make sure
*         RNG hardware is correctly initlaized.
*
* \return  Status of the RNG initialization procedure.
*
********************************************************************************** */
uint8_t RNG_Init (void);


/*! *********************************************************************************
* \brief  Generates a 32-bit statistically random number
*         No random number will be generated if the RNG was not initialized
*         or an error occurs.
*
* \param[out]  pRandomNo  Pointer to location where the value will be stored
*
********************************************************************************** */
void RNG_GetRandomNo (uint32_t* pRandomNo);


/*! *********************************************************************************
* \brief  Initialize seed for the PRNG algorithm.
*         If this function is called again, even with a NULL argument,
*         the PRNG will be reseeded.
*
* \param[in]  pSeed  Ignored - please set to NULL
*             This parameter is ignored because it is no longer needed.
*             The PRNG is automatically seeded from the true random source.
*
********************************************************************************** */
void RNG_SetPseudoRandomNoSeed (uint8_t* pSeed);


/*! *********************************************************************************
* \brief  Generates an 256 bit (or 160 bit) pseudo-random number. The PRNG algorithm used
*         depends on the platform's cryptographic hardware and software capabilities.
*         Please check the implementation and/or the output of this function at runtime
*         to see how many bytes does the PRNG produce (depending on the implementation).
*
* \param[out]  pOut  Pointer to the output buffer (max 32 bytes or max 20 bytes)
* \param[in]   outBytes  The number of bytes to be copyed (1-32 or 1-20 depending on the implementation)
* \param[in]   pSeed  Ignored - please set to NULL
*              This parameter is ignored because it is no longer needed.
*              The PRNG is automatically seeded from the true random source.
*              The length of the seed if present is 32 bytes or 20 bytes (depending on the implementation).
*
* \return  The number of bytes copied OR
*          -1 if reseed is needed OR
*          0 if he PRNG was not initialized or 0 bytes were requested or an error occurred
*
********************************************************************************** */
int16_t RNG_GetPseudoRandomNo (uint8_t* pOut,
                               uint8_t  outBytes,
                               uint8_t* pSeed);


/*! *********************************************************************************
* \brief  Returns a pointer to the general PRNG function
*         Call RNG_SetPseudoRandomNoSeed() before calling this function.
*
* \return  Function pointer to the general PRNG function or NULL if it
*          was not seeded.
*
********************************************************************************** */
fpRngPrng_t RNG_GetPrngFunc (void);

/*! *********************************************************************************
* \brief  Returns a pointer to the general PRNG context
*         Call RNG_SetPseudoRandomNoSeed() before calling this function.
*
* \return  Function pointer to the general PRNG context or NULL if it
*          was not initialized correctly.
*
********************************************************************************** */
void* RNG_GetPrngContext (void);

#endif /* _RNG_INTERFACE_H_ */