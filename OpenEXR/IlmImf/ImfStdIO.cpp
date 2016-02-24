///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.67
//
///////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//
//	Low-level file input and output for OpenEXR
//	based on C++ standard iostreams.
//
//-----------------------------------------------------------------------------

#include <ImfStdIO.h>
#include "Iex.h"
#include <errno.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "ImfNamespace.h"

OPENEXR_IMF_INTERNAL_NAMESPACE_SOURCE_ENTER

#ifdef _WIN32

namespace StrUtils {

std::wstring
utf8_to_utf16 (const std::string& str)
{
    std::wstring native;
    
    native.resize(MultiByteToWideChar (CP_UTF8, 0, str.c_str(), -1, NULL, 0));
    MultiByteToWideChar (CP_UTF8, 0, str.c_str(), -1, &native[0], (int)native.size());

    return native;
}

}

#endif // _WIN32

namespace {

void
clearError ()
{
    errno = 0;
}


bool
checkError (std::istream &is, std::streamsize expected = 0)
{
    if (!is)
    {
	if (errno)
	    IEX_NAMESPACE::throwErrnoExc();

	if (is.gcount() < expected) 
	{
		THROW (IEX_NAMESPACE::InputExc, "Early end of file: read " << is.gcount() 
			<< " out of " << expected << " requested bytes.");
	}
	return false;
    }

    return true;
}


void
checkError (std::ostream &os)
{
    if (!os)
    {
	if (errno)
	    IEX_NAMESPACE::throwErrnoExc();

	throw IEX_NAMESPACE::ErrnoExc ("File output failed.");
    }
}

} // namespace


StdIFStream::StdIFStream (const char fileName[]):
    OPENEXR_IMF_INTERNAL_NAMESPACE::IStream (fileName),
    _is (0),
    _deleteStream (true)
{
#ifdef _WIN32
	std::wstring wfilename = StrUtils::utf8_to_utf16(std::string(fileName));
	_is = new OPENEXR_IMF_INTERNAL_NAMESPACE::ifstream (wfilename, std::ios_base::binary);
#else
	_is = new OPENEXR_IMF_INTERNAL_NAMESPACE::ifstream (fileName, std::ios_base::binary);
#endif

    if (!*_is)
    {
	delete _is;
	IEX_NAMESPACE::throwErrnoExc();
    }
}

    
StdIFStream::StdIFStream (OPENEXR_IMF_INTERNAL_NAMESPACE::ifstream &is, const char fileName[]):
    OPENEXR_IMF_INTERNAL_NAMESPACE::IStream (fileName),
    _is (&is),
    _deleteStream (false)
{
    // empty
}


StdIFStream::~StdIFStream ()
{
    if (_deleteStream)
	delete _is;
}


bool
StdIFStream::read (char c[/*n*/], int n)
{
    if (!*_is)
        throw IEX_NAMESPACE::InputExc ("Unexpected end of file.");

    clearError();
    _is->read (c, n);
    return checkError (*_is, n);
}


Int64
StdIFStream::tellg ()
{
    return std::streamoff (_is->tellg());
}


void
StdIFStream::seekg (Int64 pos)
{
    _is->seekg (pos);
    checkError (*_is);
}


void
StdIFStream::clear ()
{
    _is->clear();
}


StdOFStream::StdOFStream (const char fileName[]):
    OPENEXR_IMF_INTERNAL_NAMESPACE::OStream (fileName),
    _os (0),
    _deleteStream (true)
{
#ifdef _WIN32
	std::wstring wfilename = StrUtils::utf8_to_utf16(std::string(fileName));
	_os = new OPENEXR_IMF_INTERNAL_NAMESPACE::ofstream (wfilename, std::ios_base::binary);
#else
	_os = new OPENEXR_IMF_INTERNAL_NAMESPACE::ofstream (fileName, std::ios_base::binary);
#endif
    if (!*_os)
    {
	delete _os;
	IEX_NAMESPACE::throwErrnoExc();
    }
}


StdOFStream::StdOFStream (OPENEXR_IMF_INTERNAL_NAMESPACE::ofstream &os, const char fileName[]):
    OPENEXR_IMF_INTERNAL_NAMESPACE::OStream (fileName),
    _os (&os),
    _deleteStream (false)
{
    // empty
}


StdOFStream::~StdOFStream ()
{
    if (_deleteStream)
	delete _os;
}


void
StdOFStream::write (const char c[/*n*/], int n)
{
    clearError();
    _os->write (c, n);
    checkError (*_os);
}


Int64
StdOFStream::tellp ()
{
    return std::streamoff (_os->tellp());
}


void
StdOFStream::seekp (Int64 pos)
{
    _os->seekp (pos);
    checkError (*_os);
}


StdOSStream::StdOSStream (): OPENEXR_IMF_INTERNAL_NAMESPACE::OStream ("(string)")
{
    // empty
}


void
StdOSStream::write (const char c[/*n*/], int n)
{
    clearError();
    _os.write (c, n);
    checkError (_os);
}


Int64
StdOSStream::tellp ()
{
    return std::streamoff (_os.tellp());
}


void
StdOSStream::seekp (Int64 pos)
{
    _os.seekp (pos);
    checkError (_os);
}


OPENEXR_IMF_INTERNAL_NAMESPACE_SOURCE_EXIT
