#include "ParsingTree.h"
#include "..\Collections\Operation.h"

namespace vl
{
	using namespace collections;

	namespace parsing
	{
		vint CompareTextRange(Ptr<ParsingTreeNode> r1, Ptr<ParsingTreeNode> r2)
		{
			return r1->GetCodeRange().start.index-r2->GetCodeRange().start.index;
		}

/***********************************************************************
ParsingTreeNode
***********************************************************************/

		ParsingTreeNode::ParsingTreeNode(const ParsingTextRange& _codeRange)
			:codeRange(_codeRange)
		{
		}

		ParsingTreeNode::~ParsingTreeNode()
		{
		}

		ParsingTextRange ParsingTreeNode::GetCodeRange()
		{
			return codeRange;
		}

		void ParsingTreeNode::InitializeQueryCache()
		{
			CopyFrom(cachedOrderedSubNodes.Wrap(), GetSubNodesInternal()>>OrderBy(&CompareTextRange));
		}

		const ParsingTreeNode::INodeList& ParsingTreeNode::SubNodes()
		{
			return cachedOrderedSubNodes.Wrap();
		}

		Ptr<ParsingTreeNode> ParsingTreeNode::FindNode(const ParsingTextPos& position)
		{
			vint start=0;
			vint end=cachedOrderedSubNodes.Count()-1;
			while(start<=end)
			{
				vint selected=(start+end)/2;
				ParsingTreeNode* selectedNode=cachedOrderedSubNodes[selected].Obj();
				if(position<selectedNode->codeRange.start)
				{
					end=selected-1;
				}
				else if(position>selectedNode->codeRange.end)
				{
					start=selected+1;
				}
				else
				{
					return cachedOrderedSubNodes[selected];
				}
			}
			return 0;
		}

/***********************************************************************
ParsingTreeObject
***********************************************************************/

		const ParsingTreeObject::INodeList& ParsingTreeObject::GetSubNodesInternal()
		{
			return members.Values();
		}

		ParsingTreeObject::ParsingTreeObject(const ParsingTextRange& _codeRange)
			:ParsingTreeNode(_codeRange)
		{
		}

		ParsingTreeObject::~ParsingTreeObject()
		{
		}

		ParsingTreeObject::INodeMap& ParsingTreeObject::GetMembers()
		{
			return members.Wrap();
		}

		Ptr<ParsingTreeNode> ParsingTreeObject::GetMember(const WString& name)
		{
			vint index=members.Keys().IndexOf(name);
			return index==-1?0:members.Values()[index];
		}

		void ParsingTreeObject::SetMember(const WString& name, Ptr<ParsingTreeNode> node)
		{
			if(node)
			{
				members.Set(name, node);
			}
			else
			{
				members.Remove(name);
			}
		}

		void ParsingTreeObject::RemoveMember(const WString& name)
		{
			members.Remove(name);
		}

		const ParsingTreeObject::INameList& ParsingTreeObject::GetMemberNames()
		{
			return members.Keys();
		}

/***********************************************************************
ParsingTreeArray
***********************************************************************/

		const ParsingTreeArray::INodeList& ParsingTreeArray::GetSubNodesInternal()
		{
			return items.Wrap();
		}

		ParsingTreeArray::ParsingTreeArray(const ParsingTextRange& _codeRange)
			:ParsingTreeNode(_codeRange)
		{
		}

		ParsingTreeArray::~ParsingTreeArray()
		{
		}

		ParsingTreeArray::INodeArray& ParsingTreeArray::GetItems()
		{
			return items.Wrap();
		}

		Ptr<ParsingTreeNode> ParsingTreeArray::GetItem(vint index)
		{
			if(0<=index && index<items.Count())
			{
				return items[index];
			}
			else
			{
				return 0;
			}
		}

		bool ParsingTreeArray::SetItem(vint index, Ptr<ParsingTreeNode> node)
		{
			if(0<=index && index<items.Count())
			{
				items[index]=node;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool ParsingTreeArray::AddItem(Ptr<ParsingTreeNode> node)
		{
			items.Add(node);
			return true;
		}

		bool ParsingTreeArray::InsertItem(vint index, Ptr<ParsingTreeNode> node)
		{
			if(0<=index && index<=items.Count())
			{
				items.Insert(index, node);
				return true;
			}
			else
			{
				return false;
			}
		}

		bool ParsingTreeArray::RemoveItem(vint index)
		{
			if(0<=index && index<items.Count())
			{
				items.RemoveAt(index);
				return true;
			}
			else
			{
				return false;
			}
		}

		bool ParsingTreeArray::RemoveItem(Ptr<ParsingTreeNode> node)
		{
			return items.Remove(node.Obj());
		}

		vint ParsingTreeArray::IndexOfItem(Ptr<ParsingTreeNode> node)
		{
			return items.IndexOf(node.Obj());
		}

		bool ParsingTreeArray::ContainsItem(Ptr<ParsingTreeNode> node)
		{
			return items.Contains(node.Obj());
		}

		vint ParsingTreeArray::Count()
		{
			return items.Count();
		}
	}
}