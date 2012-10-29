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

		class ParsingTreeNode : public Object
		{
		protected:
			typedef collections::List<Ptr<ParsingTreeNode>>				NodeList;
			typedef collections::IReadonlyList<Ptr<ParsingTreeNode>>	INodeList;

			ParsingTextRange			codeRange;
			NodeList					cachedOrderedSubNodes;

			virtual const INodeList&	GetSubNodesInternal()=0;
		public:
			ParsingTreeNode(const ParsingTextRange& _codeRange);
			~ParsingTreeNode();

			ParsingTextRange			GetCodeRange();
			void						InitializeQueryCache();
			const INodeList&			SubNodes();
			Ptr<ParsingTreeNode>		FindNode(const ParsingTextPos& position);
		};

		class ParsingTreeObject : public ParsingTreeNode
		{
		protected:
			typedef collections::Dictionary<WString, Ptr<ParsingTreeNode>>		NodeMap;
			typedef collections::IDictionary<WString, Ptr<ParsingTreeNode>>		INodeMap;
			typedef collections::IReadonlyList<WString>							INameList;

			NodeMap						members;

			const INodeList&			GetSubNodesInternal()override;
		public:
			ParsingTreeObject(const ParsingTextRange& _codeRange);
			~ParsingTreeObject();

			INodeMap&					GetMembers();
			Ptr<ParsingTreeNode>		GetMember(const WString& name);
			void						SetMember(const WString& name, Ptr<ParsingTreeNode> node);
			void						RemoveMember(const WString& name);
			const INameList&			GetMemberNames();
		};

		class ParsingTreeArray : public ParsingTreeNode
		{
		protected:
			typedef collections::List<Ptr<ParsingTreeNode>>		NodeArray;
			typedef collections::IList<Ptr<ParsingTreeNode>>	INodeArray;

			NodeArray					items;

			const INodeList&			GetSubNodesInternal()override;
		public:
			ParsingTreeArray(const ParsingTextRange& _codeRange);
			~ParsingTreeArray();

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
		};

/***********************************************************************
基本数据结构
***********************************************************************/
	}
}

#endif