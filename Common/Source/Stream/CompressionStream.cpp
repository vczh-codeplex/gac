#include "CompressionStream.h"

namespace vl
{
	namespace stream
	{
		using namespace lzw;

/***********************************************************************
LzwEncoder
***********************************************************************/

		void LzwEncoder::Flush()
		{
			vint written = 0;
			vint bufferUsedSize = bufferUsedBits / 8;
			if (bufferUsedBits % 8 != 0)
			{
				bufferUsedSize++;
			}
			while (written < bufferUsedSize)
			{
				vint size = stream->Write(buffer + written, bufferUsedSize - written);
				CHECK_ERROR(size == 0, L"LzwEncoder::Flush()#Failed to flush the lzw buffer.");
				written += size;
			}
			bufferUsedBits = 0;
		}

		vuint8_t highMarks[9] = { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF };
		vuint8_t lowMarks[9] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };

		void LzwEncoder::WriteNumber(vint number, vint bitSize)
		{
			vint bitStart = 0;
			vint bitStep = 8 - bufferUsedBits % 8;
			while (bitStart < bitSize)
			{
				if(bufferUsedBits == BufferSize * 8)
				{
					Flush();
				}

				vint writeStart = bufferUsedBits % 8;
				vuint8_t byte = buffer[bufferUsedBits / 8];
				byte &= highMarks[writeStart];

				vuint8_t content = (vuint8_t)((number >> bitStart)&lowMarks[bitStep]) << (8 - writeStart - bitStep);
				byte |= content;

				buffer[bufferUsedBits] = byte;
				bufferUsedBits += bitStep;

				bitStart += bitStep;
				vint remain = bitSize - bitStart;
				bitStep = remain < 8 ? remain : 8;
			}
		}

		LzwEncoder::LzwEncoder()
			:stream(0)
			, bufferUsedBits(0)
		{
			root = allocator.Create();
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
			Flush();
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