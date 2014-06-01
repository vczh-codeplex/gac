/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Stream::CharFormat

Classes:
***********************************************************************/

#ifndef VCZH_STREAM_COMPRESSIONSTREAM
#define VCZH_STREAM_COMPRESSIONSTREAM

#include "Interfaces.h"

namespace vl
{
	namespace stream
	{

/***********************************************************************
Compression
***********************************************************************/

		namespace lzw
		{
			static const vint						BufferSize = 1024;
			static const vint						MaxDictionarySize = 1 << 24;

			struct Code
			{
				typedef collections::PushOnlyAllocator<Code>			CodeAllocator;
				typedef collections::ByteObjectMap<Code>::Allocator		MapAllocator;

				vuint8_t							byte = 0;
				vint								code = -1;
				Code*								parent = 0;
				vint								size = 0;
				collections::ByteObjectMap<Code>	children;
			};
		}

		class LzwBase : public Object
		{
		protected:
			lzw::Code::CodeAllocator				codeAllocator;
			lzw::Code::MapAllocator					mapAllocator;
			lzw::Code*								root;
			vint									eofIndex = -1;
			vint									nextIndex = 0;
			vint									indexBits = 1;

			void									UpdateIndexBits();
			lzw::Code*								CreateCode(lzw::Code* parent, vuint8_t byte);

			LzwBase();
			LzwBase(bool (&existingBytes)[256]);
			~LzwBase();
		};

		class LzwEncoder : public LzwBase, public IEncoder
		{
		protected:
			IStream*								stream = 0;

			vuint8_t								buffer[lzw::BufferSize];
			vint									bufferUsedBits = 0;
			lzw::Code*								prefix;

			void									Flush();
			void									WriteNumber(vint number, vint bitSize);
		public:
			LzwEncoder();
			LzwEncoder(bool (&existingBytes)[256]);
			~LzwEncoder();

			void									Setup(IStream* _stream)override;
			void									Close()override;
			vint									Write(void* _buffer, vint _size)override;
		};

		class LzwDecoder :public LzwBase, public IDecoder
		{
		protected:
			IStream*								stream = 0;
			collections::List<lzw::Code*>			dictionary;
			lzw::Code*								lastCode = 0;

			vuint8_t								inputBuffer[lzw::BufferSize];
			vint									inputBufferSize = 0;
			vint									inputBufferUsedBits = 0;

			collections::Array<vuint8_t>			outputBuffer;
			vint									outputBufferSize = 0;
			vint									outputBufferUsedBytes = 0;

			bool									ReadNumber(vint& number, vint bitSize);
			void									PrepareOutputBuffer(vint size);
			void									ExpandCodeToOutputBuffer(lzw::Code* code);
		public:
			LzwDecoder();
			LzwDecoder(bool (&existingBytes)[256]);
			~LzwDecoder();

			void									Setup(IStream* _stream)override;
			void									Close()override;
			vint									Read(void* _buffer, vint _size)override;
		};
	}
}

#endif