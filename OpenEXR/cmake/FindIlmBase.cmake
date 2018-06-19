#-*-cmake-*-
#
# yue.nicholas@gmail.com
#
# This auxiliary CMake file helps in find the ILMBASE headers and libraries
#
# ILMBASE_FOUND                  set if ILMBASE is found.
# ILMBASE_INCLUDE_DIR            ILMBASE's include directory
# Ilmbase_HALF_LIBRARY           ILMBASE's Half libraries
# Ilmbase_IEX_LIBRARY            ILMBASE's Iex libraries
# Ilmbase_IEXMATH_LIBRARY        ILMBASE's IexMath libraries
# Ilmbase_ILMTHREAD_LIBRARY      ILMBASE's IlmThread libraries
# Ilmbase_IMATH_LIBRARY          ILMBASE's Imath libraries

FIND_PATH ( ILMBASE_LOCATION include/OpenEXR/IlmBaseConfig.h
  "${ILMBASE_PACKAGE_PREFIX}"
  NO_DEFAULT_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  )

# MESSAGE ( "ILMBASE_ROOT = " $ENV{ILMBASE_ROOT} )
# MESSAGE ( "ILMBASE_LOCATION = " ${ILMBASE_LOCATION} )

IF ( ILMBASE_LOCATION )
  
  MESSAGE ( STATUS "Found IlmBase " ${ILMBASE_LOCATION} )
  SET ( ILMBASE_INCLUDE_DIRS
    ${ILMBASE_LOCATION}/include
    ${ILMBASE_LOCATION}/include/OpenEXR
    CACHE STRING "ILMBase include directories")
  SET ( ILMBASE_LIBRARY_DIRS ${ILMBASE_LOCATION}/lib
    CACHE STRING "ILMBase library directories")
  SET ( ILMBASE_FOUND TRUE )
  
  SET ( ORIGINAL_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
  IF (NOT BUILD_SHARED_LIBS)
    IF (APPLE)
      SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
      FIND_LIBRARY ( Ilmbase_HALF_LIBRARY Half PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IEX_LIBRARY Iex PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_ILMTHREAD_LIBRARY IlmThread PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IMATH_LIBRARY Imath PATHS ${ILMBASE_LOCATION}/lib )
    ELSEIF (WIN32)
      # Link library
      SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

      FIND_LIBRARY ( Ilmbase_HALF_LIBRARY_DEBUG          Half_debug      PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IEX_LIBRARY_DEBUG           Iex_debug       PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IEXMATH_LIBRARY_DEBUG       IexMath_debug   PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_ILMTHREAD_LIBRARY_DEBUG     IlmThread_debug PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IMATH_LIBRARY_DEBUG         Imath_debug     PATHS ${ILMBASE_LOCATION}/lib )

      FIND_LIBRARY ( Ilmbase_HALF_LIBRARY_OPTIMIZED      Half            PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IEX_LIBRARY_OPTIMIZED       Iex             PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IEXMATH_LIBRARY_OPTIMIZED   IexMath         PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_ILMTHREAD_LIBRARY_OPTIMIZED IlmThread       PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IMATH_LIBRARY_OPTIMIZED     Imath           PATHS ${ILMBASE_LOCATION}/lib )

      SET ( Ilmbase_HALF_LIBRARY
            optimized ${Ilmbase_HALF_LIBRARY_OPTIMIZED}
        debug ${Ilmbase_HALF_LIBRARY_DEBUG}
        CACHE STRING "ILMBase Half Library"
        )
      SET ( Ilmbase_IEX_LIBRARY
            optimized ${Ilmbase_IEX_LIBRARY_OPTIMIZED}
        debug ${Ilmbase_IEX_LIBRARY_DEBUG}
        CACHE STRING "ILMBase Iex Library"
        )
      SET ( Ilmbase_IEXMATH_LIBRARY
            optimized ${Ilmbase_IEXMATH_LIBRARY_OPTIMIZED}
        debug ${Ilmbase_IEXMATH_LIBRARY_DEBUG}
        CACHE STRING "ILMBase IexMath Library")
      SET ( Ilmbase_ILMTHREAD_LIBRARY
            optimized ${Ilmbase_ILMTHREAD_LIBRARY_OPTIMIZED}
        debug ${Ilmbase_ILMTHREAD_LIBRARY_DEBUG}
        CACHE STRING "ILMBase IlmThread Library")
      SET ( Ilmbase_IMATH_LIBRARY
            optimized ${Ilmbase_IMATH_LIBRARY_OPTIMIZED}
        debug ${Ilmbase_IMATH_LIBRARY_DEBUG}
        CACHE STRING "ILMBase Imath Library")

    ELSE (APPLE)
      SET ( CMAKE_FIND_LIBRARY_SUFFIXES ".a")
      FIND_LIBRARY ( Ilmbase_HALF_LIBRARY Half PATHS ${ILMBASE_LOCATION}/lib
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
      FIND_LIBRARY ( Ilmbase_IEX_LIBRARY Iex PATHS ${ILMBASE_LOCATION}/lib
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
      FIND_LIBRARY ( Ilmbase_ILMTHREAD_LIBRARY IlmThread PATHS ${ILMBASE_LOCATION}/lib
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
      FIND_LIBRARY ( Ilmbase_IMATH_LIBRARY Imath PATHS ${ILMBASE_LOCATION}/lib
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
    ENDIF (APPLE)
  ELSE (Ilmbase_USE_STATIC_LIBS)
    IF (APPLE)
      SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
      FIND_LIBRARY ( Ilmbase_HALF_LIBRARY Half PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IEX_LIBRARY Iex PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_ILMTHREAD_LIBRARY IlmThread PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IMATH_LIBRARY Imath PATHS ${ILMBASE_LOCATION}/lib )
    ELSEIF (WIN32)
      # Link library
      SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
      FIND_LIBRARY ( Ilmbase_HALF_LIBRARY Half PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IEX_LIBRARY Iex PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_ILMTHREAD_LIBRARY IlmThread PATHS ${ILMBASE_LOCATION}/lib )
      FIND_LIBRARY ( Ilmbase_IMATH_LIBRARY Imath PATHS ${ILMBASE_LOCATION}/lib )
      # Load library
      SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
      FIND_LIBRARY ( Ilmbase_HALF_DLL Half PATHS ${ILMBASE_LOCATION}/bin
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
      FIND_LIBRARY ( Ilmbase_IEX_DLL Iex PATHS ${ILMBASE_LOCATION}/bin
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
      FIND_LIBRARY ( Ilmbase_ILMTHREAD_DLL IlmThread PATHS ${ILMBASE_LOCATION}/bin
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
      FIND_LIBRARY ( Ilmbase_IMATH_DLL Imath PATHS ${ILMBASE_LOCATION}/bin
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
    ELSE (APPLE)
      FIND_LIBRARY ( Ilmbase_HALF_LIBRARY Half PATHS ${ILMBASE_LOCATION}/lib
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
      FIND_LIBRARY ( Ilmbase_IEX_LIBRARY Iex PATHS ${ILMBASE_LOCATION}/lib
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
      FIND_LIBRARY ( Ilmbase_ILMTHREAD_LIBRARY IlmThread PATHS ${ILMBASE_LOCATION}/lib
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
      FIND_LIBRARY ( Ilmbase_IMATH_LIBRARY Imath PATHS ${ILMBASE_LOCATION}/lib
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        )
    ENDIF (APPLE)
  ENDIF ()

  # MUST reset
  SET(CMAKE_FIND_LIBRARY_SUFFIXES ${ORIGINAL_CMAKE_FIND_LIBRARY_SUFFIXES})
  
ENDIF ( ILMBASE_LOCATION )
