//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) Contributors to the OpenEXR Project.
//


#ifndef INCLUDED_IMF_OPAQUE_ATTRIBUTE_H
#define INCLUDED_IMF_OPAQUE_ATTRIBUTE_H

//-----------------------------------------------------------------------------
//
//	class OpaqueAttribute
//
//	When an image file is read, OpqaqueAttribute objects are used
//	to hold the values of attributes whose types are not recognized
//	by the reading program.  OpaqueAttribute objects can be read
//	from an image file, copied, and written back to to another image
//	file, but their values are inaccessible.
//
//-----------------------------------------------------------------------------

#include "ImfAttribute.h"
#include "ImfArray.h"
#include "ImfNamespace.h"

OPENEXR_IMF_INTERNAL_NAMESPACE_HEADER_ENTER


class IMF_EXPORT OpaqueAttribute: public Attribute
{
  public:

    //----------------------------
    // Constructors and destructor
    //----------------------------

    OpaqueAttribute (const char typeName[]);
    OpaqueAttribute (const OpaqueAttribute &other);
    virtual ~OpaqueAttribute ();


    //-------------------------------
    // Get this attribute's type name
    //-------------------------------

    virtual const char *	typeName () const;
    

    //------------------------------
    // Make a copy of this attribute
    //------------------------------

    virtual Attribute *		copy () const;


    //----------------
    // I/O and copying
    //----------------

    virtual void		writeValueTo (OPENEXR_IMF_INTERNAL_NAMESPACE::OStream &os,
					      int version) const;

    virtual void		readValueFrom (OPENEXR_IMF_INTERNAL_NAMESPACE::IStream &is,
					       int size,
					       int version);

    virtual void		copyValueFrom (const Attribute &other);


    int                         dataSize() const { return _dataSize; }
    const Array<char>&          data() const { return _data; }
        
  private:

    std::string			_typeName;
    long			_dataSize;
    Array<char>			_data;
};


OPENEXR_IMF_INTERNAL_NAMESPACE_HEADER_EXIT

#endif
