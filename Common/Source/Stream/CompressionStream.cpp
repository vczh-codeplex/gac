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
				CHECK_ERROR(size != 0, L"LzwEncoder::Flush()#Failed to flush the lzw buffer.");
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
				vint byteIndex = bufferUsedBits / 8;
				vuint8_t byte = buffer[byteIndex];
				byte &= highMarks[writeStart];

				vuint8_t content = (vuint8_t)((number >> bitStart)&lowMarks[bitStep]) << (8 - writeStart - bitStep);
				byte |= content;

				buffer[byteIndex] = byte;
				bufferUsedBits += bitStep;

				bitStart += bitStep;
				vint remain = bitSize - bitStart;
				bitStep = remain < 8 ? remain : 8;
			}
		}

		LzwEncoder::LzwEncoder()
			:stream(0)
			, nextIndex(256)
			, bufferUsedBits(0)
			, indexBits(8)
		{
			root = allocator.Create();
			root->code = -1;
			prefix = root;

			for (vint i = 0; i < nextIndex; i++)
			{
				Code* code = allocator.Create();
				code->code = i;
				root->children.Set((vuint8_t)i, code);
			}
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
			if (prefix != root)
			{
				WriteNumber(prefix->code, indexBits);
				prefix = root;
			}
			Flush();
		}

		vint LzwEncoder::Write(void* _buffer, vint _size)
		{
			vuint8_t* bytes = (vuint8_t*)_buffer;
			for (vint i = 0; i < _size; i++)
			{
				vuint8_t byte = bytes[i];
				Code* next = prefix->children.Get(byte);
				if (next)
				{
					prefix = next;
				}
				else
				{
					WriteNumber(prefix->code, indexBits);

					if (nextIndex < MaxDictionarySize)
					{
						Code* code = allocator.Create();
						code->code = nextIndex;
						prefix->children.Set(byte, code);

						if ((nextIndex & (nextIndex - 1)) == 0)
						{
							indexBits++;
						}
						nextIndex++;
					}
					prefix = root->children.Get(byte);
				}
			}
			return _size;
		}

/***********************************************************************
LzwDecoder
***********************************************************************/

		bool LzwDecoder::ReadNumber(vint& number, vint bitSize)
		{
			number = 0;
			if (inputBufferSize == -1)
			{
				return false;
			}

			vint remainBits = inputBufferSize * 8 - inputBufferUsedBits;
			vint writtenBits = 0;
			vint bitStep = 8 - inputBufferUsedBits % 8;
			while (writtenBits < bitSize)
			{
				if (remainBits == 0)
				{
					inputBufferSize = stream->Read(inputBuffer, BufferSize);
					if (inputBufferSize == 0)
					{
						inputBufferSize = -1;
						return false;
					}
					remainBits = inputBufferSize * 8;
					inputBufferUsedBits = 0;
				}

				vuint8_t byte = inputBuffer[inputBufferUsedBits / 8];
				byte >>= (8 - inputBufferUsedBits % 8 - bitStep) & lowMarks[bitStep];
				number |= byte << writtenBits;

				remainBits -= bitStep;
				writtenBits += bitStep;
				vint remain = bitSize - writtenBits;
				bitStep = remain < 8 ? remain : 8;
			}

			return true;
		}

		LzwDecoder::LzwDecoder()
			:stream(0)
			, inputBufferSize(0)
			, inputBufferUsedBits(0)
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
		}

		vint LzwDecoder::Read(void* _buffer, vint _size)
		{
			return 0;
		}
	}
}