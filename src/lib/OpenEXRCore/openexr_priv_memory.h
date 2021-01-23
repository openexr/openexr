/*
** SPDX-License-Identifier: BSD-3-Clause
** Copyright Contributors to the OpenEXR Project.
*/

#ifndef OPENEXR_PRIVATE_MEMORY_H
#define OPENEXR_PRIVATE_MEMORY_H

#include "openexr_base.h"

#if defined(__GNUC__) || defined(__clang__)
__attribute__((malloc))
#endif
extern void *priv_alloc( size_t bytes );

extern void priv_free( void *ptr );

#endif /* OPENEXR_PRIVATE_MEMORY_H */
