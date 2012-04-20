/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITREEVIEWCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUITREEVIEWCONTROLS

#include "..\GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiVirtualTreeListControl NodeProvider
***********************************************************************/

			namespace tree
			{
				class INodeProvider;
				class INodeRootProvider;

				//-----------------------------------------------------------
				// Callback Interfaces
				//-----------------------------------------------------------

				/// <summary>Callback object for <see cref="INodeProvider"/>. A node will invoke this callback to notify any content modification.</summary>
				class INodeProviderCallback : public virtual IDescriptable, public Description<INodeProviderCallback>
				{
				public:
					/// <summary>Called when this callback is attached to a node root.</summary>
					/// <param name="provider">The root node.</param>
					virtual void					OnAttached(INodeRootProvider* provider)=0;
					/// <summary>Called before sub items of a node are modified.</summary>
					/// <param name="parentNode">The node containing modified sub items.</param>
					/// <param name="start">The index of the first sub item.</param>
					/// <param name="count">The number of sub items to be modified.</param>
					/// <param name="newCount">The new number of modified sub items.</param>
					virtual void					OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)=0;
					/// <summary>Called after sub items of a node are modified.</summary>
					/// <param name="parentNode">The node containing modified sub items.</param>
					/// <param name="start">The index of the first sub item.</param>
					/// <param name="count">The number of sub items to be modified.</param>
					/// <param name="newCount">The new number of modified sub items.</param>
					virtual void					OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)=0;
					/// <summary>Called when a node is expanded.</summary>
					/// <param name="provider">The node.</param>
					virtual void					OnItemExpanded(INodeProvider* node)=0;
					/// <summary>Called when a node is collapsed.</summary>
					/// <param name="provider">The node.</param>
					virtual void					OnItemCollapsed(INodeProvider* node)=0;
				};

				//-----------------------------------------------------------
				// Provider Interfaces
				//-----------------------------------------------------------

				/// <summary>Represents a node.</summary>
				class INodeProvider : public virtual IDescriptable, public Description<INodeProvider>
				{
				public:
					/// <summary>Get the expanding state of this node.</summary>
					/// <returns>Returns true if this node is expanded.</returns>
					virtual bool					GetExpanding()=0;
					/// <summary>Set the expanding state of this node.</summary>
					/// <param name="value">Set to true to expand this node.</param>
					virtual void					SetExpanding(bool value)=0;
					/// <summary>Calculate the number of total visible nodes of this node. The number of total visible nodes includes the node itself, and all total visible nodes of all visible sub nodes. If this node is collapsed, this number will be 1.</summary>
					/// <returns>The number of total visible nodes.</returns>
					virtual int						CalculateTotalVisibleNodes()=0;

					/// <summary>Get the number of all sub nodes.</summary>
					/// <returns>The number of all sub nodes.</returns>
					virtual int						GetChildCount()=0;
					/// <summary>Get the parent node.</summary>
					/// <returns>The parent node.</returns>
					virtual INodeProvider*			GetParent()=0;
					/// <summary>Request the instance of a specified sub node. If the sub node is not longer needed, a call to [M:vl.presentation.controls.tree.INodeProvider.ReleaseChild] is required.</summary>
					/// <returns>The instance of a specified sub node.</returns>
					/// <param name="index">The index of the sub node.</param>
					virtual INodeProvider*			RequestChild(int index)=0;
					/// <summary>Release an instance of a sub node.</summary>
					/// <param name="node">The instance of a sub node.</param>
					virtual void					ReleaseChild(INodeProvider* node)=0;
				};
				
				/// <summary>Represents a root node provider.</summary>
				class INodeRootProvider : public virtual IDescriptable, public Description<INodeRootProvider>
				{
				public:
					/// <summary>Get the instance of the root node.</summary>
					virtual INodeProvider*			GetRootNode()=0;
					/// <summary>Test does the provider provided an optimized algorithm to get an instance of a node by the index of all visible nodes. If this function returns true, [M:vl.presentation.controls.tree.INodeRootProvider.GetNodeByVisibleIndex] can be used.</summary>
					/// <returns>Returns true if such an algorithm is provided.</returns>
					virtual bool					CanGetNodeByVisibleIndex()=0;
					/// <summary>Get a node by the index in all visible nodes. This requires [M:vl.presentation.controls.tree.INodeRootProvider.CanGetNodeByVisibleIndex] returning true. If the node is no longer needed, a call to the parent's [M:vl.presentation.controls.tree.INodeProvider.ReleaseChild] is needed, unless this is a root node so that its parent is null.</summary>
					/// <returns>The node for the index in all visible nodes.</returns>
					/// <param name="index">The index in all visible nodes.</param>
					virtual INodeProvider*			GetNodeByVisibleIndex(int index)=0;
					/// <summary>Attach an node provider callback to this node provider.</summary>
					/// <returns>Returns true if this operation succeeded.</returns>
					/// <param name="value">The node provider callback.</param>
					virtual bool					AttachCallback(INodeProviderCallback* value)=0;
					/// <summary>Detach an node provider callback from this node provider.</summary>
					/// <returns>Returns true if this operation succeeded.</returns>
					/// <param name="value">The node provider callback.</param>
					virtual bool					DetachCallback(INodeProviderCallback* value)=0;
					/// <summary>Request a view for this node provider. If the specified view is not supported, it returns null. If you want to get a view of type IXXX, use IXXX::Identifier as the identifier. When the view object is no longer needed, A call to the [M:vl.presentation.controls.tree.INodeRootProvider.ReleaseView] is needed.</summary>
					/// <returns>The view object.</returns>
					/// <param name="identifier">The identifier for the requested view.</param>
					virtual IDescriptable*			RequestView(const WString& identifier)=0;
					/// <summary>Release a requested view.</summary>
					/// <param name="view">The view to release.</param>
					virtual void					ReleaseView(IDescriptable* view)=0;
				};
			}

			namespace tree
			{
				//-----------------------------------------------------------
				// Tree to ListControl (IItemProvider)
				//-----------------------------------------------------------

				/// <summary>The required <see cref="GuiListControl::IItemProvider"/> view for [T:vl.presentation.controls.tree.NodeItemStyleProvider]. [T:vl.presentation.controls.tree.NodeItemProvider] provides this view. In most of the cases, the NodeItemProvider class and this view is not required users to create, or even to touch. [T:vl.presentation.controls.GuiVirtualTreeListControl] and [T:vl.presentation.controls.GuiTreeView] already handled all of this.</summary>
				class INodeItemView : public virtual GuiListControl::IItemPrimaryTextView, public Description<INodeItemView>
				{
				public:
					/// <summary>The identifier of this view.</summary>
					static const wchar_t* const		Identifier;

					/// <summary>Get an instance of a node by the index in all visible nodes. If the node is no longer needed, a call to [M:vl.presentation.controls.tree.INodeItemView.ReleaseNode] is required.</summary>
					/// <returns>The instance of a node by the index in all visible nodes.</returns>
					/// <param name="index">The index in all visible nodes.</param>
					virtual INodeProvider*			RequestNode(int index)=0;
					/// <summary>Release an instance of a node.</summary>
					/// <param name="node">The instance of a node.</param>
					virtual void					ReleaseNode(INodeProvider* node)=0;
					/// <summary>Get the index in all visible nodes of a node.</summary>
					/// <returns>The index in all visible nodes of a node.</returns>
					/// <param name="node">The node to calculate the index.</param>
					virtual int						CalculateNodeVisibilityIndex(INodeProvider* node)=0;
				};

				/// <summary>The required <see cref="INodeRootProvider"/> view for [T:vl.presentation.controls.tree.NodeItemProvider]. This view is always needed to create any customized <see cref="INodeRootProvider"/> implementation.</summary>
				class INodeItemPrimaryTextView : public virtual IDescriptable, public Description<INodeItemPrimaryTextView>
				{
				public:
					/// <summary>The identifier of this view.</summary>
					static const wchar_t* const		Identifier;
					
					/// <summary>Get the primary text of a node.</summary>
					/// <returns>The primary text of a node.</returns>
					/// <param name="node">The node.</param>
					virtual WString					GetPrimaryTextViewText(INodeProvider* node)=0;
				};

				/// <summary>This is a general implementation to convert an <see cref="INodeRootProvider"/> to a <see cref="GuiListControl::IItemProvider"/>. It requires the <see cref="INodeItemPrimaryTextView"/> to provide a <see cref="GuiListControl::IItemPrimaryTextView"/>.</summary>
				class NodeItemProvider
					: public list::ItemProviderBase
					, protected virtual INodeProviderCallback
					, protected virtual INodeItemView
					, public Description<NodeItemProvider>
				{
				protected:
					Ptr<INodeRootProvider>			root;
					INodeItemPrimaryTextView*		nodeItemPrimaryTextView;
					int								offsetBeforeChildModified;

					INodeProvider*					GetNodeByOffset(INodeProvider* provider, int offset);
					void							OnAttached(INodeRootProvider* provider)override;
					void							OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
					void							OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
					void							OnItemExpanded(INodeProvider* node)override;
					void							OnItemCollapsed(INodeProvider* node)override;
					int								CalculateNodeVisibilityIndexInternal(INodeProvider* node);
					int								CalculateNodeVisibilityIndex(INodeProvider* node)override;
					
					bool							ContainsPrimaryText(int itemIndex)override;
					WString							GetPrimaryTextViewText(int itemIndex)override;
					INodeProvider*					RequestNode(int index)override;
					void							ReleaseNode(INodeProvider* node)override;
				public:
					/// <summary>Create an item provider using a node root provider.</summary>
					/// <param name="_root">The node root provider.</param>
					NodeItemProvider(INodeRootProvider* _root);
					~NodeItemProvider();
					
					/// <summary>Get the owned node root provider.</summary>
					/// <returns>The node root provider.</returns>
					Ptr<INodeRootProvider>			GetRoot();
					int								Count()override;
					IDescriptable*					RequestView(const WString& identifier)override;
					void							ReleaseView(IDescriptable* view)override;
				};

				//-----------------------------------------------------------
				// Tree to ListControl (IItemStyleProvider)
				//-----------------------------------------------------------

				class INodeItemStyleProvider;

				/// <summary>Node item style controller for a [T:vl.presentation.controls.GuiVirtualTreeListControl] to render a node.</summary>
				class INodeItemStyleController : public virtual GuiListControl::IItemStyleController, public Description<INodeItemStyleController>
				{
				public:
					/// <summary>Get the owner node style provider.</summary>
					/// <returns>The owner node style provider.</returns>
					virtual INodeItemStyleProvider*					GetNodeStyleProvider()=0;
				};
				
				/// <summary>Node item style provider for a [T:vl.presentation.controls.GuiVirtualTreeListControl] to render visible nodes.</summary>
				class INodeItemStyleProvider : public virtual IDescriptable, public Description<INodeItemStyleProvider>
				{
				public:
					/// <summary>Bind a item style provider. Generally the item style provider will be used to create a item style controller for a list control, because a tree view control is implemented using a list control.</summary>
					/// <param name="styleProvider">The item style provider.</param>
					virtual void									BindItemStyleProvider(GuiListControl::IItemStyleProvider* styleProvider)=0;
					/// <summary>Get the binded item style provider.</summary>
					/// <returns>The binded item style provider.</returns>
					virtual GuiListControl::IItemStyleProvider*		GetBindedItemStyleProvider()=0;
					/// <summary>Called when a node item style provider in installed to a <see cref="GuiListControl"/>.</summary>
					/// <param name="value">The list control.</param>
					virtual void									AttachListControl(GuiListControl* value)=0;
					/// <summary>Called when a node item style provider in uninstalled from a <see cref="GuiListControl"/>.</summary>
					virtual void									DetachListControl()=0;
					/// <summary>Get a node item style id from a node.</summary>
					/// <returns>The node item style id.</returns>
					/// <param name="node">The node.</param>
					virtual int										GetItemStyleId(INodeProvider* node)=0;
					/// <summary>Create a node item style controller from a node item style id.</summary>
					/// <returns>The created node item style controller.</returns>
					/// <param name="styleId">The node item style id.</param>
					virtual INodeItemStyleController*				CreateItemStyle(int styleId)=0;
					/// <summary>Destroy a node item style controller.</summary>
					/// <param name="style">The node item style controller.</param>
					virtual void									DestroyItemStyle(INodeItemStyleController* style)=0;
					/// <summary>Bind a node to a node item style controller.</summary>
					/// <param name="style">The node item style controller.</param>
					/// <param name="node">The node item style id.</param>
					virtual void									Install(INodeItemStyleController* style, INodeProvider* node)=0;
					/// <summary>Change the visual affect of a node item style controller to be selected or unselected.</summary>
					/// <param name="style">The node item style controller.</param>
					/// <param name="value">Set to true if the node item is expected to be rendered as selected.</param>
					virtual void									SetStyleSelected(INodeItemStyleController* style, bool value)=0;
				};
				
				/// <summary>This is a general implementation to convert an <see cref="INodeItemStyleProvider"/> to a <see cref="GuiSelectableListControl::IItemStyleProvider"/>.</summary>
				class NodeItemStyleProvider : public Object, public virtual GuiSelectableListControl::IItemStyleProvider, public Description<NodeItemStyleProvider>
				{
				protected:
					Ptr<INodeItemStyleProvider>						nodeItemStyleProvider;
					GuiListControl*									listControl;
					INodeItemView*									nodeItemView;
				public:
					/// <summary>Create an item style provider using a node item style provider.</summary>
					/// <param name="provider">The node item style provider.</param>
					NodeItemStyleProvider(Ptr<INodeItemStyleProvider> provider);
					~NodeItemStyleProvider();

					void											AttachListControl(GuiListControl* value)override;
					void											DetachListControl()override;
					int												GetItemStyleId(int itemIndex)override;
					GuiListControl::IItemStyleController*			CreateItemStyle(int styleId)override;
					void											DestroyItemStyle(GuiListControl::IItemStyleController* style)override;
					void											Install(GuiListControl::IItemStyleController* style, int itemIndex)override;
					void											SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)override;
				};
			}

/***********************************************************************
GuiVirtualTreeListControl Predefined NodeProvider
***********************************************************************/

			namespace tree
			{
				class MemoryNodeProvider
					: public Object
					, public virtual INodeProvider
					, private collections::IList<Ptr<MemoryNodeProvider>>
					, public Description<MemoryNodeProvider>
				{
					typedef collections::List<Ptr<MemoryNodeProvider>> ChildList;
					typedef collections::IList<Ptr<MemoryNodeProvider>> IChildList;
					typedef collections::IEnumerator<Ptr<MemoryNodeProvider>> ChildListEnumerator;
				protected:
					MemoryNodeProvider*				parent;
					bool							expanding;
					int								childCount;
					int								totalVisibleNodeCount;
					int								offsetBeforeChildModified;
					Ptr<DescriptableObject>			data;
					ChildList						children;

					virtual INodeProviderCallback*	GetCallbackProxyInternal();
					void							OnChildTotalVisibleNodesChanged(int offset);
					void							OnBeforeChildModified(int start, int count, int newCount);
					void							OnAfterChildModified(int start, int count, int newCount);
					bool							OnRequestRemove(MemoryNodeProvider* child);
					bool							OnRequestInsert(MemoryNodeProvider* child);
				private:
					
					ChildListEnumerator*			CreateEnumerator()const;
					bool							Contains(const KeyType<Ptr<MemoryNodeProvider>>::Type& item)const;
					vint							Count()const;
					vint							Count();
					const							Ptr<MemoryNodeProvider>& Get(vint index)const;
					const							Ptr<MemoryNodeProvider>& operator[](vint index)const;
					vint							IndexOf(const KeyType<Ptr<MemoryNodeProvider>>::Type& item)const;
					vint							Add(const Ptr<MemoryNodeProvider>& item);
					bool							Remove(const KeyType<Ptr<MemoryNodeProvider>>::Type& item);
					bool							RemoveAt(vint index);
					bool							RemoveRange(vint index, vint count);
					bool							Clear();
					vint							Insert(vint index, const Ptr<MemoryNodeProvider>& item);
					bool							Set(vint index, const Ptr<MemoryNodeProvider>& item);
				public:
					MemoryNodeProvider();
					MemoryNodeProvider(const Ptr<DescriptableObject>& _data);
					~MemoryNodeProvider();

					Ptr<DescriptableObject>			GetData();
					void							SetData(const Ptr<DescriptableObject>& value);
					void							NotifyDataModified();
					IChildList&						Children();

					bool							GetExpanding()override;
					void							SetExpanding(bool value)override;
					int								CalculateTotalVisibleNodes()override;

					int								GetChildCount()override;
					INodeProvider*					GetParent()override;
					INodeProvider*					RequestChild(int index)override;
					void							ReleaseChild(INodeProvider* node)override;
				};

				class NodeRootProviderBase : public virtual INodeRootProvider, protected virtual INodeProviderCallback, public Description<NodeRootProviderBase>
				{
					collections::List<INodeProviderCallback*>			callbacks;
				protected:
					void							OnAttached(INodeRootProvider* provider)override;
					void							OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
					void							OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
					void							OnItemExpanded(INodeProvider* node)override;
					void							OnItemCollapsed(INodeProvider* node)override;
				public:
					NodeRootProviderBase();
					~NodeRootProviderBase();
					
					bool							CanGetNodeByVisibleIndex()override;
					INodeProvider*					GetNodeByVisibleIndex(int index)override;
					bool							AttachCallback(INodeProviderCallback* value)override;
					bool							DetachCallback(INodeProviderCallback* value)override;
					IDescriptable*					RequestView(const WString& identifier)override;
					void							ReleaseView(IDescriptable* view)override;
				};

				class MemoryNodeRootProvider
					: public MemoryNodeProvider
					, public NodeRootProviderBase
					, public Description<MemoryNodeRootProvider>
				{
				protected:
					INodeProviderCallback*			GetCallbackProxyInternal()override;
				public:
					MemoryNodeRootProvider();
					~MemoryNodeRootProvider();

					INodeProvider*					GetRootNode()override;
				};
			}

/***********************************************************************
GuiVirtualTreeListControl
***********************************************************************/

			/// <summary>Tree view control in virtual node.</summary>
			class GuiVirtualTreeListControl : public GuiSelectableListControl, public Description<GuiVirtualTreeListControl>
			{
			protected:
				tree::NodeItemProvider*				nodeItemProvider;
				tree::INodeItemView*				nodeItemView;
				Ptr<tree::INodeItemStyleProvider>	nodeStyleProvider;
			public:
				GuiVirtualTreeListControl(IStyleProvider* _styleProvider, tree::INodeRootProvider* _nodeRootProvider);
				~GuiVirtualTreeListControl();

				tree::INodeItemView*				GetNodeItemView();
				tree::INodeRootProvider*			GetNodeRootProvider();
				tree::INodeItemStyleProvider*		GetNodeStyleProvider();
				Ptr<tree::INodeItemStyleProvider>	SetNodeStyleProvider(Ptr<tree::INodeItemStyleProvider> styleProvider);
			};

/***********************************************************************
TreeView
***********************************************************************/

			namespace tree
			{
				class ITreeViewItemView : public virtual INodeItemPrimaryTextView, public Description<ITreeViewItemView>
				{
				public:
					static const wchar_t* const		Identifier;

					virtual Ptr<GuiImageData>		GetNodeImage(INodeProvider* node)=0;
					virtual WString					GetNodeText(INodeProvider* node)=0;
				};

				class TreeViewItem : public Object, public Description<TreeViewItem>
				{
				public:
					Ptr<GuiImageData>				image;
					WString							text;

					TreeViewItem();
					TreeViewItem(const Ptr<GuiImageData>& _image, const WString& _text);
				};

				class TreeViewItemRootProvider
					: public MemoryNodeRootProvider
					, protected virtual ITreeViewItemView
					, public Description<TreeViewItemRootProvider>
				{
				protected:

					WString							GetPrimaryTextViewText(INodeProvider* node)override;
					Ptr<GuiImageData>				GetNodeImage(INodeProvider* node)override;
					WString							GetNodeText(INodeProvider* node)override;
				public:
					TreeViewItemRootProvider();
					~TreeViewItemRootProvider();

					IDescriptable*					RequestView(const WString& identifier)override;
					void							ReleaseView(IDescriptable* view)override;
				};
			}
			
			/// <summary>Tree view control.</summary>
			class GuiTreeView : public GuiVirtualTreeListControl, public Description<GuiTreeView>
			{
			public:
				class IStyleProvider : public virtual GuiVirtualTreeListControl::IStyleProvider, public Description<IStyleProvider>
				{
				public:
					virtual GuiSelectableButton::IStyleController*		CreateItemBackground()=0;
					virtual GuiSelectableButton::IStyleController*		CreateItemExpandingDecorator()=0;
					virtual Color										GetTextColor()=0;
				};
			protected:
				IStyleProvider*								styleProvider;
				Ptr<tree::TreeViewItemRootProvider>			nodes;
			public:
				GuiTreeView(IStyleProvider* _styleProvider, tree::INodeRootProvider* _nodeRootProvider=0);
				~GuiTreeView();

				IStyleProvider*								GetTreeViewStyleProvider();
				Ptr<tree::TreeViewItemRootProvider>			Nodes();
			};

			namespace tree
			{
				class TreeViewNodeItemStyleProvider
					: public Object
					, public virtual INodeItemStyleProvider
					, protected virtual INodeProviderCallback
					, public Description<TreeViewNodeItemStyleProvider>
				{
				protected:
#pragma warning(push)
#pragma warning(disable:4250)
					class ItemController : public list::ItemStyleControllerBase, public virtual INodeItemStyleController
					{
					protected:
						TreeViewNodeItemStyleProvider*		styleProvider;
						GuiSelectableButton*				backgroundButton;
						GuiSelectableButton*				expandingButton;
						compositions::GuiTableComposition*	table;
						elements::GuiImageFrameElement*		image;
						elements::GuiSolidLabelElement*		text;

						void								SwitchNodeExpanding();
						void								OnBackgroundButtonDoubleClick(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
						void								OnExpandingButtonDoubleClick(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
						void								OnExpandingButtonClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
					public:
						ItemController(TreeViewNodeItemStyleProvider* _styleProvider);

						INodeItemStyleProvider*				GetNodeStyleProvider()override;
						void								Install(INodeProvider* node);

						bool								GetSelected();
						void								SetSelected(bool value);
						void								UpdateExpandingButton(INodeProvider* associatedNode);
					};
#pragma warning(pop)

					GuiTreeView*							treeControl;
					GuiListControl::IItemStyleProvider*		bindedItemStyleProvider;
					ITreeViewItemView*						treeViewItemView;

				protected:
					ItemController*							GetRelatedController(INodeProvider* node);
					void									UpdateExpandingButton(INodeProvider* node);
					void									OnAttached(INodeRootProvider* provider)override;
					void									OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
					void									OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
					void									OnItemExpanded(INodeProvider* node)override;
					void									OnItemCollapsed(INodeProvider* node)override;
				public:
					TreeViewNodeItemStyleProvider();
					~TreeViewNodeItemStyleProvider();

					void									BindItemStyleProvider(GuiListControl::IItemStyleProvider* styleProvider)override;
					GuiListControl::IItemStyleProvider*		GetBindedItemStyleProvider()override;
					void									AttachListControl(GuiListControl* value)override;
					void									DetachListControl()override;
					int										GetItemStyleId(INodeProvider* node)override;
					INodeItemStyleController*				CreateItemStyle(int styleId)override;
					void									DestroyItemStyle(INodeItemStyleController* style)override;
					void									Install(INodeItemStyleController* style, INodeProvider* node)override;
					void									SetStyleSelected(INodeItemStyleController* style, bool value)override;
				};
			}
		}
	}
}

#endif