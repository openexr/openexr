// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.

#ifdef NDEBUG
#  undef NDEBUG
#endif

#include "read.h"

#include <openexr.h>

#include <assert.h>
#include <limits.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include <iostream>
#include <iomanip>
#include <memory>

static void err_cb( exr_context_t f, int code, const char *msg )
{
    std::cerr << "err_cb ERROR " << code << ": " << msg << std::endl;
}

int64_t dummyreadstream( exr_context_t f, void *, void *, uint64_t, uint64_t,
                         exr_stream_error_func_ptr_t errcb )
{
    return -1;
}

void testReadBadArgs( const std::string &tempdir )
{
    exr_context_t f;
    std::string fn = tempdir + "invalid.exr";
    exr_context_initializer_t cinit = EXR_DEFAULT_CONTEXT_INITIALIZER;
    cinit.error_handler_fn = &err_cb;

    assert(EXR_ERR_INVALID_ARGUMENT == exr_start_read( NULL, fn.c_str(), NULL ) );
    assert(EXR_ERR_INVALID_ARGUMENT == exr_start_read( &f, NULL, NULL ) );
    assert(EXR_ERR_INVALID_ARGUMENT == exr_start_read( &f, NULL, &cinit ) );

    assert(EXR_ERR_FILE_ACCESS == exr_start_read(
               &f, fn.c_str(), &cinit ) );
}

void testReadBadFiles( const std::string &tempdir )
{
    exr_context_t f;
    std::string fn = ILM_IMF_TEST_IMAGEDIR;
    fn += "invalid.exr";
    exr_result_t rv;
    exr_context_initializer_t cinit = EXR_DEFAULT_CONTEXT_INITIALIZER;
    cinit.error_handler_fn = &err_cb;

    rv = exr_start_read( &f, fn.c_str(), &cinit );
    assert( rv != EXR_ERR_SUCCESS );
}

void testOpenScans( const std::string &tempdir )
{
    exr_context_t f;
    std::string fn = ILM_IMF_TEST_IMAGEDIR;
    fn += "v1.7.test.1.exr";
    exr_result_t rv;
    exr_context_initializer_t cinit = EXR_DEFAULT_CONTEXT_INITIALIZER;
    cinit.error_handler_fn = &err_cb;

    rv = exr_start_read( &f, fn.c_str(), &cinit );
    assert( rv == EXR_ERR_SUCCESS );
    exr_finish( &f );

    fn = ILM_IMF_TEST_IMAGEDIR;
    fn += "v1.7.test.planar.exr";
    rv = exr_start_read( &f, fn.c_str(), &cinit );
    assert( rv == EXR_ERR_SUCCESS );
    exr_finish( &f );

    fn = ILM_IMF_TEST_IMAGEDIR;
    fn += "v1.7.test.interleaved.exr";
    rv = exr_start_read( &f, fn.c_str(), &cinit );
    assert( rv == EXR_ERR_SUCCESS );
    exr_finish( &f );
}

void testOpenTiles( const std::string &tempdir )
{
    exr_context_t f;
    std::string fn = ILM_IMF_TEST_IMAGEDIR;
    exr_context_initializer_t cinit = EXR_DEFAULT_CONTEXT_INITIALIZER;
    cinit.error_handler_fn = &err_cb;

    fn += "tiled.exr";
    exr_result_t rv;
    rv = exr_start_read( &f, fn.c_str(), &cinit );
    assert( rv == EXR_ERR_SUCCESS );
    exr_finish( &f );

    fn = ILM_IMF_TEST_IMAGEDIR;
    fn += "v1.7.test.tiled.exr";
    rv = exr_start_read( &f, fn.c_str(), &cinit );
    assert( rv == EXR_ERR_SUCCESS );
    exr_finish( &f );

}

void testOpenMultiPart( const std::string &tempdir )
{
}

void testOpenDeep( const std::string &tempdir )
{
}

void testReadScans( const std::string &tempdir )
{
}

void testReadTiles( const std::string &tempdir )
{
    exr_context_t f;
    std::string fn = ILM_IMF_TEST_IMAGEDIR;
    exr_context_initializer_t cinit = EXR_DEFAULT_CONTEXT_INITIALIZER;
    cinit.error_handler_fn = &err_cb;

    fn += "v1.7.test.tiled.exr";
    exr_result_t rv;
    rv = exr_start_read( &f, fn.c_str(), &cinit );
    assert( rv == EXR_ERR_SUCCESS );

    exr_storage_t ps;
    rv = exr_get_part_storage( f, 0, &ps );
    assert( rv == EXR_ERR_SUCCESS );
    assert( EXR_STORAGE_TILED == ps );

    int levelsx = -1, levelsy = -1;
    rv = exr_get_tile_levels( NULL, 0, &levelsx, &levelsy );
    assert( rv == -1 );
    assert( levelsx == -1 );
    assert( levelsy == -1 );

    rv = exr_get_tile_levels( f, 1, &levelsx, &levelsy );
    assert( rv == EXR_ERR_INVALID_ARGUMENT );
    assert( levelsx == -1 );
    assert( levelsy == -1 );

    rv = exr_get_tile_levels( f, 0, NULL, NULL );
    assert( rv == EXR_ERR_SUCCESS );

    levelsx = -1;
    rv = exr_get_tile_levels( f, 0, &levelsx, NULL );
    assert( rv == EXR_ERR_SUCCESS );
    assert( levelsx == 1 );

    levelsy = -1;
    rv = exr_get_tile_levels( f, 0, NULL, &levelsy );
    assert( rv == EXR_ERR_SUCCESS );
    assert( levelsy == 1 );

    levelsx = levelsy = -1;
    rv = exr_get_tile_levels( f, 0, &levelsx, &levelsy );
    assert( rv == EXR_ERR_SUCCESS );
    assert( levelsx == 1 );
    assert( levelsy == 1 );

    rv = exr_get_tile_sizes( f, 0, 0, 0, &levelsx, &levelsy );
    assert( rv == EXR_ERR_SUCCESS );
    assert( levelsx == 12 );
    assert( levelsy == 24 );

    exr_decode_chunk_info_t chunk = {0};
    rv = exr_decode_chunk_init_scanline( f, 0, &chunk, 42, 1 );
    assert( rv == EXR_ERR_SCAN_TILE_MIXEDAPI );

    // actually read a tile...
    rv = exr_decode_chunk_init_tile( f, 0, &chunk, 4, 2, 0, 0, 1 );
    assert( rv == EXR_ERR_SUCCESS );
    assert( chunk.own_scratch_buffers == 1 );
    assert( chunk.unpacked.size == exr_get_chunk_unpacked_size( f, 0 ) );
    assert( chunk.channel_count == 2 );
    assert( ! strcmp( chunk.channels[0].channel_name, "G" ) );
    assert( chunk.channels[0].bytes_per_pel == 2 );
    assert( chunk.channels[0].width == 12 );
    assert( chunk.channels[0].height == 24 );
    assert( chunk.channels[0].x_samples == 1 );
    assert( chunk.channels[0].y_samples == 1 );
    assert( ! strcmp( chunk.channels[1].channel_name, "Z" ) );
    assert( chunk.channels[1].bytes_per_pel == 4 );
    assert( chunk.channels[1].width == 12 );
    assert( chunk.channels[1].height == 24 );
    assert( chunk.channels[1].x_samples == 1 );
    assert( chunk.channels[1].y_samples == 1 );

    std::unique_ptr<uint8_t []> gptr 
        {
            new uint8_t[24*12*2]
        };
    std::unique_ptr<uint8_t []> zptr 
        {
            new uint8_t[24*12*4]
        };
    memset( gptr.get(), 0, 24*12*2 );
    memset( zptr.get(), 0, 24*12*4 );
    chunk.channels[0].data_ptr = gptr.get();
    chunk.channels[0].output_pixel_stride = 2;
    chunk.channels[0].output_line_stride = 2 * 12;
    chunk.channels[1].data_ptr = zptr.get();
    chunk.channels[1].output_pixel_stride = 4;
    chunk.channels[1].output_line_stride = 4 * 12;

    rv = exr_read_chunk( f, &chunk );
    assert( rv == EXR_ERR_SUCCESS );
    // it is compression: none
    assert( chunk.packed.buffer == NULL );
    // it is compression: none
    assert( chunk.unpacked.buffer == NULL );
    /* TODO: add actual comparison against C++ library */
    const uint16_t *curg = reinterpret_cast<const uint16_t *>( gptr.get() );
    const float *curz = reinterpret_cast<const float *>( zptr.get() );
    assert( *curg == 0x33d5 );
    assert( fabsf( *curz - 0.244778f ) < 0.000001f );
    //for ( int y = 0; y < 24; ++y )
    //{
    //    for ( int x = 0; x < 12; ++x )
    //        std::cout << ' ' << std::hex << std::setw( 4 ) << std::setfill( '0' ) << *curg++ << std::dec << " (" << *curz++ << " )";
    //    std::cout << std::endl;
    //}

    exr_destroy_decode_chunk_info( &chunk );
    exr_finish( &f );

    rv = exr_start_read( &f, "/home/kimball/Development/OSS/OpenEXR/kdt3rd/testmips.exr", &cinit );
    assert( rv == EXR_ERR_SUCCESS );

    exr_storage_t ps;
    rv = exr_get_part_storage( f, 0, &ps );
    assert( rv == EXR_ERR_SUCCESS );
    assert( EXR_STORAGE_TILED == ps );

    levelsx = -1;
    levelsy = -1;
    rv = exr_get_tile_levels( f, 0, &levelsx, &levelsy );
    assert( rv == 0 );
    assert( levelsx == 11 );
    assert( levelsy == 11 );
    
    rv = exr_get_tile_sizes( f, 0, 0, 0, &levelsx, &levelsy );
    assert( rv == EXR_ERR_SUCCESS );
    assert( levelsx == 32 );
    assert( levelsy == 32 );

    rv = exr_get_tile_sizes( f, 0, 10, 10, &levelsx, &levelsy );
    assert( rv == EXR_ERR_SUCCESS );
    assert( levelsx == 1 );
    assert( levelsy == 1 );

    rv = exr_decode_chunk_init_tile( f, 0, &chunk, 4, 2, 0, 0, 1 );

    assert( rv == EXR_ERR_SUCCESS );
    assert( chunk.unpacked.size == exr_get_chunk_unpacked_size( f, 0 ) );
    assert( chunk.channel_count == 3 );
    exr_destroy_decode_chunk_info( &chunk );

    rv = exr_decode_chunk_init_tile( f, 0, &chunk, 0, 0, 10, 10, 1 );
    assert( rv == EXR_ERR_SUCCESS );
    assert( chunk.unpacked.size == 1*1*2*3 );
    assert( chunk.channel_count == 3 );
    assert( chunk.width == 1 );
    assert( chunk.height == 1 );
    assert( chunk.channels[0].width == 1 );
    assert( chunk.channels[0].height == 1 );
    assert( chunk.channels[1].width == 1 );
    assert( chunk.channels[1].height == 1 );
    assert( chunk.channels[2].width == 1 );
    assert( chunk.channels[2].height == 1 );

    exr_destroy_decode_chunk_info( &chunk );
    exr_finish( &f );
}

void testReadMultiPart( const std::string &tempdir )
{
}

void testReadDeep( const std::string &tempdir )
{
}

