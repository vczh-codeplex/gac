/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Stream::Interfaces

Interfaces:
	IStream							£ºÁ÷
***********************************************************************/

#ifndef VCZH_STREAM_INTERFACES
#define VCZH_STREAM_INTERFACES

#include "../Basic.h"
#include "../String.h"
#include "../Collections/List.h"

namespace vl
{
	namespace stream
	{
		class IStream : public virtual Interface
		{
		public:
			virtual bool					CanRead()const=0;
			virtual bool					CanWrite()const=0;
			virtual bool					CanSeek()const=0;
			virtual bool					CanPeek()const=0;
			virtual bool					IsLimited()const=0;
			virtual bool					IsAvailable()const=0;
			virtual void					Close()=0;
			virtual pos_t					Position()const=0;
			virtual pos_t					Size()const=0;
			virtual void					Seek(pos_t _size)=0;
			virtual void					SeekFromBegin(pos_t _size)=0;
			virtual void					SeekFromEnd(pos_t _size)=0;
			virtual vint					Read(void* _buffer, vint _size)=0;
			virtual vint					Write(void* _buffer, vint _size)=0;
			virtual vint					Peek(void* _buffer, vint _size)=0;
		};

		class IEncoder : public Interface
		{
		public:
			virtual void					Setup(IStream* _stream)=0;
			virtual	void					Close()=0;
			virtual vint					Write(void* _buffer, vint _size)=0;
		};

		class IDecoder : public Interface
		{
		public:
			virtual void					Setup(IStream* _stream)=0;
			virtual	void					Close()=0;
			virtual vint					Read(void* _buffer, vint _size)=0;
		};

		namespace internal
		{
			struct Reader
			{
				stream::IStream& input;

				Reader(stream::IStream& _input)
					:input(_input)
				{
				}
			};
				
			struct Writer
			{
				stream::IStream& output;

				Writer(stream::IStream& _output)
					:output(_output)
				{
				}
			};

			template<typename T>
			struct Serialization
			{
				template<typename TIO>
				static bool IO(TIO& io, T& value);
			};

			template<typename T>
			Reader& operator<<(Reader& reader, T& value)
			{
				Serialization<T>::IO(reader, value);
				return reader;
			}

			template<typename T>
			Writer& operator<<(Writer& writer, T& value)
			{
				Serialization<T>::IO(writer, value);
				return writer;
			}

			//---------------------------------------------

			template<>
			struct Serialization<vint32_t>
			{
				static void IO(Reader& reader, vint32_t& value)
				{
					if (reader.input.Read(&value, sizeof(value)) != sizeof(value))
					{
						CHECK_FAIL(L"Deserialization failed.");
					}
				}
					
				static void IO(Writer& writer, vint32_t& value)
				{
					if (writer.output.Write(&value, sizeof(value)) != sizeof(value))
					{
						CHECK_FAIL(L"Serialization failed.");
					}
				}
			};

			template<>
			struct Serialization<vint64_t>
			{
				static void IO(Reader& reader, vint64_t& value)
				{
					vint32_t v = 0;
					Serialization<vint32_t>::IO(reader, v);;
					value = (vint64_t)v;
				}
					
				static void IO(Writer& writer, vint64_t& value)
				{
					vint32_t v = (vint32_t)value;
					Serialization<vint32_t>::IO(writer, v);
				}
			};

			template<>
			struct Serialization<bool>
			{
				static void IO(Reader& reader, bool& value)
				{
					vint8_t v = 0;
					if (reader.input.Read(&v, sizeof(v)) != sizeof(v))
					{
						CHECK_FAIL(L"Deserialization failed.");
					}
					else
					{
						value = v == -1;
					}
				}
					
				static void IO(Writer& writer, bool& value)
				{
					vint8_t v = value ? -1 : 0;
					if (writer.output.Write(&v, sizeof(v)) != sizeof(v))
					{
						CHECK_FAIL(L"Serialization failed.");
					}
				}
			};

			template<typename T>
			struct Serialization<Ptr<T>>
			{
				static void IO(Reader& reader, Ptr<T>& value)
				{
					bool notNull = false;
					reader << notNull;
					if (notNull)
					{
						value = new T;
						Serialization<T>::IO(reader, *value.Obj());
					}
					else
					{
						value = 0;
					}
				}
					
				static void IO(Writer& writer, Ptr<T>& value)
				{
					bool notNull = value;
					writer << notNull;
					if (notNull)
					{
						Serialization<T>::IO(writer, *value.Obj());
					}
				}
			};

			template<>
			struct Serialization<WString>
			{
				static void IO(Reader& reader, WString& value)
				{
					vint32_t count = -1;
					reader << count;

					collections::Array<wchar_t> buffer(count + 1);
					if (reader.input.Read((void*)&buffer[0], count*sizeof(wchar_t)) != count*sizeof(wchar_t))
					{
						CHECK_FAIL(L"Deserialization failed.");
					}
					buffer[count] = 0;

					value = &buffer[0];
				}
					
				static void IO(Writer& writer, WString& value)
				{
					vint32_t count = (vint32_t)value.Length();
					writer << count;
					if (writer.output.Write((void*)value.Buffer(), count*sizeof(wchar_t)) != count*sizeof(wchar_t))
					{
						CHECK_FAIL(L"Serialization failed.");
					}
				}
			};

			template<typename T>
			struct Serialization<collections::List<T>>
			{
				static void IO(Reader& reader, collections::List<T>& value)
				{
					vint32_t count = -1;
					reader << count;
					value.Clear();
					for (vint i = 0; i < count; i++)
					{
						T t;
						reader << t;
						value.Add(t);
					}
				}
					
				static void IO(Writer& writer, collections::List<T>& value)
				{
					vint32_t count = (vint32_t)value.Count();
					writer << count;
					for (vint i = 0; i < count; i++)
					{
						writer << value[i];
					}
				}
			};

			template<typename T>
			struct Serialization<collections::Array<T>>
			{
				static void IO(Reader& reader, collections::Array<T>& value)
				{
					vint32_t count = -1;
					reader << count;
					value.Resize(count);
					for (vint i = 0; i < count; i++)
					{
						reader << value[i];
					}
				}
					
				static void IO(Writer& writer, collections::Array<T>& value)
				{
					vint32_t count = (vint32_t)value.Count();
					writer << count;
					for (vint i = 0; i < count; i++)
					{
						writer << value[i];
					}
				}
			};

			//---------------------------------------------

#define BEGIN_SERIALIZATION(TYPE)\
				template<>\
				struct Serialization<TYPE>\
				{\
					template<typename TIO>\
					static void IO(TIO& op, TYPE& value)\
					{\
						op\

#define SERIALIZE(FIELD)\
						<< value.FIELD\

#define END_SERIALIZATION\
						;\
					}\
				};\

		}
	}
}

#endif