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
ParsingTreeNode::TraversalVisitor
***********************************************************************/

		ParsingTreeNode::TraversalVisitor::TraversalVisitor(TraverseDirection _direction)
			:direction(_direction)
		{
		}

		void ParsingTreeNode::TraversalVisitor::BeforeVisit(ParsingTreeToken* node)
		{
		}

		void ParsingTreeNode::TraversalVisitor::AfterVisit(ParsingTreeToken* node)
		{
		}

		void ParsingTreeNode::TraversalVisitor::BeforeVisit(ParsingTreeObject* node)
		{
		}

		void ParsingTreeNode::TraversalVisitor::AfterVisit(ParsingTreeObject* node)
		{
		}

		void ParsingTreeNode::TraversalVisitor::BeforeVisit(ParsingTreeArray* node)
		{
		}

		void ParsingTreeNode::TraversalVisitor::AfterVisit(ParsingTreeArray* node)
		{
		}

		void ParsingTreeNode::TraversalVisitor::Visit(ParsingTreeToken* node)
		{
			BeforeVisit(node);
			AfterVisit(node);
		}

		void ParsingTreeNode::TraversalVisitor::Visit(ParsingTreeObject* node)
		{
			BeforeVisit(node);
			switch(direction)
			{
			case TraverseDirection::ByTextPosition:
				{
					FOREACH(Ptr<ParsingTreeNode>, node, node->GetSubNodes())
					{
						node->Accept(this);
					}
				}
				break;
			case TraverseDirection::ByStorePosition:
				{
					FOREACH(Ptr<ParsingTreeNode>, node, node->GetMembers().Values())
					{
						node->Accept(this);
					}
				}
				break;
			}
			AfterVisit(node);
		}

		void ParsingTreeNode::TraversalVisitor::Visit(ParsingTreeArray* node)
		{
			BeforeVisit(node);
			switch(direction)
			{
			case TraverseDirection::ByTextPosition:
				{
					FOREACH(Ptr<ParsingTreeNode>, node, node->GetSubNodes())
					{
						node->Accept(this);
					}
				}
				break;
			case TraverseDirection::ByStorePosition:
				{
					FOREACH(Ptr<ParsingTreeNode>, node, node->GetItems())
					{
						node->Accept(this);
					}
				}
				break;
			}
			AfterVisit(node);
		}

/***********************************************************************
ParsingTreeNode
***********************************************************************/

		bool ParsingTreeNode::BeforeAddChild(Ptr<ParsingTreeNode> node)
		{
			return node->parent==0;
		}

		void ParsingTreeNode::AfterAddChild(Ptr<ParsingTreeNode> node)
		{
			node->parent=this;
			ClearQueryCache();
		}

		bool ParsingTreeNode::BeforeRemoveChild(Ptr<ParsingTreeNode> node)
		{
			return node->parent!=0;
		}

		void ParsingTreeNode::AfterRemoveChild(Ptr<ParsingTreeNode> node)
		{
			node->parent=0;
			ClearQueryCache();
		}

		ParsingTreeNode::ParsingTreeNode(const ParsingTextRange& _codeRange)
			:codeRange(_codeRange)
			,parent(0)
		{
		}

		ParsingTreeNode::~ParsingTreeNode()
		{
		}

		ParsingTextRange ParsingTreeNode::GetCodeRange()
		{
			return codeRange;
		}

		void ParsingTreeNode::SetCodeRange(const ParsingTextRange& range)
		{
			codeRange=range;
		}

		void ParsingTreeNode::InitializeQueryCache()
		{
			const NodeList& subNodes=GetSubNodesInternal();
			ClearQueryCache();
			if(&subNodes)
			{
				CopyFrom(cachedOrderedSubNodes, subNodes>>OrderBy(&CompareTextRange));
				FOREACH(Ptr<ParsingTreeNode>, node, cachedOrderedSubNodes)
				{
					node->InitializeQueryCache();
				}
			}
		}

		void ParsingTreeNode::ClearQueryCache()
		{
			cachedOrderedSubNodes.Clear();
		}

		ParsingTreeNode* ParsingTreeNode::GetParent()
		{
			return parent;
		}

		const ParsingTreeNode::NodeList& ParsingTreeNode::GetSubNodes()
		{
			return cachedOrderedSubNodes;
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
ParsingTreeToken
***********************************************************************/

		const ParsingTreeToken::NodeList& ParsingTreeToken::GetSubNodesInternal()
		{
			return *(NodeList*)0;
		}

		ParsingTreeToken::ParsingTreeToken(const WString& _value, vint _tokenIndex, const ParsingTextRange& _codeRange)
			:ParsingTreeNode(_codeRange)
			,value(_value)
			,tokenIndex(_tokenIndex)
		{
		}

		ParsingTreeToken::~ParsingTreeToken()
		{
		}

		void ParsingTreeToken::Accept(IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		vint ParsingTreeToken::GetTokenIndex()
		{
			return tokenIndex;
		}

		void ParsingTreeToken::SetTokenIndex(vint _tokenIndex)
		{
			tokenIndex=_tokenIndex;
		}

		const WString& ParsingTreeToken::GetValue()
		{
			return value;
		}

		void ParsingTreeToken::SetValue(const WString& _value)
		{
			value=_value;
		}

/***********************************************************************
ParsingTreeObject
***********************************************************************/

		const ParsingTreeObject::NodeList& ParsingTreeObject::GetSubNodesInternal()
		{
			return members.Values();
		}

		ParsingTreeObject::ParsingTreeObject(const WString& _type, const ParsingTextRange& _codeRange)
			:ParsingTreeNode(_codeRange)
			,type(_type)
		{
		}

		ParsingTreeObject::~ParsingTreeObject()
		{
		}

		void ParsingTreeObject::Accept(IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		const WString& ParsingTreeObject::GetType()
		{
			return type;
		}

		void ParsingTreeObject::SetType(const WString& _type)
		{
			type=_type;
		}

		ParsingTreeObject::NodeMap& ParsingTreeObject::GetMembers()
		{
			return members;
		}

		Ptr<ParsingTreeNode> ParsingTreeObject::GetMember(const WString& name)
		{
			vint index=members.Keys().IndexOf(name);
			return index==-1?0:members.Values().Get(index);
		}

		bool ParsingTreeObject::SetMember(const WString& name, Ptr<ParsingTreeNode> node)
		{
			vint index=members.Keys().IndexOf(name);
			if(index!=-1)
			{
				Ptr<ParsingTreeNode> previous=members.Values().Get(index);
				if(previous==node) return true;
				if(!BeforeRemoveChild(previous) || !BeforeAddChild(node)) return false;
				members.Remove(name);
				AfterRemoveChild(previous);
			}
			members.Add(name, node);
			AfterAddChild(node);
			return true;
		}

		bool ParsingTreeObject::RemoveMember(const WString& name)
		{
			vint index=members.Keys().IndexOf(name);
			if(index!=-1)
			{
				Ptr<ParsingTreeNode> previous=members.Values().Get(index);
				if(BeforeRemoveChild(previous))
				{
					members.Remove(name);
					AfterRemoveChild(previous);
					return true;
				}
			}
			return false;
		}

		const ParsingTreeObject::NameList& ParsingTreeObject::GetMemberNames()
		{
			return members.Keys();
		}

/***********************************************************************
ParsingTreeArray
***********************************************************************/

		const ParsingTreeArray::NodeList& ParsingTreeArray::GetSubNodesInternal()
		{
			return items;
		}

		ParsingTreeArray::ParsingTreeArray(const WString& _elementType, const ParsingTextRange& _codeRange)
			:ParsingTreeNode(_codeRange)
			,elementType(_elementType)
		{
		}

		ParsingTreeArray::~ParsingTreeArray()
		{
		}

		void ParsingTreeArray::Accept(IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		const WString& ParsingTreeArray::GetElementType()
		{
			return elementType;
		}

		void ParsingTreeArray::SetElementType(const WString& _elementType)
		{
			elementType=_elementType;
		}

		ParsingTreeArray::NodeArray& ParsingTreeArray::GetItems()
		{
			return items;
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
			return InsertItem(items.Count(), node);
		}

		bool ParsingTreeArray::InsertItem(vint index, Ptr<ParsingTreeNode> node)
		{
			if(0<=index && index<=items.Count())
			{
				if(BeforeAddChild(node))
				{
					items.Insert(index, node);
					AfterAddChild(node);
					return true;
				}
			}
			return false;
		}

		bool ParsingTreeArray::RemoveItem(vint index)
		{
			if(0<=index && index<items.Count())
			{
				Ptr<ParsingTreeNode> previous=items[index];
				if(BeforeRemoveChild(previous))
				{
					items.RemoveAt(index);
					AfterRemoveChild(previous);
					return true;
				}
			}
			return false;
		}

		bool ParsingTreeArray::RemoveItem(Ptr<ParsingTreeNode> node)
		{
			return RemoveItem(items.IndexOf(node.Obj()));
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

		bool ParsingTreeArray::Clear()
		{
			FOREACH(Ptr<ParsingTreeNode>, node, items)
			{
				if(!BeforeRemoveChild(node)) return false;
			}
			FOREACH(Ptr<ParsingTreeNode>, node, items)
			{
				AfterRemoveChild(node);
			}
			items.Clear();
			return true;
		}

/***********************************************************************
ParsingError
***********************************************************************/

		ParsingError::ParsingError()
		{
		}

		ParsingError::ParsingError(ParsingTreeCustomBase* _parsingTree, const WString& _errorMessage)
			:codeRange(_parsingTree->codeRange)
			,parsingTree(_parsingTree)
			,errorMessage(_errorMessage)
		{
		}

		ParsingError::~ParsingError()
		{
		}
	}
}