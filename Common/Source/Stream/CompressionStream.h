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
			struct Code
			{
				typedef collections::PushOnlyAllocator<Code>	Allocator;

				vint								code;
				collections::ByteObjectMap<Code>	children;
			};
		}

		class LzwEncoder :public Object, public IEncoder
		{
		protected:
			static const vint						BufferSize = 1024;

			IStream*								stream;
			lzw::Code::Allocator					allocator;
			lzw::Code*								root;
			vint									nextIndex;

			vuint8_t								buffer[BufferSize];
			vint									bufferUsedBits;
			lzw::Code*								prefix;
			vuint									indexBits;

			void									Flush();
			void									WriteNumber(vint number, vint bitSize);
		public:
			LzwEncoder();
			~LzwEncoder();

			void									Setup(IStream* _stream)override;
			void									Close()override;
			vint									Write(void* _buffer, vint _size)override;
		};

		class LzwDecoder :public Object, public IDecoder
		{
		protected:
			IStream*								stream;

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