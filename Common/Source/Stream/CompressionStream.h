/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Stream::CharFormat

Classes:
***********************************************************************/

#ifndef VCZH_STREAM_COMPRESSIONSTREAM
#define VCZH_STREAM_COMPRESSIONSTREAM

#include "Interfaces.h"
#include "..\Collections\List.h"

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
				typedef collections::PushOnlyAllocator<Code>	Allocator;

				vuint8_t							byte = 0;
				vint								code = 1;
				Code*								parent = 0;
				collections::ByteObjectMap<Code>	children;
			};
		}

		class LzwBase : public Object
		{
		protected:
			lzw::Code::Allocator					allocator;
			lzw::Code*								root;
			vint									nextIndex;
			vuint									indexBits;

			lzw::Code*								CreateCode(lzw::Code* parent, vuint8_t byte);

			LzwBase();
			~LzwBase();
		};

		class LzwEncoder : public LzwBase, public IEncoder
		{
		protected:
			IStream*								stream;

			vuint8_t								buffer[lzw::BufferSize];
			vint									bufferUsedBits;
			lzw::Code*								prefix;

			void									Flush();
			void									WriteNumber(vint number, vint bitSize);
		public:
			LzwEncoder();
			~LzwEncoder();

			void									Setup(IStream* _stream)override;
			void									Close()override;
			vint									Write(void* _buffer, vint _size)override;
		};

		class LzwDecoder :public LzwBase, public IDecoder
		{
		protected:
			IStream*								stream;
			collections::List<lzw::Code*>			dictionary;
			vint									lastIndex;

			vuint8_t								inputBuffer[lzw::BufferSize];
			vint									inputBufferSize;
			vint									inputBufferUsedBits;

			collections::Array<vuint8_t>			outputBuffer;
			vint									outputBufferSize;
			vint									outputBufferUsedBytes;

			bool									ReadNumber(vint& number, vint bitSize);
		public:
			LzwDecoder();
			~LzwDecoder();

			void									Setup(IStream* _stream)override;
			void									Close()override;
			vint									Read(void* _buffer, vint _size)override;
		};
	}
}

#endif