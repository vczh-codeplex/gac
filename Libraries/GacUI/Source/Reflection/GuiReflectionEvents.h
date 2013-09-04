/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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

#ifndef VCZH_DEBUG_NO_REFLECTION

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
				typedef Func<GuiGraphicsEvent<T>*(DescriptableObject*, bool)>		EventRetriverFunction;

				EventRetriverFunction				eventRetriver;

				void AttachInternal(DescriptableObject* thisObject, IEventHandler* eventHandler)override
				{
					if(thisObject)
					{
						if(EventHandlerImpl* handlerImpl=dynamic_cast<EventHandlerImpl*>(eventHandler))
						{
							GuiGraphicsEvent<T>* eventObject=eventRetriver(thisObject, true);
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

				void DetachInternal(DescriptableObject* thisObject, IEventHandler* eventHandler)override
				{
					if(thisObject)
					{
						if(EventHandlerImpl* handlerImpl=dynamic_cast<EventHandlerImpl*>(eventHandler))
						{
							GuiGraphicsEvent<T>* eventObject=eventRetriver(thisObject, false);
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

				void InvokeInternal(DescriptableObject* thisObject, Value& arguments)override
				{
					if(thisObject)
					{
						GuiGraphicsEvent<T>* eventObject=eventRetriver(thisObject, false);
						if(eventObject)
						{
							T* value=UnboxValue<T*>(arguments, Description<T>::GetAssociatedTypeDescriptor());
							eventObject->Execute(*value);
						}
					}
				}

				Ptr<ITypeInfo> GetHandlerTypeInternal()override
				{
					return TypeInfoRetriver<Func<void(GuiGraphicsComposition*, T*)>>::CreateTypeInfo();
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

			template<typename T>
			struct GuiEventArgumentTypeRetriver
			{
				typedef vint								Type;
			};

			template<typename TClass, typename TEvent>
			struct GuiEventArgumentTypeRetriver<TEvent TClass::*>
			{
				typedef typename TEvent::ArgumentType		Type;
			};

/***********************************************************************
Macros
***********************************************************************/

#define CLASS_MEMBER_GUIEVENT(EVENTNAME)\
			AddEvent(\
				new GuiEventInfoImpl<GuiEventArgumentTypeRetriver<decltype(&ClassType::EVENTNAME)>::Type>(\
					this,\
					L#EVENTNAME,\
					[](DescriptableObject* thisObject, bool addEventHandler){\
						return &dynamic_cast<ClassType*>(thisObject)->EVENTNAME;\
					}\
				)\
			);\

#define CLASS_MEMBER_GUIEVENT_COMPOSITION(EVENTNAME)\
			AddEvent(\
				new GuiEventInfoImpl<GuiEventArgumentTypeRetriver<decltype(&GuiGraphicsEventReceiver::EVENTNAME)>::Type>(\
					this,\
					L#EVENTNAME,\
					[](DescriptableObject* thisObject, bool addEventHandler){\
						GuiGraphicsComposition* composition=dynamic_cast<GuiGraphicsComposition*>(thisObject);\
						if(!addEventHandler && !composition->HasEventReceiver())\
						{\
							return (GuiGraphicsEvent<GuiEventArgumentTypeRetriver<decltype(&GuiGraphicsEventReceiver::EVENTNAME)>::Type>*)0;\
						}\
						return &composition->GetEventReceiver()->EVENTNAME;\
					}\
				)\
			);\

#define CLASS_MEMBER_PROPERTY_GUIEVENT_FAST(PROPERTYNAME)\
			CLASS_MEMBER_GUIEVENT(PROPERTYNAME##Changed)\
			CLASS_MEMBER_PROPERTY_EVENT_FAST(PROPERTYNAME, PROPERTYNAME##Changed)\

#define CLASS_MEMBER_PROPERTY_GUIEVENT_READONLY_FAST(PROPERTYNAME)\
			CLASS_MEMBER_GUIEVENT(PROPERTYNAME##Changed)\
			CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(PROPERTYNAME, PROPERTYNAME##Changed)\

/***********************************************************************
Type Loader
***********************************************************************/

#endif

			extern bool						LoadGuiEventTypes();
		}
	}
}

#endif