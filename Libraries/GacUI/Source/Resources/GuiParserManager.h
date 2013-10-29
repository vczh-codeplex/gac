/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Resource

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_RESOURCES_GUIPARSERMANAGER
#define VCZH_PRESENTATION_RESOURCES_GUIPARSERMANAGER

#include "..\NativeWindow\GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection;

/***********************************************************************
Parser
***********************************************************************/

		/// <summary>Represents a parser.</summary>
		class IGuiGeneralParser : public IDescriptable, public Description<IGuiGeneralParser>
		{
		public:
			/// <summary>Parse a text.</summary>
			/// <returns>The parsed object. Returns null if failed to parse.</returns>
			/// <param name="text">The text.</param>
			virtual Ptr<Object>						Parse(const WString& text)=0;
		};

		template<typename T>
		class IGuiParser : public IGuiGeneralParser
		{
		public:
			virtual Ptr<T>							ParseWithType(const WString& text)=0;

			Ptr<Object> Parse(const WString& text)override
			{
				return ParseToType(text);
			}
		};

		/// <summary>Parser manager for caching parsing table globally.</summary>
		class IGuiParserManager : public IDescriptable, public Description<IGuiParserManager>
		{
		protected:
			typedef parsing::tabling::ParsingTable			Table;

		public:
			/// <summary>Get a parsing table by name.</summary>
			/// <returns>The parsing table.</returns>
			/// <param name="name">The name.</param>
			virtual Ptr<Table>						GetParsingTable(const WString& name)=0;
			/// <summary>Set a parsing table loader by name.</summary>
			/// <param name="name">The name.</param>
			/// <param name="loader">The parsing table loader.</param>
			virtual bool							SetParsingTable(const WString& name, Func<Ptr<Table>()> loader)=0;
			/// <summary>Get a parser.</summary>
			/// <returns>The parser.</returns>
			/// <param name="name">The name.</param>
			virtual Ptr<IGuiGeneralParser>			GetParser(const WString& name)=0;
			/// <summary>Set a parser by name.</summary>
			/// <param name="name">The name.</param>
			/// <param name="parser">The parser.</param>
			virtual bool							SetParser(const WString& name, Ptr<IGuiGeneralParser> parser)=0;

			template<typename T>
			Ptr<IGuiParser<T>> GetParser(const WString& name)
			{
				return GetParser(name).Cast<IGuiParser<T>>();
			}
		};

		/// <summary>Get the global <see cref="IGuiParserManager"/> object.</summary>
		extern IGuiParserManager*					GetParserManager();
	}
}

#endif