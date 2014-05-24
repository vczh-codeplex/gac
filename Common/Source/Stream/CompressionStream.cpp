#include "CompressionStream.h"

namespace vl
{
	namespace stream
	{

/***********************************************************************
LzwEncoder
***********************************************************************/

		LzwEncoder::LzwEncoder()
			:stream(0)
		{
		}

		LzwEncoder::~LzwEncoder()
		{
		}

		void LzwEncoder::Setup(IStream* _stream)
		{
			stream = _stream;
		}

		void LzwEncoder::Close()
		{
			throw 0;
		}

		vint LzwEncoder::Write(void* _buffer, vint _size)
		{
			throw 0;
		}

/***********************************************************************
LzwDecoder
***********************************************************************/

		LzwDecoder::LzwDecoder()
			:stream(0)
		{
		}

		LzwDecoder::~LzwDecoder()
		{
		}

		void LzwDecoder::Setup(IStream* _stream)
		{
			stream = _stream;
		}

		void LzwDecoder::Close()
		{
			throw 0;
		}

		vint LzwDecoder::Read(void* _buffer, vint _size)
		{
			throw 0;
		}
	}
}