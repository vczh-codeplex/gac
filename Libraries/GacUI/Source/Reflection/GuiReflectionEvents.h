/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Events

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONEVENTS
#define VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONEVENTS

#include "GuiReflectionControls.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
Type List
***********************************************************************/

#define GUIREFLECTIONEVENT_TYPELIST(F)\
			F(presentation::compositions::GuiEventArgs)\
			F(presentation::compositions::GuiRequestEventArgs)\
			F(presentation::compositions::GuiKeyEventArgs)\
			F(presentation::compositions::GuiCharEventArgs)\
			F(presentation::compositions::GuiMouseEventArgs)\
			F(presentation::compositions::GuiItemEventArgs)\
			F(presentation::compositions::GuiItemMouseEventArgs)\
			F(presentation::compositions::GuiNodeEventArgs)\
			F(presentation::compositions::GuiNodeMouseEventArgs)\

			GUIREFLECTIONEVENT_TYPELIST(DECL_TYPE_INFO)

/***********************************************************************
GuiEventInfoImpl
***********************************************************************/

			template<typename T>
			class GuiEventInfoImpl : public EventInfoImpl
			{
			protected:
				typedef Func<GuiGraphicsEvent<T>*(DescriptableObject*)>		EventRetriverFunction;

				EventRetriverFunction				eventRetriver;

				void AttachInternal(DescriptableObject* thisObject, IEventHandler* eventHandler)
				{
					if(thisObject)
					{
						if(EventHandlerImpl* handlerImpl=dynamic_cast<EventHandlerImpl*>(eventHandler))
						{
							GuiGraphicsEvent<T>* eventObject=eventRetriver(thisObject);
							if(eventObject)
							{
								Ptr<GuiGraphicsEvent<T>::IHandler> handler=eventObject->AttachLambda(
									[=](GuiGraphicsComposition* sender, T& arguments)
									{
										Value thisObject=BoxValue<GuiGraphicsComposition*>(sender, Description<GuiGraphicsComposition>::GetAssociatedTypeDescriptor());
										Value argumentsObject=BoxValue<T*>(&arguments, Description<T>::GetAssociatedTypeDescriptor());
										eventHandler->Invoke(thisObject, argumentsObject);
									});
								handlerImpl->SetTag(handler);
							}
						}
					}
				}

				void DetachInternal(DescriptableObject* thisObject, IEventHandler* eventHandler)
				{
					if(thisObject)
					{
						if(EventHandlerImpl* handlerImpl=dynamic_cast<EventHandlerImpl*>(eventHandler))
						{
							GuiGraphicsEvent<T>* eventObject=eventRetriver(thisObject);
							if(eventObject)
							{
								Ptr<GuiGraphicsEvent<T>::IHandler> handler=handlerImpl->GetTag().Cast<GuiGraphicsEvent<T>::IHandler>();
								if(handler)
								{
									eventObject->Detach(handler);
								}
							}
						}
					}
				}
			public:
				GuiEventInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name, const EventRetriverFunction& _eventRetriver)
					:EventInfoImpl(_ownerTypeDescriptor, _name)
					,eventRetriver(_eventRetriver)
				{
				}

				~GuiEventInfoImpl()
				{
				}
			};

/***********************************************************************
Macros
***********************************************************************/

/***********************************************************************
Type Loader
***********************************************************************/

			extern bool						LoadGuiEventTypes();
		}
	}
}

#endif