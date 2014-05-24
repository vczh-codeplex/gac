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

		class LzwEncoder :public Object, public IEncoder
		{
		protected:
			IStream*						stream;

		public:
			LzwEncoder();
			~LzwEncoder();

			void							Setup(IStream* _stream)override;
			void							Close()override;
			vint							Write(void* _buffer, vint _size)override;
		};

		class LzwDecoder :public Object, public IDecoder
		{
		protected:
			IStream*						stream;

		public:
			LzwDecoder();
			~LzwDecoder();

			void							Setup(IStream* _stream)override;
			void							Close()override;
			vint							Read(void* _buffer, vint _size)override;
		};
	}
}

#endif