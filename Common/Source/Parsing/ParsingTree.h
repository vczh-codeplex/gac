/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Parsing::Parsing Tree

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGTREE
#define VCZH_PARSING_PARSINGTREE

#include "../Regex/Regex.h"
#include "../Stream/Accessor.h"
#include "../Stream/CompressionStream.h"
#include "../Stream/CharFormat.h"
#include "../Collections/OperationCopyFrom.h"
#include "../Reflection/GuiTypeDescriptor.h"

namespace vl
{
	namespace parsing
	{

/***********************************************************************
Location
***********************************************************************/

		struct ParsingTextPos
		{
			static const int	UnknownValue=-2;
			vint				index;
			vint				row;
			vint				column;

			ParsingTextPos()
				:index(UnknownValue)
				,row(UnknownValue)
				,column(UnknownValue)
			{
			}

			ParsingTextPos(vint _index)
				:index(_index)
				,row(UnknownValue)
				,column(UnknownValue)
			{
			}

			ParsingTextPos(vint _row, vint _column)
				:index(UnknownValue)
				,row(_row)
				,column(_column)
			{
			}

			ParsingTextPos(vint _index, vint _row, vint _column)
				:index(_index)
				,row(_row)
				,column(_column)
			{
			}

			static vint Compare(const ParsingTextPos& a, const ParsingTextPos& b)
			{
				if(a.index!=UnknownValue && b.index!=UnknownValue)
				{
					return a.index-b.index;
				}
				else if(a.row!=UnknownValue && a.column!=UnknownValue && b.row!=UnknownValue && b.column!=UnknownValue)
				{
					if(a.row==b.row)
					{
						return a.column-b.column;
					}
					else
					{
						return a.row-b.row;
					}
				}
				else
				{
					return 0;
				}
			}

			bool operator==(const ParsingTextPos& pos)const{return Compare(*this, pos)==0;}
			bool operator!=(const ParsingTextPos& pos)const{return Compare(*this, pos)!=0;}
			bool operator<(const ParsingTextPos& pos)const{return Compare(*this, pos)<0;}
			bool operator<=(const ParsingTextPos& pos)const{return Compare(*this, pos)<=0;}
			bool operator>(const ParsingTextPos& pos)const{return Compare(*this, pos)>0;}
			bool operator>=(const ParsingTextPos& pos)const{return Compare(*this, pos)>=0;}
		};

		struct ParsingTextRange
		{
			ParsingTextPos	start;
			ParsingTextPos	end;
			vint			codeIndex;

			ParsingTextRange()
				:codeIndex(-1)
			{
				end.index=-1;
				end.column=-1;
			}

			ParsingTextRange(const ParsingTextPos& _start, const ParsingTextPos& _end, vint _codeIndex = -1)
				:start(_start)
				, end(_end)
				, codeIndex(_codeIndex)
			{
			}

			ParsingTextRange(const regex::RegexToken* startToken, const regex::RegexToken* endToken)
				:codeIndex(startToken->codeIndex)
			{
				start.index=startToken->start;
				start.row=startToken->rowStart;
				start.column=startToken->columnStart;
				end.index=endToken->start+endToken->length-1;
				end.row=endToken->rowEnd;
				end.column=endToken->columnEnd;
			}

			bool operator==(const ParsingTextRange& range)const{return start==range.start && end==range.end;}
			bool operator!=(const ParsingTextRange& range)const{return start!=range.start || end!=range.end;}
			bool Contains(const ParsingTextPos& pos)const{return start<=pos && pos<=end;}
			bool Contains(const ParsingTextRange& range)const{return start<=range.start && range.end<=end;}
		};
	}

	namespace stream
	{
		namespace internal
		{
			BEGIN_SERIALIZATION(parsing::ParsingTextPos)
				SERIALIZE(index)
				SERIALIZE(row)
				SERIALIZE(column)
			END_SERIALIZATION
			
			BEGIN_SERIALIZATION(parsing::ParsingTextRange)
				SERIALIZE(start)
				SERIALIZE(end)
				SERIALIZE(codeIndex)
			END_SERIALIZATION
		}
	}

	namespace parsing
	{
/***********************************************************************
General Syntax Tree
***********************************************************************/

		class ParsingTreeNode;
		class ParsingTreeToken;
		class ParsingTreeObject;
		class ParsingTreeArray;

		class ParsingTreeNode : public Object, public reflection::Description<ParsingTreeNode>
		{
		public:
			class IVisitor : public Interface
			{
			public:
				virtual void					Visit(ParsingTreeToken* node)=0;
				virtual void					Visit(ParsingTreeObject* node)=0;
				virtual void					Visit(ParsingTreeArray* node)=0;
			};

			class TraversalVisitor : public Object, public IVisitor
			{
			public:
				enum TraverseDirection
				{
					ByTextPosition,
					ByStorePosition
				};
			protected:
				TraverseDirection				direction;
			public:
				TraversalVisitor(TraverseDirection _direction);

				virtual void					BeforeVisit(ParsingTreeToken* node);
				virtual void					AfterVisit(ParsingTreeToken* node);
				virtual void					BeforeVisit(ParsingTreeObject* node);
				virtual void					AfterVisit(ParsingTreeObject* node);
				virtual void					BeforeVisit(ParsingTreeArray* node);
				virtual void					AfterVisit(ParsingTreeArray* node);

				virtual void					Visit(ParsingTreeToken* node)override;
				virtual void					Visit(ParsingTreeObject* node)override;
				virtual void					Visit(ParsingTreeArray* node)override;
			};
		protected:
			typedef collections::List<Ptr<ParsingTreeNode>>				NodeList;

			ParsingTextRange					codeRange;
			ParsingTreeNode*					parent;
			NodeList							cachedOrderedSubNodes;

			virtual const NodeList&				GetSubNodesInternal()=0;
			bool								BeforeAddChild(Ptr<ParsingTreeNode> node);
			void								AfterAddChild(Ptr<ParsingTreeNode> node);
			bool								BeforeRemoveChild(Ptr<ParsingTreeNode> node);
			void								AfterRemoveChild(Ptr<ParsingTreeNode> node);
		public:
			ParsingTreeNode(const ParsingTextRange& _codeRange);
			~ParsingTreeNode();

			virtual void						Accept(IVisitor* visitor)=0;
			virtual Ptr<ParsingTreeNode>		Clone()=0;
			ParsingTextRange					GetCodeRange();
			void								SetCodeRange(const ParsingTextRange& range);

			void								InitializeQueryCache();
			void								ClearQueryCache();
			ParsingTreeNode*					GetParent();
			const NodeList&						GetSubNodes();

			ParsingTreeNode*					FindSubNode(const ParsingTextPos& position);
			ParsingTreeNode*					FindSubNode(const ParsingTextRange& range);
			ParsingTreeNode*					FindDeepestNode(const ParsingTextPos& position);
			ParsingTreeNode*					FindDeepestNode(const ParsingTextRange& range);
		};

		class ParsingTreeToken : public ParsingTreeNode, public reflection::Description<ParsingTreeToken>
		{
		protected:
			WString								value;
			vint								tokenIndex;

			const NodeList&						GetSubNodesInternal()override;
		public:
			ParsingTreeToken(const WString& _value, vint _tokenIndex=-1, const ParsingTextRange& _codeRange=ParsingTextRange());
			~ParsingTreeToken();

			void								Accept(IVisitor* visitor)override;
			Ptr<ParsingTreeNode>				Clone()override;
			vint								GetTokenIndex();
			void								SetTokenIndex(vint _tokenIndex);
			const WString&						GetValue();
			void								SetValue(const WString& _value);
		};

		class ParsingTreeObject : public ParsingTreeNode, public reflection::Description<ParsingTreeObject>
		{
		protected:
			typedef collections::Dictionary<WString, Ptr<ParsingTreeNode>>				NodeMap;
			typedef collections::SortedList<WString>									NameList;
			typedef collections::List<WString>											RuleList;

			WString								type;
			NodeMap								members;
			RuleList							rules;

			const NodeList&			GetSubNodesInternal()override;
		public:
			ParsingTreeObject(const WString& _type=L"", const ParsingTextRange& _codeRange=ParsingTextRange());
			~ParsingTreeObject();

			void								Accept(IVisitor* visitor)override;
			Ptr<ParsingTreeNode>				Clone()override;
			const WString&						GetType();
			void								SetType(const WString& _type);
			NodeMap&							GetMembers();
			Ptr<ParsingTreeNode>				GetMember(const WString& name);
			bool								SetMember(const WString& name, Ptr<ParsingTreeNode> node);
			bool								RemoveMember(const WString& name);
			const NameList&						GetMemberNames();
			RuleList&							GetCreatorRules();
		};

		class ParsingTreeArray : public ParsingTreeNode, public reflection::Description<ParsingTreeArray>
		{
		protected:
			typedef collections::List<Ptr<ParsingTreeNode>>								NodeArray;

			WString								elementType;
			NodeArray							items;

			const NodeList&						GetSubNodesInternal()override;
		public:
			ParsingTreeArray(const WString& _elementType=L"", const ParsingTextRange& _codeRange=ParsingTextRange());
			~ParsingTreeArray();

			void								Accept(IVisitor* visitor)override;
			Ptr<ParsingTreeNode>				Clone()override;
			const WString&						GetElementType();
			void								SetElementType(const WString& _elementType);
			NodeArray&							GetItems();
			Ptr<ParsingTreeNode>				GetItem(vint index);
			bool								SetItem(vint index, Ptr<ParsingTreeNode> node);
			bool								AddItem(Ptr<ParsingTreeNode> node);
			bool								InsertItem(vint index, Ptr<ParsingTreeNode> node);
			bool								RemoveItem(vint index);
			bool								RemoveItem(ParsingTreeNode* node);
			vint								IndexOfItem(ParsingTreeNode* node);
			bool								ContainsItem(ParsingTreeNode* node);
			vint								Count();
			bool								Clear();
		};

/***********************************************************************
�﷨��������ʩ
***********************************************************************/

		class ParsingTreeCustomBase : public Object, public reflection::Description<ParsingTreeCustomBase>
		{
		public:
			ParsingTextRange					codeRange;
			collections::List<WString>			creatorRules;
		};

		class ParsingToken : public ParsingTreeCustomBase, public reflection::Description<ParsingToken>
		{
		public:
			vint								tokenIndex;
			WString								value;

			ParsingToken():tokenIndex(-1){}
		};

		class ParsingError : public Object, public reflection::Description<ParsingError>
		{
		public:
			ParsingTextRange					codeRange;
			const regex::RegexToken*			token;
			ParsingTreeCustomBase*				parsingTree;
			WString								errorMessage;

			ParsingError();
			ParsingError(const WString& _errorMessage);
			ParsingError(const regex::RegexToken* _token, const WString& _errorMessage);
			ParsingError(ParsingTreeCustomBase* _parsingTree, const WString& _errorMessage);
			~ParsingError();
		};

/***********************************************************************
Syntax Tree Serialization Helper
***********************************************************************/

		class ParsingTreeConverter : public Object
		{
		public:
			typedef collections::List<regex::RegexToken>	TokenList;

			virtual Ptr<ParsingTreeCustomBase>				ConvertClass(Ptr<ParsingTreeObject> obj, const TokenList& tokens)=0;

			bool SetMember(ParsingToken& member, Ptr<ParsingTreeNode> node, const TokenList& tokens)
			{
				Ptr<ParsingTreeToken> token=node.Cast<ParsingTreeToken>();
				if(token)
				{
					member.tokenIndex=token->GetTokenIndex();
					member.value=token->GetValue();
					member.codeRange=token->GetCodeRange();
					return true;
				}
				return false;
			}

			template<typename T>
			bool SetMember(collections::List<T>& member, Ptr<ParsingTreeNode> node, const TokenList& tokens)
			{
				Ptr<ParsingTreeArray> arr=node.Cast<ParsingTreeArray>();
				if(arr)
				{
					member.Clear();
					vint count=arr->Count();
					for(vint i=0;i<count;i++)
					{
						T t;
						SetMember(t, arr->GetItem(i), tokens);
						member.Add(t);
					}
					return true;
				}
				return false;
			}

			template<typename T>
			bool SetMember(Ptr<T>& member, Ptr<ParsingTreeNode> node, const TokenList& tokens)
			{
				Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>();
				if(obj)
				{
					Ptr<ParsingTreeCustomBase> tree=ConvertClass(obj, tokens);
					if(tree)
					{
						tree->codeRange=node->GetCodeRange();
						member=tree.Cast<T>();
						return member;
					}
				}
				return false;
			}
		};

/***********************************************************************
Logging
***********************************************************************/

		class IParsingPrintNodeRecorder : public virtual Interface
		{
		public:
			virtual void						Record(ParsingTreeCustomBase* node, const ParsingTextRange& range) = 0;
		};

		class ParsingEmptyPrintNodeRecorder : public Object, public virtual IParsingPrintNodeRecorder
		{
		public:
			ParsingEmptyPrintNodeRecorder();
			~ParsingEmptyPrintNodeRecorder();

			void								Record(ParsingTreeCustomBase* node, const ParsingTextRange& range)override;
		};

		class ParsingMultiplePrintNodeRecorder : public Object, public virtual IParsingPrintNodeRecorder
		{
			typedef collections::List<Ptr<IParsingPrintNodeRecorder>>				RecorderList;
		protected:
			RecorderList						recorders;

		public:
			ParsingMultiplePrintNodeRecorder();
			~ParsingMultiplePrintNodeRecorder();

			void								AddRecorder(Ptr<IParsingPrintNodeRecorder> recorder);
			void								Record(ParsingTreeCustomBase* node, const ParsingTextRange& range)override;
		};

		class ParsingOriginalLocationRecorder : public Object, public virtual IParsingPrintNodeRecorder
		{
		protected:
			Ptr<IParsingPrintNodeRecorder>		recorder;

		public:
			ParsingOriginalLocationRecorder(Ptr<IParsingPrintNodeRecorder> _recorder);
			~ParsingOriginalLocationRecorder();

			void								Record(ParsingTreeCustomBase* node, const ParsingTextRange& range)override;
		};

		class ParsingGeneratedLocationRecorder : public Object, public virtual IParsingPrintNodeRecorder
		{
			typedef collections::Dictionary<ParsingTreeCustomBase*, ParsingTextRange>		RangeMap;
		protected:
			RangeMap&							rangeMap;

		public:
			ParsingGeneratedLocationRecorder(RangeMap& _rangeMap);
			~ParsingGeneratedLocationRecorder();

			void								Record(ParsingTreeCustomBase* node, const ParsingTextRange& range)override;
		};

		class ParsingWriter : public stream::TextWriter
		{
			typedef collections::Pair<ParsingTreeCustomBase*, ParsingTextPos>				NodePosPair;
			typedef collections::List<NodePosPair>											NodePosList;
		protected:
			stream::TextWriter&					writer;
			Ptr<IParsingPrintNodeRecorder>		recorder;
			vint								codeIndex;
			ParsingTextPos						lastPos;
			ParsingTextPos						currentPos;
			NodePosList							nodePositions;

			void								HandleChar(wchar_t c);
		public:
			ParsingWriter(stream::TextWriter& _writer, Ptr<IParsingPrintNodeRecorder> _recorder = nullptr, vint _codeIndex = -1);
			~ParsingWriter();

			using stream::TextWriter::WriteString;
			void								WriteChar(wchar_t c)override;
			void								WriteString(const wchar_t* string, vint charCount)override;
			void								BeforePrint(ParsingTreeCustomBase* node);
			void								AfterPrint(ParsingTreeCustomBase* node);
		};

		extern void								Log(ParsingTreeNode* node, const WString& originalInput, stream::TextWriter& writer, const WString& prefix=L"");
	}
}

#endif
