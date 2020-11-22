/*
** SPDX-License-Identifier: BSD-3-Clause
** Copyright Contributors to the OpenEXR Project.
*/

#include <openexr.h>

#include <set>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#ifdef _WIN32
# include <windows.h>
#else
# include <unistd.h>
#endif

#include "base_units.h"
#include "general_attr.h"
#include "read.h"

#if defined(ANDROID) || defined(__ANDROID_API__)
    #define IMF_TMP_DIR "/sdcard/"
    #define IMF_PATH_SEPARATOR "/"
#elif defined(_WIN32) || defined(_WIN64)
    #define IMF_TMP_DIR ""  // TODO: get this from GetTempPath() or env var $TEMP or $TMP
    #define IMF_PATH_SEPARATOR "\\"
    #include <direct.h> // for _mkdir, _rmdir
    #define mkdir(name,mode) _mkdir(name)
    #define rmdir _rmdir
#else
    #include <sys/stat.h> // for mkdir
    #define IMF_TMP_DIR "/var/tmp/"
    #define IMF_PATH_SEPARATOR "/"
#endif

#define TEST_STRING(x) #x

#define TEST(x,y)                               \
    if (helpMode)                               \
    {                                           \
        tests.insert(std::string(TEST_STRING(x)));   \
        suites.insert(std::string(y));               \
    }                                                                   \
    else if (argc < 2 || (!strcmp (argv[1], TEST_STRING(x)) || !strcmp (argv[1], y))) \
    {                                                                   \
        std::cout << "\n=======\nRunning " << TEST_STRING(x) << std::endl; \
        x(tempDir);                                                     \
    }

std::string makeTempDir()
{
    std::string tempDir;

    while (true)
    {
#ifdef _WIN32
        IMATH_NAMESPACE::Rand48 rand48 (time ((time_t*)0) );
        char  tmpbuf[4096];
        DWORD len = GetTempPathA (4096, tmpbuf);
        if (len == 0 || len > 4095)
        {
            std::cerr << "Cannot retrieve temporary directory" << std::endl;
            exit(1);
        }
        tempDir = tmpbuf;
        // windows does this automatically
        // tempDir += IMF_PATH_SEPARATOR;
        tempDir += "IlmImfTest_";

        for (int i = 0; i < 8; ++i)
            tempDir += ('A' + rand48.nexti() % 26);

        int status = mkdir(tempDir.c_str(), 0777);

        if (errno != EEXIST)
        {
            std::cerr << "ERROR -- mkdir(" << tempDir << ") failed: "
                         "errno = " << errno << std::endl;
            exit(1);
        }
#else
        char tmpbuf[4096];
        strncpy( tmpbuf, IMF_TMP_DIR "OpenEXR_XXXXXX", 4095 );
        tmpbuf[4095] = '\0';
        char *tmpd = mkdtemp( tmpbuf );
        if ( tmpd )
        {
            tempDir = tmpd;
        }
        else
        {
            std::cerr << "ERROR: mkdtemp( \"" << tmpbuf << "\" failed: " << strerror(errno) << std::endl;
            exit(1);
        }
        int status = 0;
#endif
        std::cout << "tempDir = " << tempDir << std::endl;

        if (status == 0)
        {
            tempDir += IMF_PATH_SEPARATOR;
            break; // success
        }
    }

    return tempDir;
}

int
main (int argc, char *argv[])
{
    // Create temporary files in a uniquely named private temporary
    // subdirectory of IMF_TMP_DIR to avoid colliding with other
    // running instances of this program.

    std::string tempDir;

    bool helpMode = false;
    if( argc==2 && (strcmp(argv[1],"--help")==0 || strcmp(argv[1],"-h")==0))
    {
            helpMode = true;
    }
    std::set<std::string> tests;
    std::set<std::string> suites;


    if ( !helpMode )
    {
	    tempDir = makeTempDir();
    }

    TEST( testBase, "core" );
    TEST( testBaseErrors, "core" );
    TEST( testBaseLimits, "core" );
    TEST( testBaseDebug, "core" );

    TEST( testAttrSizes, "gen_attr" );
    TEST( testAttrStrings, "gen_attr" );
    TEST( testAttrStringVectors, "gen_attr" );
    TEST( testAttrFloatVectors, "gen_attr" );
    TEST( testAttrChlists, "gen_attr" );
    TEST( testAttrPreview, "gen_attr" );
    TEST( testAttrOpaque, "gen_attr" );
    TEST( testAttrHandler, "gen_attr" );
    TEST( testAttrLists, "gen_attr" );

    TEST( testReadBadArgs, "core_read" );
    TEST( testReadBadFiles, "core_read" );
    TEST( testOpenScans, "core_read" );
    TEST( testOpenTiles, "core_read" );
    TEST( testOpenMultiPart, "core_read" );
    TEST( testOpenDeep, "core_read" );
    TEST( testReadScans, "core_read" );
    TEST( testReadTiles, "core_read" );
    TEST( testReadMultiPart, "core_read" );
    TEST( testReadDeep, "core_read" );

    if ( helpMode )
    {
        std::cout << "OpenEXR Core Test runs a series of tests to confirm\n"
            "correct behavior of the core low-level OpenEXR library.\n"
            "If all is correct, OpenEXRCoreTest will complete without\n"
            "crashing or leaking memory.\n";
        std::cout << "\n";
        std::cout << "If a test fails, an individual test can be re-run, avoiding\n"
            "the wait for previous tests to complete. This allows easier debugging\n"
            "of the failure.\n";
        std::cout << "\n";
        std::cout << "A 'suite' of tests can also be run, to allow a subset of\n"
             << "tests to run. This is useful as an initial confirmation\n"
             << "that a modification to the library has not introduced an error.\n"
             << "Suites can be run in parallel for speed. Every test is in one suite.\n";
        std::cout << "\n";
        std::cout << "usage:\n"
             << "OpenEXRCoreTest           : with no arguments, run all tests\n"
             << "OpenEXRCoreTest TEST      : run only specific test, then quit\n"
             << "OpenEXRCoreTest SUITE     : run all the tests in the given SUITE\n";
        std::cout << "\n";
        std::cout << "available TESTs:\n";
        for ( auto i = tests.begin() ; i!= tests.end() ; ++i)
        {
            std::cout << ' ' << *i << std::endl;
        }
        std::cout << "\n";
        std::cout << "available SUITEs:\n";
        for ( auto i = suites.begin() ; i!= suites.end() ; ++i )
        {
            std::cout << ' ' << *i << std::endl;
        }
    } 
    else
    {
        std::cout << "removing temp dir " << tempDir << std::endl;
        rmdir (tempDir.c_str());

#ifdef OPENEXR_IMF_HAVE_LINUX_PROCFS

        //
        // Allow the user to check for file descriptor leaks
        //

        std::cout << "open file descriptors:" << std::endl;

        std::stringstream ss;
        ss << "ls -lG /proc/" << getpid() << "/fd";
            
        if (system (ss.str().c_str()) == -1)
        {
            std::cout << "failed to run ls\n";
        }

        std::cout << endl;

#endif
    }
    return 0;
}


