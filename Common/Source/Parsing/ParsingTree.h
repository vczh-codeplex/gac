/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parsing::Parsing Tree

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGTREE
#define VCZH_PARSING_PARSINGTREE

#include "..\Regex\Regex.h"

namespace vl
{
	namespace parsing
	{

/***********************************************************************
位置信息
***********************************************************************/

		struct ParsingTextPos
		{
			vint			index;
			vint			row;
			vint			column;

			ParsingTextPos()
				:index(0)
				,row(0)
				,column(0)
			{
			}

			ParsingTextPos(vint _index, vint _row, vint _column)
				:index(_index)
				,row(_row)
				,column(_column)
			{
			}

			bool operator==(const ParsingTextPos& pos)const{return index==pos.index;}
			bool operator!=(const ParsingTextPos& pos)const{return index!=pos.index;}
			bool operator<(const ParsingTextPos& pos)const{return index<pos.index;}
			bool operator<=(const ParsingTextPos& pos)const{return index<=pos.index;}
			bool operator>(const ParsingTextPos& pos)const{return index>pos.index;}
			bool operator>=(const ParsingTextPos& pos)const{return index>=pos.index;}
		};

		struct ParsingTextRange
		{
			ParsingTextPos	start;
			ParsingTextPos	end;

			ParsingTextRange()
			{
			}

			ParsingTextRange(const ParsingTextPos& _start, const ParsingTextPos& _end)
				:start(_start)
				,end(_end)
			{
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
			typedef collections::IReadonlyList<Ptr<ParsingTreeNode>>	INodeList;

			ParsingTextRange			codeRange;
			ParsingTreeNode*			parent;
			NodeList					cachedOrderedSubNodes;

			virtual const INodeList&	GetSubNodesInternal()=0;
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
			const INodeList&			GetSubNodes();
			Ptr<ParsingTreeNode>		FindNode(const ParsingTextPos& position);
		};

		class ParsingTreeToken : public ParsingTreeNode
		{
		protected:
			vint						tokenIndex;

			const INodeList&			GetSubNodesInternal()override;
		public:
			ParsingTreeToken(vint _tokenIndex=-1, const ParsingTextRange& _codeRange=ParsingTextRange());
			~ParsingTreeToken();

			void						Accept(IVisitor* visitor)override;
			vint						GetTokenIndex();
			void						SetTokenIndex(vint _tokenIndex);
		};

		class ParsingTreeObject : public ParsingTreeNode
		{
		protected:
			typedef collections::Dictionary<WString, Ptr<ParsingTreeNode>>				NodeMap;
			typedef collections::IReadonlyDictionary<WString, Ptr<ParsingTreeNode>>		INodeMap;
			typedef collections::IReadonlyList<WString>									INameList;

			WString						type;
			NodeMap						members;

			const INodeList&			GetSubNodesInternal()override;
		public:
			ParsingTreeObject(const WString& _type=L"", const ParsingTextRange& _codeRange=ParsingTextRange());
			~ParsingTreeObject();

			void						Accept(IVisitor* visitor)override;
			const WString&				GetType();
			void						SetType(const WString& _type);
			INodeMap&					GetMembers();
			Ptr<ParsingTreeNode>		GetMember(const WString& name);
			bool						SetMember(const WString& name, Ptr<ParsingTreeNode> node);
			bool						RemoveMember(const WString& name);
			const INameList&			GetMemberNames();
		};

		class ParsingTreeArray : public ParsingTreeNode
		{
		protected:
			typedef collections::List<Ptr<ParsingTreeNode>>								NodeArray;
			typedef collections::IReadonlyList<Ptr<ParsingTreeNode>>					INodeArray;

			WString						elementType;
			NodeArray					items;

			const INodeList&			GetSubNodesInternal()override;
		public:
			ParsingTreeArray(const WString& _elementType=L"", const ParsingTextRange& _codeRange=ParsingTextRange());
			~ParsingTreeArray();

			void						Accept(IVisitor* visitor)override;
			const WString&				GetElementType();
			void						SetElementType(const WString& _elementType);
			INodeArray&					GetItems();
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
基本数据结构
***********************************************************************/
	}
}

#endif