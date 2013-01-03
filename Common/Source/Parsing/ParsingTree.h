/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parsing::Parsing Tree

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGTREE
#define VCZH_PARSING_PARSINGTREE

#include "..\Regex\Regex.h"
#include "..\Stream\Accessor.h"

namespace vl
{
	namespace parsing
	{

/***********************************************************************
位置信息
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
				if(a.index!=UnknownValue && a.index!=UnknownValue)
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

			ParsingTextRange()
			{
				end.index=-1;
				end.column=-1;
			}

			ParsingTextRange(const ParsingTextPos& _start, const ParsingTextPos& _end)
				:start(_start)
				,end(_end)
			{
			}

			ParsingTextRange(const regex::RegexToken* startToken, const regex::RegexToken* endToken)
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

/***********************************************************************
通用语法树
***********************************************************************/

		class ParsingTreeNode;
		class ParsingTreeToken;
		class ParsingTreeObject;
		class ParsingTreeArray;

		class ParsingTreeNode : public Object
		{
		public:
			class IVisitor : public Interface
			{
			public:
				virtual void			Visit(ParsingTreeToken* node)=0;
				virtual void			Visit(ParsingTreeObject* node)=0;
				virtual void			Visit(ParsingTreeArray* node)=0;
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
				TraverseDirection		direction;
			public:
				TraversalVisitor(TraverseDirection _direction);

				virtual void			BeforeVisit(ParsingTreeToken* node);
				virtual void			AfterVisit(ParsingTreeToken* node);
				virtual void			BeforeVisit(ParsingTreeObject* node);
				virtual void			AfterVisit(ParsingTreeObject* node);
				virtual void			BeforeVisit(ParsingTreeArray* node);
				virtual void			AfterVisit(ParsingTreeArray* node);

				virtual void			Visit(ParsingTreeToken* node)override;
				virtual void			Visit(ParsingTreeObject* node)override;
				virtual void			Visit(ParsingTreeArray* node)override;
			};
		protected:
			typedef collections::List<Ptr<ParsingTreeNode>>				NodeList;

			ParsingTextRange			codeRange;
			ParsingTreeNode*			parent;
			NodeList					cachedOrderedSubNodes;

			virtual const NodeList&		GetSubNodesInternal()=0;
			bool						BeforeAddChild(Ptr<ParsingTreeNode> node);
			void						AfterAddChild(Ptr<ParsingTreeNode> node);
			bool						BeforeRemoveChild(Ptr<ParsingTreeNode> node);
			void						AfterRemoveChild(Ptr<ParsingTreeNode> node);
		public:
			ParsingTreeNode(const ParsingTextRange& _codeRange);
			~ParsingTreeNode();

			virtual void				Accept(IVisitor* visitor)=0;
			ParsingTextRange			GetCodeRange();
			void						SetCodeRange(const ParsingTextRange& range);

			void						InitializeQueryCache();
			void						ClearQueryCache();
			ParsingTreeNode*			GetParent();
			const NodeList&				GetSubNodes();

			Ptr<ParsingTreeNode>		FindSubNode(const ParsingTextPos& position);
			Ptr<ParsingTreeNode>		FindSubNode(const ParsingTextRange& range);
			Ptr<ParsingTreeNode>		FindDeepestNode(const ParsingTextPos& position);
			Ptr<ParsingTreeNode>		FindDeepestNode(const ParsingTextRange& range);
		};

		class ParsingTreeToken : public ParsingTreeNode
		{
		protected:
			WString						value;
			vint						tokenIndex;

			const NodeList&				GetSubNodesInternal()override;
		public:
			ParsingTreeToken(const WString& _value, vint _tokenIndex=-1, const ParsingTextRange& _codeRange=ParsingTextRange());
			~ParsingTreeToken();

			void						Accept(IVisitor* visitor)override;
			vint						GetTokenIndex();
			void						SetTokenIndex(vint _tokenIndex);
			const WString&				GetValue();
			void						SetValue(const WString& _value);
		};

		class ParsingTreeObject : public ParsingTreeNode
		{
		protected:
			typedef collections::Dictionary<WString, Ptr<ParsingTreeNode>>				NodeMap;
			typedef collections::SortedList<WString>									NameList;

			WString						type;
			NodeMap						members;

			const NodeList&			GetSubNodesInternal()override;
		public:
			ParsingTreeObject(const WString& _type=L"", const ParsingTextRange& _codeRange=ParsingTextRange());
			~ParsingTreeObject();

			void						Accept(IVisitor* visitor)override;
			const WString&				GetType();
			void						SetType(const WString& _type);
			NodeMap&					GetMembers();
			Ptr<ParsingTreeNode>		GetMember(const WString& name);
			bool						SetMember(const WString& name, Ptr<ParsingTreeNode> node);
			bool						RemoveMember(const WString& name);
			const NameList&				GetMemberNames();
		};

		class ParsingTreeArray : public ParsingTreeNode
		{
		protected:
			typedef collections::List<Ptr<ParsingTreeNode>>								NodeArray;

			WString						elementType;
			NodeArray					items;

			const NodeList&				GetSubNodesInternal()override;
		public:
			ParsingTreeArray(const WString& _elementType=L"", const ParsingTextRange& _codeRange=ParsingTextRange());
			~ParsingTreeArray();

			void						Accept(IVisitor* visitor)override;
			const WString&				GetElementType();
			void						SetElementType(const WString& _elementType);
			NodeArray&					GetItems();
			Ptr<ParsingTreeNode>		GetItem(vint index);
			bool						SetItem(vint index, Ptr<ParsingTreeNode> node);
			bool						AddItem(Ptr<ParsingTreeNode> node);
			bool						InsertItem(vint index, Ptr<ParsingTreeNode> node);
			bool						RemoveItem(vint index);
			bool						RemoveItem(Ptr<ParsingTreeNode> node);
			vint						IndexOfItem(Ptr<ParsingTreeNode> node);
			bool						ContainsItem(Ptr<ParsingTreeNode> node);
			vint						Count();
			bool						Clear();
		};

/***********************************************************************
辅助函数
***********************************************************************/

		extern void						Log(Ptr<ParsingTreeNode> node, const WString& originalInput, stream::TextWriter& writer, const WString& prefix=L"");

/***********************************************************************
语法树基础设施
***********************************************************************/

		class ParsingTreeCustomBase : public Object
		{
		public:
			ParsingTextRange			codeRange;
		};

		class ParsingError : public Object
		{
		public:
			ParsingTextRange			codeRange;
			const regex::RegexToken*	token;
			ParsingTreeCustomBase*		parsingTree;
			WString						errorMessage;

			ParsingError();
			ParsingError(const WString& _errorMessage);
			ParsingError(const regex::RegexToken* _token, const WString& _errorMessage);
			ParsingError(ParsingTreeCustomBase* _parsingTree, const WString& _errorMessage);
			~ParsingError();
		};
	}
}

#endif