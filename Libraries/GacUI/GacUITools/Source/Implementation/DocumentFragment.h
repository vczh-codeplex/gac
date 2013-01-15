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
			IDocumentContainer*							ownerContainer;
			IDocumentFragment*							ownerFragment;
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
			void										NotifyUpdateFragment();
		public:
			DocumentFragment(IDocumentContainer* _ownerContainer, IDocumentFragment* _ownerFragment, const WString& _friendlyName);
			~DocumentFragment();

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
		public:
			FileDocumentFragment();
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
		public:
			VirtualDocumentFragment();
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