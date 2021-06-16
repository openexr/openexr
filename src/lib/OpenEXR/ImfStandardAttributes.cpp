//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) Contributors to the OpenEXR Project.
//

//-----------------------------------------------------------------------------
//
//	Optional Standard Attributes
//
//-----------------------------------------------------------------------------

#include <ImfStandardAttributes.h>


#if defined(_MSC_VER)
// suppress warning about non-exported base classes
#pragma warning (disable : 4251)
#pragma warning (disable : 4275)
#endif

#define IMF_STRING(name) #name

#define IMF_STD_ATTRIBUTE_IMP(name,suffix,type)				 \
									 \
    void								 \
    IMF_ADD_SUFFIX(suffix) (Header &header, const type &value)          \
    {									 \
	header.insert (IMF_STRING (name), TypedAttribute<type> (value)); \
    }									 \
									 \
    bool								 \
    IMF_HAS_SUFFIX(suffix) (const Header &header)                       \
    {									 \
	return header.findTypedAttribute <TypedAttribute <type> >	 \
		(IMF_STRING (name)) != 0;				 \
    }									 \
									 \
    const TypedAttribute<type> &					 \
    IMF_NAME_ATTRIBUTE(name) (const Header &header)                    \
    {									 \
	return header.typedAttribute <TypedAttribute <type> >		 \
		(IMF_STRING (name));					 \
    }									 \
									 \
    TypedAttribute<type> &						 \
    IMF_NAME_ATTRIBUTE(name) (Header &header)                      \
    {									 \
	return header.typedAttribute <TypedAttribute <type> >		 \
		(IMF_STRING (name));					 \
    }									 \
									 \
    const type &							 \
    name (const Header &header)						 \
    {									 \
	return IMF_NAME_ATTRIBUTE(name) (header).value();           \
    }									 \
									 \
    type &								 \
    name (Header &header)						 \
    {									 \
	return IMF_NAME_ATTRIBUTE(name) (header).value();           \
    }

#include "ImfNamespace.h"

using namespace IMATH_NAMESPACE;
using namespace std;

OPENEXR_IMF_INTERNAL_NAMESPACE_SOURCE_ENTER

IMF_STD_ATTRIBUTE_IMP (acesImageContainerFlag, AcesImageContainerFlag, int)
IMF_STD_ATTRIBUTE_IMP (originalImageFlag, OriginalImageFlag, int)
IMF_STD_ATTRIBUTE_IMP (chromaticities, Chromaticities, Chromaticities)
IMF_STD_ATTRIBUTE_IMP (whiteLuminance, WhiteLuminance, float)
IMF_STD_ATTRIBUTE_IMP (adoptedNeutral, AdoptedNeutral, V2f)
IMF_STD_ATTRIBUTE_IMP (owner, Owner, string)
IMF_STD_ATTRIBUTE_IMP (creator, Creator, std::string)
IMF_STD_ATTRIBUTE_IMP (comments, Comments, string)
IMF_STD_ATTRIBUTE_IMP (capDate, CapDate, string)
IMF_STD_ATTRIBUTE_IMP (utcOffset, UtcOffset, float)
IMF_STD_ATTRIBUTE_IMP (longitude, Longitude, float)
IMF_STD_ATTRIBUTE_IMP (latitude, Latitude, float)
IMF_STD_ATTRIBUTE_IMP (altitude, Altitude, float)
IMF_STD_ATTRIBUTE_IMP (cameraIdentifier, CameraIdentifier, std::string)
IMF_STD_ATTRIBUTE_IMP (cameraLabel, CameraLabel, std::string)
IMF_STD_ATTRIBUTE_IMP (cameraMake, CameraMake, std::string)
IMF_STD_ATTRIBUTE_IMP (cameraModel, CameraModel, std::string)
IMF_STD_ATTRIBUTE_IMP (cameraSerialNumber, CameraSerialNumber, std::string)
IMF_STD_ATTRIBUTE_IMP (cameraFirmwareVersion, CameraFirmwareVersion, std::string)
IMF_STD_ATTRIBUTE_IMP (isoSpeed, IsoSpeed, float)
IMF_STD_ATTRIBUTE_IMP (lensMake, LensMake, std::string)
IMF_STD_ATTRIBUTE_IMP (lensModel, LensModel, std::string)
IMF_STD_ATTRIBUTE_IMP (lensSerialNumber, LensSerialNumber, std::string)
IMF_STD_ATTRIBUTE_IMP (lensAttributes, LensAttributes, std::string)
IMF_STD_ATTRIBUTE_IMP (focus, Focus, float)
IMF_STD_ATTRIBUTE_IMP (focalLength, FocalLength, float)
IMF_STD_ATTRIBUTE_IMP (aperture, Aperture, float)
IMF_STD_ATTRIBUTE_IMP (convergenceDistance, ConvergenceDistance, float)
IMF_STD_ATTRIBUTE_IMP (interocularDistance, InterocularDistance, float)
IMF_STD_ATTRIBUTE_IMP (multiView, MultiView, StringVector)
IMF_STD_ATTRIBUTE_IMP (recorderMake, RecorderMake, std::string)
IMF_STD_ATTRIBUTE_IMP (recorderModel, RecorderModel, std::string)
IMF_STD_ATTRIBUTE_IMP (recorderSerialNumber, RecorderSerialNumber, std::string)
IMF_STD_ATTRIBUTE_IMP (recorderFirmwareVersion, RecorderFirmwareVersion, std::string)
IMF_STD_ATTRIBUTE_IMP (storageMediaSerialNumber, StorageMediaSerialNumber, std::string)
IMF_STD_ATTRIBUTE_IMP (reelName, ReelName, std::string)
IMF_STD_ATTRIBUTE_IMP (uuid, Uuid, std::string)
IMF_STD_ATTRIBUTE_IMP (keyCode, KeyCode, KeyCode)
IMF_STD_ATTRIBUTE_IMP (expTime, ExpTime, float)
IMF_STD_ATTRIBUTE_IMP (captureRate, CaptureRate, Rational)
IMF_STD_ATTRIBUTE_IMP (timecodeRate, TimecodeRate, int)
IMF_STD_ATTRIBUTE_IMP (timeCode, TimeCode, TimeCode)
IMF_STD_ATTRIBUTE_IMP (imageCounter, ImageCounter, int)
IMF_STD_ATTRIBUTE_IMP (framesPerSecond, FramesPerSecond, Rational)
IMF_STD_ATTRIBUTE_IMP (cameraPosition, CameraPosition, IMATH_NAMESPACE::V3f)
IMF_STD_ATTRIBUTE_IMP (cameraUpDirection, CameraUpDirection, IMATH_NAMESPACE::V3f)
IMF_STD_ATTRIBUTE_IMP (cameraViewingDirection, CameraViewingDirection, IMATH_NAMESPACE::V3f)
IMF_STD_ATTRIBUTE_IMP (worldToCamera, WorldToCamera, M44f)
IMF_STD_ATTRIBUTE_IMP (worldToNDC, WorldToNDC, M44f)
IMF_STD_ATTRIBUTE_IMP (originalDataWindow, OriginalDataWindow, Box2i)
IMF_STD_ATTRIBUTE_IMP (envmap, Envmap, Envmap)
IMF_STD_ATTRIBUTE_IMP (wrapmodes, Wrapmodes, string)
IMF_STD_ATTRIBUTE_IMP (xDensity, XDensity, float)
IMF_STD_ATTRIBUTE_IMP (deepImageState, DeepImageState, DeepImageState)
IMF_STD_ATTRIBUTE_IMP (dwaCompressionLevel, DwaCompressionLevel, float)
IMF_STD_ATTRIBUTE_IMP (idManifest, IDManifest, CompressedIDManifest)

OPENEXR_IMF_INTERNAL_NAMESPACE_SOURCE_EXIT
