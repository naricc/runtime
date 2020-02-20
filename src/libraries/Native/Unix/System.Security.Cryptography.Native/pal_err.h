// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pal_compiler.h"
#include <stdint.h>
#include "opensslshim.h"

/*
Shims the ERR_clear_error method.
*/
PALEXPORT void CryptoNative_ErrClearError(void);

/*
Shim to ERR_get_error which also returns whether the error
was caused by an allocation failure.
*/
PALEXPORT uint64_t CryptoNative_ErrGetErrorAlloc(int32_t* isAllocFailure);

PALEXPORT uint64_t CryptoNative_ErrPeekError(void);

PALEXPORT uint64_t CryptoNative_ErrPeekLastError(void);

/*
Shims the ERR_reason_error_string method.

Returns the string for the specified error.
*/
PALEXPORT const char* CryptoNative_ErrReasonErrorString(uint64_t error);

/*
Direct shim to ERR_error_string_n.
*/
PALEXPORT void CryptoNative_ErrErrorStringN(uint64_t e, char* buf, int32_t len);
