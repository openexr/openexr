# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) Contributors to the OpenEXR Project.

# From https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
#
# - If the library source code has changed at all since the last
#   update, then increment revision (‘c.r.a’ becomes ‘c.r+1.a’).
#
# - If any interfaces have been added, removed, or changed since the
#   last update, increment current, and set revision to 0.
#
# - If any interfaces have been added since the last public release,
#   then increment age.
#
# - If any interfaces have been removed or changed since the last
#   public release, then set age to 0.

set(OPENEXR_VERSION_MAJOR 2)
set(OPENEXR_VERSION_MINOR 5)
set(OPENEXR_VERSION_PATCH 99)

set(OPENEXR_VERSION_EXTRA "" CACHE STRING "Extra version tag string for OpenEXR build")
if (OPENEXR_VERSION_EXTRA STREQUAL "")
  set(OPENEXR_VERSION "${OPENEXR_VERSION_MAJOR}.${OPENEXR_VERSION_MINOR}.${OPENEXR_VERSION_PATCH}")
else()
  set(OPENEXR_VERSION "${OPENEXR_VERSION_MAJOR}.${OPENEXR_VERSION_MINOR}.${OPENEXR_VERSION_PATCH}-${OPENEXR_VERSION_EXTRA}")
endif()

set(OPENEXR_VERSION_API   ${OPENEXR_VERSION_MAJOR}_${OPENEXR_VERSION_MINOR})
set(OPENEXR_SOCURRENT 26)
set(OPENEXR_SOREVISION 1)
set(OPENEXR_SOAGE 0)
math(EXPR OPENEXR_SOVERSION "${OPENEXR_SOCURRENT} - ${OPENEXR_SOAGE}")
set(OPENEXR_LIB_VERSION "${OPENEXR_SOVERSION}.${OPENEXR_SOAGE}.${OPENEXR_SOREVISION}")
message(STATUS "Configure OpenEXR Version: ${OPENEXR_VERSION} Lib API: ${OPENEXR_LIB_VERSION}")
unset(CONFIGURE_AC_CONTENTS)
