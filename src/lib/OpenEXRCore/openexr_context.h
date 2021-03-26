/*
** SPDX-License-Identifier: BSD-3-Clause
** Copyright Contributors to the OpenEXR Project.
*/

#ifndef OPENEXR_CONTEXT_H
#define OPENEXR_CONTEXT_H

#include "openexr_errors.h"

#include "openexr_base.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @defgroup Context Context related definitions
 *
 * A context is a single instance of an OpenEXR file or stream. Beyond
 * a particular file or stream handle, it also has separate controls
 * for error handling and memory allocation. This is done to enable
 * encoding or decoding on mixed hardware.
 *
 * @{
 */

/** Opaque context handle
 *
 * The implementation of this is partly opaque to provide better
 * version portability, and all accesses to relevant data should
 * happen using provided functions. This handle serves as a container
 * and identifier for all the metadata and parts associated with a
 * file and/or stream
 */

typedef struct _priv_exr_context_t* exr_context_t;

/** 
 * @defgroup ContextFunctions OpenEXR Context Stream / File Functions
 *
 * @brief These are a group of function interfaces used to customize
 * the error handling, memory allocations, or I/O behavior of an
 * OpenEXR context.
 *
 * @{
 */

/** @brief Stream error notifier
 *
 *  This function pointer is provided to the stream functions by the
 *  library such that they can provide a nice error message to the
 *  user during stream operations.
 */
typedef exr_result_t (*exr_stream_error_func_ptr_t) (
    const exr_context_t ctxt, exr_result_t code, const char* fmt, ...)
    EXR_PRINTF_FUNC_ATTRIBUTE;

/** @brief Error callback function
 *
 *  Because a file can be read from using many threads at once, it is
 *  difficult to store an error message for later retrieval. As such,
 *  when a file is constructed, a callback function can be provided
 *  which delivers an error message for the calling application to
 *  handle. This will then be delivered on the same thread causing the
 *  error.
 */
typedef void (*exr_error_handler_cb_t) (
    const exr_context_t ctxt, exr_result_t code, const char* msg);

/** Destroy custom stream function pointer
 *
 *  Generic callback to clean up user data for custom streams.
 *  This is called when the file is closed and expected not to
 *  error
 *
 *  @param failed - indicates the write operation failed, the
 *                  implementor may wish to cleanup temporary
 *                  files
 */
typedef void (*exr_destroy_stream_func_ptr_t) (
    const exr_context_t ctxt, void* userdata, int failed);

/** Query stream size function pointer
 *
 * Used to query the size of the file, or amount of data representing
 * the openexr file in the data stream.
 *
 * This is used to validate requests against the file. If the size is
 * unavailable, return -1, which will disable these validation steps
 * for this file, although appropriate memory safeguards must be in
 * place in the calling application.
 */
typedef int64_t (*exr_query_size_func_ptr_t) (
    const exr_context_t ctxt, void* userdata);

/** @brief Read custom function pointer
 *
 * Used to read data from a custom output. Expects similar semantics to
 * pread or ReadFile with overlapped data under win32
 *
 * It is required that this provides thread-safe concurrent access to
 * the same file. If the stream / input layer you are providing does
 * not have this guarantee, your are responsible for providing
 * appropriate serialization of requests.
 *
 * A file should be expected to be accessed in the following pattern:
 *  - upon open, the header and part information attributes will be read
 *  - upon the first image read request, the offset tables will be read
 *    multiple threads accessing this concurrently may actually read
 *    these values at the same time
 *  - chunks can then be read in any order as preferred by the
 *    application
 *
 * While this should mean that the header will be read in 'stream'
 * order (no seeks required), no guarantee is made beyond that to
 * retrieve image / deep data in order. So if the backing file is
 * truly a stream, it is up to the provider to implement appropriate
 * caching of data to give the appearance of being able to seek / read
 * atomically.
 */
typedef int64_t (*exr_read_func_ptr_t) (
    const exr_context_t         ctxt,
    void*                       userdata,
    void*                       buffer,
    uint64_t                    sz,
    uint64_t                    offset,
    exr_stream_error_func_ptr_t error_cb);

/** Write custom function pointer
 *
 *  Used to write data to a custom output. Expects similar semantics to
 *  pwrite or WriteFile with overlapped data under win32
 *
 *  It is required that this provides thread-safe concurrent access to
 *  the same file. While it is unlikely that multiple threads will
 *  be used to write data for compressed forms, it is possible.
 *
 *  A file should be expected to be accessed in the following pattern:
 *  - upon open, the header and part information attributes is constructed
 *
 *  - when the write_header routine is called, the header becomes immutable
 *    and is written to the file. This computes the space to store the chunk
 *    offsets, but does not yet write the values
 *
 *  - Image chunks are written to the file, and appear in the order
 *    they are written, not in the ordering that is required by the
 *    chunk offset table (unless written in that order). This may vary
 *    slightly if the size of the chunks is not directly known and
 *    tight packing of data is necessary
 *
 *  - at file close, the chunk offset tables are written to the file
 */
typedef int64_t (*exr_write_func_ptr_t) (
    const exr_context_t         ctxt,
    void*                       userdata,
    const void*                 buffer,
    uint64_t                    sz,
    uint64_t                    offset,
    exr_stream_error_func_ptr_t error_cb);

/** @brief struct used to pass function pointers into the context
 * initialization routines.
 *
 * This partly exists to avoid the chicken and egg issue around creating the storage needed for the context on systems which want to override the malloc / free routines.
 *
 * However, it also serves to make a tidier / simpler set of functions
 * to create and start processing exr files.
 *
 * The size member is required for version portability
 *
 * It can be initialized using @sa EXR_DEFAULT_CONTEXT_INITIALIZER
 *
 * \code{.c}
 * exr_context_initializer_t myctxtinit = DEFAULT_CONTEXT_INITIALIZER;
 * myctxtinit.error_cb = &my_super_cool_error_callback_function;
 * ...
 * \endcode
 *
 */
typedef struct _exr_context_initializer
{
    /** @brief size member to tag initializer for version stability.
     *
     * This should be initialized to the size of the current
     * structure. This allows EXR to add functions or other
     * initializers in the future, and retain version compatibility
     */
    size_t size;

    /** @brief Error callback function pointer
     *
     * The error callback is allowed to be null, and will use a default print which outputs to stderr
     *
     * @sa exr_error_handler_cb_t
     */
    exr_error_handler_cb_t error_handler_fn;

    /** custom allocator, if null, will use malloc. @sa exr_memory_allocation_func_t */
    exr_memory_allocation_func_t alloc_fn;
    /** custom deallocator, if null, will use free. @sa exr_memory_free_func_t */
    exr_memory_free_func_t free_fn;

    /** passed to custom read, size, write, destroy functions below. Up to user to manage this pointer */
    void* user_data;

    /** @brief custom read routine.
     *
     * This is only used during read or update contexts. If this is
     * provided, it is expected that the caller has previously made
     * the stream available, and placed whatever stream / file data
     * into @sa user_data above.
     *
     * If this is NULL, and the context requested is for reading an
     * exr file, an internal implementation is provided for reading
     * from normal filesystem files, and the filename provided is
     * attempted to be opened as such.
     *
     * Expected to be NULL for a write-only operation, but is ignored
     * if it is provided.
     *
     * For update contexts, both read and write functions must be
     * provided if either is.
     *
     * @sa exr_read_func_ptr_t
     */
    exr_read_func_ptr_t read_fn;

    /** @brief custom size query routine.
     *
     * Used to provide validation when reading header values. If this
     * is not provided, but a custom read routine is provided, this
     * will disable some of the validation checks when parsing the
     * image header.
     *
     * Expected to be NULL for a write-only operation, but is ignored
     * if it is provided.
     *
     * @sa exr_query_size_func_ptr_t
     */
    exr_query_size_func_ptr_t size_fn;

    /** @brief custom write routine.
     *
     * This is only used during write or update contexts. If this is
     * provided, it is expected that the caller has previously made
     * the stream available, and placed whatever stream / file data
     * into @sa user_data above.
     *
     * If this is NULL, and the context requested is for writing an
     * exr file, an internal implementation is provided for reading
     * from normal filesystem files, and the filename provided is
     * attempted to be opened as such.
     *
     * For update contexts, both read and write functions must be
     * provided if either is.
     *
     * @sa exr_write_func_ptr_t
     */
    exr_write_func_ptr_t write_fn;

    /** @brief optional function to destroy the user data block of a custom stream
     *
     * Allows one to free any user allocated data, and close any handles.
     *
     * @sa exr_destroy_stream_func_ptr_t
     * */
    exr_destroy_stream_func_ptr_t destroy_fn;

    /** initializes a field specifying what the maximum image width
     * allowed by the context is. @sa exr_set_maximum_image_size to
     * understand how this interacts with global defaults */
    int max_image_width;
    /** initializes a field specifying what the maximum image height
     * allowed by the context is. @sa exr_set_maximum_image_size to
     * understand how this interacts with global defaults */
    int max_image_height;
    /** initializes a field specifying what the maximum tile width
     * allowed by the context is. @sa exr_set_maximum_tile_size to
     * understand how this interacts with global defaults */
    int max_tile_width;
    /** initializes a field specifying what the maximum tile height
     * allowed by the context is. @sa exr_set_maximum_tile_size to
     * understand how this interacts with global defaults */
    int max_tile_height;
} exr_context_initializer_t;

/** @brief simple macro to initialize the context initializer with default values */
#define EXR_DEFAULT_CONTEXT_INITIALIZER                                        \
    {                                                                          \
        sizeof (exr_context_initializer_t), 0                                    \
    }

/** @} */ /* context function pointer declarations */

/** @brief Closes and frees any internally allocated memory,
 * calling any provided destroy function for custom streams
 *
 * If the file was opened for write, will first save the chunk offsets
 * or any other unwritten data.
 */
EXR_EXPORT exr_result_t exr_finish (exr_context_t* ctxt);

/** @brief Creates and initializes a read-only exr read context.
 *
 * If a custom read function is provided, the filename is for
 * informational purposes only, the system assumes the user has
 * previously opened a stream, file, or whatever and placed relevant
 * data in userdata to access that.
 *
 * One notable attribute of the context is that once it has been
 * created and returned a successful code, it has parsed all the
 * header data. This is done as one step such that it is easier to
 * provide a safe context for multiple threads to request data from
 * the same context concurrently.
 *
 * Once finished reading data, use @sa exr_context_finish to clean up
 * the context.
 *
 * If you have custom I/O requirements, see the initializer context
 * documentation @sa exr_context_initializer_t. The ctxtdata parameter
 * is optional, if NULL, default values will be used.
 */
EXR_EXPORT exr_result_t exr_start_read (
    exr_context_t*                   ctxt,
    const char*                      filename,
    const exr_context_initializer_t* ctxtdata);

/** @brief enum describing how default files are handled during write */
enum exr_default_write_mode
{
    EXR_WRITE_FILE_DIRECTLY =
        0, /**< overwrites filename provided directly, deleted upon error */
    EXR_INTERMEDIATE_TEMP_FILE =
        1 /**< creates a temporary file, renaming it upon successful write, leaving original upon error */
};

/** @brief Creates and initializes a write-only context. 
 *
 * If a custom write function is provided, the filename is for
 * informational purposes only, and the default_mode parameter will be
 * ignored. As such, the system assumes the user has previously opened
 * a stream, file, or whatever and placed relevant data in userdata to
 * access that.
 *
 * Multi-Threading: To avoid issues with creating multi-part EXR
 * files, the library approaches writing as a multi-step process, so
 * the same concurrent guarantees can not be made for writing a
 * file. The steps are:
 *
 * 1. Context creation (this function)
 *
 * 2. Part definition (required attributes and additional metadata)
 *
 * 3. Transition to writing data (this "commits" the part definitions,
 * any changes requested after will result in an error)
 *
 * 4. Write part data in sequential order of parts ( part 0->(N-1) ).
 *
 * 5. Within each part, multiple threads can be encoding and writing
 * data concurrently. For some EXR part definitions, this may be able
 * to write data concurrently when it can predict the chunk sizes, or
 * data is allowed to be padded. For others, it may need to
 * temporarily cache chunks until the data is received to flush in
 * order. The concurrency around this is handled by the library
 *
 * 6. Once finished writing data, use @sa exr_context_finish to clean
 * up the context, which will flush any unwritten data such as the
 * final chunk offset tables, and handle the temporary file flags.
 *
 * If you have custom I/O requirements, see the initializer context
 * documentation @sa exr_context_initializer_t. The ctxtdata parameter
 * is optional, if NULL, default values will be used.
 */
EXR_EXPORT exr_result_t exr_start_write (
    exr_context_t*                 ctxt,
    const char*                    filename,
    enum exr_default_write_mode    default_mode,
    const exr_context_initializer_t* ctxtdata);

/** @brief Creates a new context for updating an exr file in place.
 *
 * This is a custom mode that allows one to modify the value of a
 * metadata entry, although not to change the size of the header, or
 * any of the image data.
 *
 * If you have custom I/O requirements, see the initializer context
 * documentation @sa exr_context_initializer_t. The ctxtdata parameter
 * is optional, if NULL, default values will be used.
 */
EXR_EXPORT exr_result_t exr_start_inplace_header_update (
    exr_context_t*                 ctxt,
    const char*                    filename,
    const exr_context_initializer_t* ctxtdata);

/** @brief retrieves the file name the context is for as provided
 * during the start routine.
 *
 * do not free the resulting string.
 */
EXR_EXPORT exr_result_t
exr_get_file_name (const exr_context_t ctxt, const char** name);

/** @} */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OPENEXR_CONTEXT_H */
