//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
//=---------------------------------------------------------------------=

#include <AxTypes.h>

#include <memory>

class AxFGOp;


// Simple class representing a buffer.

class SampleSrcBuffer { 
public:
	virtual ~SampleSrcBuffer() = 0;

	virtual int GetNumSamples() = 0;
	virtual int GetNumBytes() = 0;

	// Caller does *not* take ownership of the pointer.
	virtual aafUInt8* GetBufferPtr() = 0;
};


// Simplest possible implementation.  The class takes
// ownership of a buffer pointer, and it is destroyed
// when the object is destroyed.

// Now then... we see the problems with declaring typedefs
// that are pointers.  This: std::auto_ptr<aafDataBuffer_t>
// ... won't work because auto_ptr stores and returns pointers
// to the templated class.  And what type underlies the aafDataBuffer_t.
// gee whiz... I don't know... lets go digging in AAFTypes.h yet
// again.  aafDataBuffer_t is a pointer to an aafUInt8...


class SimpleSampleSrcBuffer : public SampleSrcBuffer {

public:

	SimpleSampleSrcBuffer()
	: _numSamples(0),
	  _numBytes(0),
	  _buffer( std::auto_ptr<aafUInt8>(0) )
	{}

	SimpleSampleSrcBuffer( int numSamples,
						   int numBytes,
						   std::auto_ptr<aafUInt8> buffer )
	: _numSamples( numSamples ),
	  _numBytes( numBytes ),
	  _buffer( buffer )
	{}

	~SimpleSampleSrcBuffer()
	{}

	int GetNumSamples()
	{ return _numSamples; }

	int GetNumBytes()
	{ return _numBytes; }

	aafUInt8* GetBufferPtr()
	{ return _buffer.get(); }

private:

	int _numSamples;
	int _numBytes;
	std::auto_ptr<aafUInt8> _buffer;
};


// Simplest possible interface.  Just get sample buffers until
// a null pointer is returned.  The object should be reset
// to begin, once again, at the first buffer.

// There's no simple implementation for this one... you have
// work to do!

class SampleSource {
public:
	virtual ~SampleSource() = 0;
	
	virtual void Reset() = 0;

	virtual std::auto_ptr< SampleSrcBuffer > GetNext() = 0;
};


// Get a reference to a SampleSource given a AxFGOp

// In this case, op is the one you want to cast to sample source
// You can do it yourself if you want:  dynamic_cast<SampleSource&>(op)
SampleSource& GetSampleSource( AxFGOp& op );

// In this case, op is the operation that want to get at a SampleSource
// referenced by name.  "op" in this case is *not* the AxFGOp that is
// expected to implement SampleSource - it is any old op implementation.
SampleSource& GetSampleSource( AxFGOp& op, const AxString& name );