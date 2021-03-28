/*
** SPDX-License-Identifier: BSD-3-Clause
** Copyright Contributors to the OpenEXR Project.
*/

#include "internal_attr.h"

#include "internal_structs.h"

#include <string.h>

/**************************************/

exr_result_t
exr_attr_preview_init (
    exr_context_t ctxt, exr_attr_preview_t* p, uint32_t w, uint32_t h)
{
    exr_attr_preview_t nil   = { 0 };
    size_t             bytes = (size_t) w * (size_t) h * (size_t) 4;

    EXR_PROMOTE_CONTEXT_OR_ERROR (ctxt);

    if (bytes > (size_t) INT32_MAX)
        return pctxt->print_error (
            ctxt,
            EXR_ERR_INVALID_ARGUMENT,
            "Invalid very large size for preview image (%u x %u - %lu bytes)",
            w,
            h,
            bytes);

    if (!p)
        return pctxt->report_error (
            ctxt,
            EXR_ERR_INVALID_ARGUMENT,
            "Invalid reference to preview object to initialize");

    *p      = nil;
    p->rgba = (uint8_t*) pctxt->alloc_fn (bytes);
    if (p->rgba == NULL)
        return pctxt->standard_error (ctxt, EXR_ERR_OUT_OF_MEMORY);
    p->alloc_size = bytes;
    p->width      = w;
    p->height     = h;
    return EXR_ERR_SUCCESS;
}

/**************************************/

exr_result_t
exr_attr_preview_create (
    exr_context_t       ctxt,
    exr_attr_preview_t* p,
    uint32_t            w,
    uint32_t            h,
    const uint8_t*      d)
{
    exr_result_t rv = exr_attr_preview_init (ctxt, p, w, h);
    if (rv == EXR_ERR_SUCCESS)
    {
        size_t copybytes = w * h * 4;
        if (p->alloc_size >= copybytes)
            memcpy ((uint8_t*) p->rgba, d, copybytes);
        else
            rv = EXR_ERR_INVALID_ARGUMENT;
    }
    return rv;
}

/**************************************/

exr_result_t
exr_attr_preview_destroy (exr_context_t ctxt, exr_attr_preview_t* p)
{
    EXR_PROMOTE_CONTEXT_OR_ERROR (ctxt);

    if (p)
    {
        exr_attr_preview_t nil = { 0 };
        if (p->rgba && p->alloc_size > 0) pctxt->free_fn ((uint8_t*) p->rgba);
        *p = nil;
    }
    return EXR_ERR_SUCCESS;
}
