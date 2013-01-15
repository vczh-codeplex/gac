/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTFRAGMENT
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTFRAGMENT

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentFragment : public Object, public IDocumentFragment
		{
		private:
			List<ICallback*>							callbacks;
			IDocumentContainer*							ownedContainer;
			DocumentFragment*							ownedFragment;
			WString										friendlyName;
			List<Ptr<IDocumentFragment>>				subFragments;
			WString										defaultViewTypeId;
			Dictionary<WString, Ptr<IDocumentView>>		supportedViews;

		protected:

			const List<ICallback*>&						GetCallbacks();
			bool										AddSubFragment(IDocumentFragment* fragment);
			bool										DeleteSubFragment(IDocumentFragment* fragment);
			bool										AddSupportedView(Ptr<IDocumentView> view);
			bool										SetDefaultView(Ptr<IDocumentView> view);
			DocumentFragment*							GetOwnerFragmentInternal();
		public:
			DocumentFragment(IDocumentContainer* _ownedContainer, DocumentFragment* _ownedFragment, const WString& _friendlyName);
			~DocumentFragment();
			
			virtual void								NotifyUpdateFragment();
			bool										AttachCallback(ICallback* callback)override;
			bool										DetachCallback(ICallback* callback)override;

			IDocumentContainer*							GetOwnedContainer()override;
			IDocumentFragment*							GetOwnedFragment()override;
			vint										GetSubFragmentCount()override;
			IDocumentFragment*							GetSubFragment(vint index)override;
			WString										GetFriendlyName()override;

			vint										GetSupportedViewTypeCount()override;
			WString										GetSupportedViewTypeId(vint index)override;
			bool										IsSupportedViewTypeId(const WString& viewTypeId)override;
			WString										GetDefaultViewTypeId()override;
			IDocumentView*								GetView(const WString& viewTypeId)override;

			template<typename T>
			T* GetView()
			{
				return dynamic_cast<T*>(GetView(T::ViewTypeId));
			}
		};

		class FileDocumentFragment : public DocumentFragment
		{
		private:
			bool							modified;
			WString							currentFilePath;
			
		protected:
			void							NotifyUpdateFragment()override;
			virtual bool					LoadDocumentInternal(const WString& filePath)=0;
			virtual bool					SaveDocumentInternal(const WString& filePath)=0;
		public:
			FileDocumentFragment(IDocumentContainer* _ownedContainer, DocumentFragment* _ownedFragment, const WString& _friendlyName, const WString& _filePath);
			~FileDocumentFragment();

			bool							IsStoredInSeparatedFile()override;
			bool							CanSaveSeparately()override;
			bool							CanSaveToAnotherFile()override;
			WString							GetFilePath()override;
			bool							ReloadDocument()override;
			bool							SaveDocument()override;
			bool							SaveDocumentAs(const WString& filePath)override;
			bool							IsModified()override;
		};

		class VirtualDocumentFragment : public DocumentFragment
		{
		protected:
			void							NotifyUpdateFragment()override;
		public:
			VirtualDocumentFragment(IDocumentContainer* _ownedContainer, DocumentFragment* _ownedFragment, const WString& _friendlyName);
			~VirtualDocumentFragment();

			bool							IsStoredInSeparatedFile()override;
			bool							CanSaveSeparately()override;
			bool							CanSaveToAnotherFile()override;
			WString							GetFilePath()override;
			bool							ReloadDocument()override;
			bool							SaveDocument()override;
			bool							SaveDocumentAs(const WString& filePath)override;
			bool							IsModified()override;
		};
	}
}

#endif