# OpenEXR

[![License](https://img.shields.io/badge/License-BSD%203%20Clause-blue.svg)](LICENSE.md)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/2799/badge)](https://bestpractices.coreinfrastructure.org/projects/2799)
[![Build Status](https://dev.azure.com/openexr/OpenEXR/_apis/build/status/openexr.openexr?branchName=master)](https://dev.azure.com/openexr/OpenEXR/_build/latest?definitionId=1&branchName=master)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=openexr_openexr&metric=alert_status)](https://sonarcloud.io/dashboard?id=openexr_openexr)

![openexr](/OpenEXR/doc/images/windowExample1.png)

The OpenEXR project provides the specification and reference
implementation of the EXR file format. The format is an accurate,
flexible, and efficient representation for high dynamic range linear
scene image data and associated metadata, with strong support for
multi-part, multi-channel use cases, including deep compositing. The
library includes routines for efficiently reading, writing,
compressing/decompressing, and modifying EXR images, as well as
reference images and example programs for examining and updating image
header attributes. The goal of the OpenEXR project is to keep the
format modern and to maintain its place as the preferred image
representation of the motion picture industry.

Also a part of OpenEXR, the IlmBase library is a simple, light-weight,
and efficient representation of 2D and 3D vectors and matrices and
other useful mathematical objects and functions and data types,
including the Half 16-bit floating-point type. With its associated
bindings for Python 2 and Python 3 and its extensive test suite, the
Imath and Half libraries provide a common representation for 2D and 3D
data across visual effects software applications and libraries.

OpenEXR's features include:

* High dynamic range and color precision.
* Support for 16-bit floating-point, 32-bit floating-point, and
  32-bit integer pixels.
* Multiple image compression algorithms, both lossless and lossy. Some of
  the included codecs can achieve 2:1 lossless compression ratios on images
  with film grain.  The lossy codecs have been tuned for visual quality and
  decoding performance.
* Extensibility. New compression codecs and image types can easily be added
  by extending the C++ classes included in the OpenEXR software distribution.
  New image attributes (strings, vectors, integers, etc.) can be added to
  OpenEXR image headers without affecting backward compatibility with
  existing OpenEXR applications. 
* Support for stereoscopic image workflows and a generalization
  to multi-views.
* Flexible support for deep data: pixels can store a variable-length list
  of samples and, thus, it is possible to store multiple values at different
  depths for each pixel. Hard surfaces and volumetric data representations
  are accommodated.
* Multipart: ability to encode separate, but related, images in one file.
  This allows for access to individual parts without the need to read other
  parts in the file.
* Versioning: OpenEXR source allows for user configurable C++
  namespaces to provide protection when using multiple versions of the
  library in the same process space.

OpenEXR is included in the [VFX Reference
Platform](https://vfxplatform.com).

OpenEXR is a project of the [Academy Software
Foundation](https://www.aswf.io).  It was originally developed by
Industrial Light & Magic (ILM) and first released in 2003.  Weta
Digital, Walt Disney Animation Studios, Sony Pictures Imageworks,
Pixar Animation Studios, DreamWorks, and other studios, companies, and
individuals have made contributions to the code base.

## OpenEXR Sub-Modules

The OpenEXR distribution consists of the following sub-modules:

* **IlmBase** - Utility libraries from Industrial Light & Magic: Half, Imath, Iex, IlmThread.
* **PyIlmBase** - Python bindings for the IlmBase libraries.
* **OpenEXR** - The core image library.
* **OpenEXR_Viewers** - Example code for image viewing programs.
* **Contrib** - Various plugins and utilities, contributed by the community.
    
A collection of OpenEXR images is available from the adjacent repository
https://github.com/openexr/openexr-images.

## Supported Platforms

OpenEXR builds on:

* Linux
* macOS
* Microsoft Visual Studio 2015 or newer

The Python bindings in PyIlmBase support Python 2 and Python 3.

## Developer Quick Start

See [INSTALL](INSTALL.md) for instructions on downloading and building OpenEXR
from source.

## Resources

* Website: http:://www.openexr.com

* GitHub repository: http://www.github.com/openexr/openexr

* Documentation: http://www.openexr.com/documentation.html.

### Getting Help

There are two primary ways to connect with the OpenEXR project:

* The openexr-dev@lists.aswf.io mail list: This is a development
  focused mail list with a deep history of technical conversations and
  decisions that have shaped the project. Subscribe at
  [openexr-dev@lists.aswf.io](https://lists.aswf.io/g/openexr-dev).

* GitHub Issues: GitHub issues are used both to track bugs and to
  discuss feature requests.

See [CONTRIBUTING](CONTRIBUTING.md) for more information.

### Getting Involved

OpenEXR welcomes contributions to the project. See
[CONTRIBUTING](CONTRIBUTING.md) for more information about
contributing to OpenEXR.

## License

OpenEXR is released under the [BSD-3-Clause](LICENSE) license.

See [PATENTS](OpenEXR/PATENTS) for license information about portions of OpenEXR that are provided under a different license.

## Project Goverance

OpenEXR is governed by the Academy Software Foundation. See
[GOVERNANCE](GOVERNANCE.md) for more infomation about how the project
operates.

## Frequently Asked Questions

* "``pip install openexr`` doesn't work."

  The OpenEXR project provides python bindings for the Imath
  vector/matrix classes, but it does *not* provide python bindings for
  reading, writing, or editing .exr files.  The
  [openexrpython](https://github.com/jamesbowman/openexrpython) module
  is not affiliated with the OpenEXR project or the ASWF. Please
  direct questions there.

  Alternatively,
  [OpenImageIO](https://sites.google.com/site/openimageio/home) also
  includes python bindings for OpenEXR.

---

![aswf](/ASWF/images/aswf.png)
