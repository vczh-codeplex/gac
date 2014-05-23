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
		public:
		};

		class LzwDecoder :public Object, public IDecoder
		{
		public:
		};
	}
}

#endif