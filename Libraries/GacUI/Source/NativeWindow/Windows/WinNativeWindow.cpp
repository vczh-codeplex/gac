#include "WinNativeWindow.h"
#include "ServicesImpl\WindowsResourceService.h"
#include "ServicesImpl\WindowsClipboardService.h"
#include "ServicesImpl\WindowsImageService.h"
#include "ServicesImpl\WindowsAsyncService.h"
#include "ServicesImpl\WindowsScreenService.h"
#include "ServicesImpl\WindowsCallbackService.h"
#include "ServicesImpl\WindowsInputService.h"
#include "ServicesImpl\WindowsDialogService.h"

#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "Shlwapi.lib")

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			using namespace collections;

			HWND GetHWNDFromNativeWindowHandle(INativeWindow* window)
			{
				if(!window) return NULL;
				IWindowsForm* form=GetWindowsForm(window);
				if(!form) return NULL;
				return form->GetWindowHandle();
			}

/***********************************************************************
WindowsClass
***********************************************************************/

			class WinClass : public Object
			{
			protected:
				WString									name;
				WNDCLASSEX								windowClass;
				ATOM									windowAtom;

			public:
				WinClass(WString _name, bool shadow, bool ownDC, WNDPROC procedure, HINSTANCE hInstance)
				{
					name=_name;
					windowClass.cbSize=sizeof(windowClass);
					windowClass.style=CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | (shadow?CS_DROPSHADOW:0) | (ownDC?CS_OWNDC:0);
					windowClass.lpfnWndProc=procedure;
					windowClass.cbClsExtra=0;
					windowClass.cbWndExtra=0;
					windowClass.hInstance=hInstance;
					windowClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
					windowClass.hCursor=NULL;//LoadCursor(NULL,IDC_ARROW);
					windowClass.hbrBackground=GetSysColorBrush(COLOR_BTNFACE);
					windowClass.lpszMenuName=NULL;
					windowClass.lpszClassName=name.Buffer();
					windowClass.hIconSm=NULL;
					windowAtom=RegisterClassEx(&windowClass);
				}

				bool IsAvailable()
				{
					return windowAtom!=0;
				}

				WString GetName()
				{
					return name;
				}

				ATOM GetClassAtom()
				{
					return windowAtom;
				}
			};

/***********************************************************************
WindowsForm
***********************************************************************/

			class WindowsForm : public Object, public INativeWindow, public IWindowsForm
			{
			protected:
				
				DWORD InternalGetExStyle()
				{
					return (DWORD)GetWindowLongPtr(handle,GWL_EXSTYLE);
				}

				void InternalSetExStyle(DWORD exStyle)
				{
					LONG_PTR result=SetWindowLongPtr(handle,GWL_EXSTYLE,exStyle);
					SetWindowPos(handle,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}

				bool GetExStyle(DWORD exStyle)
				{
					LONG_PTR Long=InternalGetExStyle();
					return (Long & exStyle) != 0;
				}

				void SetExStyle(DWORD exStyle, bool available)
				{
					DWORD Long=InternalGetExStyle();
					if(available)
					{
						Long|=exStyle;
					}
					else
					{
						Long&=~exStyle;
					}
					InternalSetExStyle((DWORD)Long);
				}

				bool GetStyle(DWORD style)
				{
					LONG_PTR Long=GetWindowLongPtr(handle,GWL_STYLE);
					return (Long & style) != 0;
				}

				void SetStyle(DWORD style, bool available)
				{
					LONG_PTR Long=GetWindowLongPtr(handle,GWL_STYLE);
					if(available)
					{
						Long|=style;
					}
					else
					{
						Long&=~style;
					}
					SetWindowLongPtr(handle,GWL_STYLE,Long);
					SetWindowPos(handle,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}

				NativeWindowMouseInfo ConvertMouse(WPARAM wParam, LPARAM lParam, bool wheelMessage, bool nonClient)
				{
					NativeWindowMouseInfo info;

					info.nonClient = false;
					if (nonClient)
					{
						switch (wParam)
						{
						case HTMINBUTTON:
						case HTMAXBUTTON:
						case HTCLOSE:
							break;
						default:
							info.nonClient = true;
							break;
						}
					}

					if(wheelMessage)
					{
						info.wheel=GET_WHEEL_DELTA_WPARAM(wParam);
						wParam=GET_KEYSTATE_WPARAM(wParam);
					}
					else
					{
						info.wheel=0;
					}

					if (nonClient)
					{
						info.ctrl = WinIsKeyPressing(VK_CONTROL);
						info.shift = WinIsKeyPressing(VK_SHIFT);
						info.left= WinIsKeyPressing(MK_LBUTTON);
						info.middle= WinIsKeyPressing(MK_MBUTTON);
						info.right = WinIsKeyPressing(MK_RBUTTON);
						
						POINTS point = MAKEPOINTS(lParam);
						Point offset = GetClientBoundsInScreen().LeftTop();
						info.x = point.x - offset.x;
						info.y = point.y - offset.y;
					}
					else
					{
						info.ctrl=(wParam & MK_CONTROL)!=0;
						info.shift=(wParam & MK_SHIFT)!=0;
						info.left=(wParam & MK_LBUTTON)!=0;
						info.middle=(wParam & MK_MBUTTON)!=0;
						info.right=(wParam & MK_RBUTTON)!=0;

						POINTS point = MAKEPOINTS(lParam);

						if (wheelMessage)
						{
							Point offset = GetClientBoundsInScreen().LeftTop();
							info.x = point.x - offset.x;
							info.y = point.y - offset.y;
						}
						else
						{
							info.x = point.x;
							info.y = point.y;
						}
					}
					return info;
				}

				NativeWindowKeyInfo ConvertKey(WPARAM wParam, LPARAM lParam)
				{
					NativeWindowKeyInfo info;
					info.code=wParam;
					info.ctrl=WinIsKeyPressing(VK_CONTROL);
					info.shift=WinIsKeyPressing(VK_SHIFT);
					info.alt=WinIsKeyPressing(VK_MENU);
					info.capslock=WinIsKeyToggled(VK_CAPITAL);
					return info;
				}

				NativeWindowCharInfo ConvertChar(WPARAM wParam)
				{
					NativeWindowCharInfo info;
					info.code=(wchar_t)wParam;
					info.ctrl=WinIsKeyPressing(VK_CONTROL);
					info.shift=WinIsKeyPressing(VK_SHIFT);
					info.alt=WinIsKeyPressing(VK_MENU);
					info.capslock=WinIsKeyToggled(VK_CAPITAL);
					return info;
				}

				void TrackMouse(bool enable)
				{
					TRACKMOUSEEVENT trackMouseEvent;
					trackMouseEvent.cbSize=sizeof(trackMouseEvent);
					trackMouseEvent.hwndTrack=handle;
					trackMouseEvent.dwFlags=(enable?0:TME_CANCEL) | TME_HOVER | TME_LEAVE;
					trackMouseEvent.dwHoverTime=HOVER_DEFAULT;
					TrackMouseEvent(&trackMouseEvent);
				}

				void UpdateCompositionForContent()
				{
					HIMC imc = ImmGetContext(handle);
					COMPOSITIONFORM cf;
					cf.dwStyle = CFS_POINT;
					cf.ptCurrentPos.x = (int)caretPoint.x;
					cf.ptCurrentPos.y = (int)caretPoint.y;
					ImmSetCompositionWindow(imc, &cf);
					ImmReleaseContext(handle, imc);
				}

				bool HandleMessageInternal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
				{
					bool transferFocusEvent = false;
					bool nonClient = false;

					switch(uMsg)
					{
					case WM_LBUTTONDOWN:
					case WM_LBUTTONUP:
					case WM_LBUTTONDBLCLK:
					case WM_RBUTTONDOWN:
					case WM_RBUTTONUP:
					case WM_RBUTTONDBLCLK:
					case WM_MBUTTONDOWN:
					case WM_MBUTTONUP:
					case WM_MBUTTONDBLCLK:
						transferFocusEvent=true;
					}
					switch(uMsg)
					{
					// ************************************** moving and sizing
					case WM_MOVING:case WM_SIZING:
						{
							LPRECT rawBounds=(LPRECT)lParam;
							Rect bounds(rawBounds->left, rawBounds->top, rawBounds->right, rawBounds->bottom);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->Moving(bounds, false);
							}
							if(		rawBounds->left!=bounds.Left()
								||	rawBounds->top!=bounds.Top()
								||	rawBounds->right!=bounds.Right()
								||	rawBounds->bottom!=bounds.Bottom())
							{
								rawBounds->left=(int)bounds.Left();
								rawBounds->top=(int)bounds.Top();
								rawBounds->right=(int)bounds.Right();
								rawBounds->bottom=(int)bounds.Bottom();
								result=TRUE;
							}
						}
						break;
					case WM_MOVE:case WM_SIZE:
						{
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->Moved();
							}
						}
						break;
					// ************************************** state
					case WM_ENABLE:
						{
							for(vint i=0;i<listeners.Count();i++)
							{
								if(wParam==TRUE)
								{
									listeners[i]->Enabled();
								}
								else
								{
									listeners[i]->Disabled();
								}
							}
						}
						break;
					case WM_SETFOCUS:
						{
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->GotFocus();
							}
						}
						break;
					case WM_KILLFOCUS:
						{
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->LostFocus();
							}
						}
						break;
					case WM_ACTIVATE:
						{
							for(vint i=0;i<listeners.Count();i++)
							{
								if(wParam==WA_ACTIVE || wParam==WA_CLICKACTIVE)
								{
									listeners[i]->Activated();
								}
								else
								{
									listeners[i]->Deactivated();
								}
							}
						}
						break;
					case WM_SHOWWINDOW:
						{
							if(wParam==TRUE)
							{
								for(vint i=0;i<listeners.Count();i++)
								{
									listeners[i]->Opened();
								}
							}
							else
							{
								for(vint i=0;i<listeners.Count();i++)
								{
									listeners[i]->Closed();
								}
							}
						}
						break;
					case WM_CLOSE:
						{
							bool cancel=false;
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->Closing(cancel);
							}
							return cancel;
						}
						break;
					// ************************************** mouse
					case WM_NCLBUTTONDOWN:
						if (!customFrameMode) break;
						nonClient = true;
					case WM_LBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false, nonClient);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonDown(info);
							}
						}
						break;
					case WM_NCLBUTTONUP:
						if (!customFrameMode) break;
						nonClient = true;
					case WM_LBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false, nonClient);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonUp(info);
							}
						}
						break;
					case WM_NCLBUTTONDBLCLK:
						if (!customFrameMode) break;
						nonClient = true;
					case WM_LBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false, nonClient);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonDoubleClick(info);
							}
						}
						break;
					case WM_NCRBUTTONDOWN:
						if (!customFrameMode) break;
						nonClient = true;
					case WM_RBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false, nonClient);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonDown(info);
							}
						}
						break;
					case WM_NCRBUTTONUP:
						if (!customFrameMode) break;
						nonClient = true;
					case WM_RBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false, nonClient);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonUp(info);
							}
						}
						break;
					case WM_NCRBUTTONDBLCLK:
						if (!customFrameMode) break;
						nonClient = true;
					case WM_RBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false, nonClient);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonDoubleClick(info);
							}
						}
						break;
					case WM_NCMBUTTONDOWN:
						if (!customFrameMode) break;
						nonClient = true;
					case WM_MBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false, nonClient);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonDown(info);
							}
						}
						break;
					case WM_NCMBUTTONUP:
						if (!customFrameMode) break;
						nonClient = true;
					case WM_MBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false, nonClient);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonUp(info);
							}
						}
						break;
					case WM_NCMBUTTONDBLCLK:
						if (!customFrameMode) break;
						nonClient = true;
					case WM_MBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false, nonClient);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonDoubleClick(info);
							}
						}
						break;
					case WM_NCMOUSEMOVE:
						if (!customFrameMode) break;
						nonClient = true;
					case WM_MOUSEMOVE:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false, nonClient);
							if(info.x!=mouseLastX || info.y!=mouseLastY)
							{
								if(!mouseHoving)
								{
									mouseHoving=true;
									for(vint i=0;i<listeners.Count();i++)
									{
										listeners[i]->MouseEntered();
									}
									TrackMouse(true);
								}
								for(vint i=0;i<listeners.Count();i++)
								{
									listeners[i]->MouseMoving(info);
								}
							}
						}
						break;
					// ************************************** wheel
					case WM_MOUSEHWHEEL:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, true, false);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->HorizontalWheel(info);
							}
						}
						break;
					case WM_MOUSEWHEEL:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, true, false);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->VerticalWheel(info);
							}
						}
						break;
					// ************************************** mouse state
					case WM_NCMOUSELEAVE:
						nonClient = true;
					case WM_MOUSELEAVE:
						if (customFrameMode == nonClient)
						{
							mouseLastX=-1;
							mouseLastY=-1;
							mouseHoving=false;
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->MouseLeaved();
							}
						}
						break;
					case WM_NCMOUSEHOVER:
					case WM_MOUSEHOVER:
						{
							TrackMouse(true);
						}
						break;
					// ************************************** key
					case WM_KEYUP:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->KeyUp(info);
							}
						}
						break;
					case WM_KEYDOWN:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->KeyDown(info);
							}
						}
						break;
					case WM_SYSKEYUP:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							if (supressingAlt && !info.ctrl && !info.shift && info.code == VK_MENU)
							{
								supressingAlt = false;
								break;
							}
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->SysKeyUp(info);
							}
						}
						break;
					case WM_SYSKEYDOWN:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							if (supressingAlt && !info.ctrl && !info.shift && info.code == VK_MENU)
							{
								break;
							}
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->SysKeyDown(info);
							}
						}
						break;
					case WM_CHAR:
						{
							NativeWindowCharInfo info=ConvertChar(wParam);
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->Char(info);
							}
						}
						break;
					// ************************************** painting
					case WM_PAINT:
						{
							for(vint i=0;i<listeners.Count();i++)
							{
								listeners[i]->Paint();
							}
						}
						break;
					case WM_ERASEBKGND:
						result = 0;
						return true;
					case WM_NCPAINT:
					case WM_SYNCPAINT:
						if(customFrameMode)
						{
							result=0;
							return true;
						}
						break;
					// ************************************** IME
					case WM_IME_SETCONTEXT:
						if(wParam==TRUE)
						{
							HIMC imc = ImmGetContext(handle);
							ImmAssociateContext(hwnd, imc);
							ImmReleaseContext(handle, imc);
						}
						break;
					case WM_IME_STARTCOMPOSITION:
						UpdateCompositionForContent();
						break;
					// ************************************** hit test
					case WM_NCHITTEST:
						{
							POINTS location=MAKEPOINTS(lParam);
							Point windowLocation=GetBounds().LeftTop();
							location.x-=(SHORT)windowLocation.x;
							location.y-=(SHORT)windowLocation.y;
							for(vint i=0;i<listeners.Count();i++)
							{
								switch(listeners[i]->HitTest(Point(location.x, location.y)))
								{
								case INativeWindowListener::BorderNoSizing:
									result=HTBORDER;
									return true;
								case INativeWindowListener::BorderLeft:
									result=HTLEFT;
									return true;
								case INativeWindowListener::BorderRight:
									result=HTRIGHT;
									return true;
								case INativeWindowListener::BorderTop:
									result=HTTOP;
									return true;
								case INativeWindowListener::BorderBottom:
									result=HTBOTTOM;
									return true;
								case INativeWindowListener::BorderLeftTop:
									result=HTTOPLEFT;
									return true;
								case INativeWindowListener::BorderRightTop:
									result=HTTOPRIGHT;
									return true;
								case INativeWindowListener::BorderLeftBottom:
									result=HTBOTTOMLEFT;
									return true;
								case INativeWindowListener::BorderRightBottom:
									result=HTBOTTOMRIGHT;
									return true;
								case INativeWindowListener::Title:
									result=HTCAPTION;
									return true;
								case INativeWindowListener::ButtonMinimum:
									result=HTMINBUTTON;
									return true;
								case INativeWindowListener::ButtonMaximum:
									result=HTMAXBUTTON;
									return true;
								case INativeWindowListener::ButtonClose:
									result=HTCLOSE;
									return true;
								case INativeWindowListener::Client:
									result=HTCLIENT;
									return true;
								case INativeWindowListener::Icon:
									result=HTSYSMENU;
									return true;
								}
							}
						}
						break;
					// ************************************** MISC
					case WM_SETCURSOR:
						{
							DWORD hitTestResult=LOWORD(lParam);
							if(hitTestResult==HTCLIENT)
							{
								HCURSOR cursorHandle=cursor->GetCursorHandle();
								if(GetCursor()!=cursorHandle)
								{
									SetCursor(cursorHandle);
								}
								result=TRUE;
								return true;
							}
						}
						break;
					case WM_NCCALCSIZE:
						if((BOOL)wParam && customFrameMode)
						{
							result=0;
							return true;
						}
						break;
					case WM_NCACTIVATE:
						if(customFrameMode)
						{
							if(wParam==TRUE)
							{
								result=FALSE;
							}
							else
							{
								result=TRUE;
							}
							return true;
						}
						break;
					}

					if(IsWindow(hwnd)!=0)
					{
						if(transferFocusEvent && IsFocused())
						{
							WindowsForm* window=this;
							while(window->parentWindow && window->alwaysPassFocusToParent)
							{
								window=window->parentWindow;
							}
							if(window!=this)
							{
								window->SetFocus();
							}
						}
					}

					if (customFrameMode)
					{
						switch (uMsg)
						{
						case WM_NCLBUTTONDOWN:
							switch (wParam)
							{
							case HTMINBUTTON:
							case HTMAXBUTTON:
							case HTCLOSE:
								result = 0;
								return true;
							}
							break;
						case WM_LBUTTONUP:
							{
								POINTS location = MAKEPOINTS(lParam);
								for(vint i=0;i<listeners.Count();i++)
								{
									switch(listeners[i]->HitTest(Point(location.x, location.y)))
									{
									case INativeWindowListener::ButtonMinimum:
										ShowMinimized();
										return false;
									case INativeWindowListener::ButtonMaximum:
										if (GetSizeState() == INativeWindow::Maximized)
										{
											ShowRestored();
										}
										else
										{
											ShowMaximized();
										}
										return false;
									case INativeWindowListener::ButtonClose:
										Hide();
										return false;
									}
								}
							}
							break;
						}
					}
					return false;
				}
			protected:
				HWND								handle;
				WString								title;
				WindowsCursor*						cursor;
				Point								caretPoint;
				WindowsForm*						parentWindow;
				bool								alwaysPassFocusToParent;
				List<INativeWindowListener*>		listeners;
				vint								mouseLastX;
				vint								mouseLastY;
				vint								mouseHoving;
				Interface*							graphicsHandler;
				bool								customFrameMode;
				List<Ptr<INativeMessageHandler>>	messageHandlers;
				bool								supressingAlt;

			public:
				WindowsForm(HWND parent, WString className, HINSTANCE hInstance)
					:cursor(0)
					,parentWindow(0)
					,alwaysPassFocusToParent(false)
					,mouseLastX(-1)
					,mouseLastY(-1)
					,mouseHoving(false)
					,graphicsHandler(0)
					,customFrameMode(false)
					,supressingAlt(false)
				{
					DWORD exStyle=WS_EX_APPWINDOW | WS_EX_CONTROLPARENT;
					DWORD style=WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
					handle=CreateWindowEx(exStyle, className.Buffer(), L"", style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, NULL, hInstance, NULL);
				}

				~WindowsForm()
				{
					List<INativeWindowListener*> copiedListeners;
					CopyFrom(copiedListeners, listeners);
					for(vint i=0;i<copiedListeners.Count();i++)
					{
						INativeWindowListener* listener=copiedListeners[i];
						if(listeners.Contains(listener))
						{
							listener->Destroyed();
						}
					}
					DestroyWindow(handle);
				}

				void InvokeDestroying()
				{
					for(vint i=0;i<listeners.Count();i++)
					{
						listeners[i]->Destroying();
					}
				}

				bool HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
				{
					bool skip = false;
					{
						FOREACH(Ptr<INativeMessageHandler>, handler, messageHandlers)
						{
							handler->BeforeHandle(hwnd, uMsg, wParam, lParam, skip);
						}
						if (skip)
						{
							return true;
						}
					}
					skip = HandleMessageInternal(hwnd, uMsg, wParam, lParam, result);
					if (GetWindowsFormFromHandle(hwnd))
					{
						FOREACH(Ptr<INativeMessageHandler>, handler, messageHandlers)
						{
							handler->AfterHandle(hwnd, uMsg, wParam, lParam, skip, result);
						}
					}
					return skip;
				}

				HWND GetWindowHandle()override
				{
					return handle;
				}

				Interface* GetGraphicsHandler()override
				{
					return graphicsHandler;
				}

				void SetGraphicsHandler(Interface* handler)override
				{
					graphicsHandler=handler;
				}

				bool InstallMessageHandler(Ptr<INativeMessageHandler> handler)override
				{
					if (messageHandlers.Contains(handler.Obj()))
					{
						return false;
					}
					messageHandlers.Add(handler);
					return true;
				}

				bool UninstallMessageHandler(Ptr<INativeMessageHandler> handler)override
				{
					vint index = messageHandlers.IndexOf(handler.Obj());
					if (index == -1)return false;
					messageHandlers.RemoveAt(handler);
					return true;
				}

				Rect GetBounds()
				{
					RECT rect;
					GetWindowRect(handle, &rect);
					return Rect(rect.left, rect.top, rect.right, rect.bottom);
				}

				void SetBounds(const Rect& bounds)
				{
					Rect newBounds=bounds;
					for(vint i=0;i<listeners.Count();i++)
					{
						listeners[i]->Moving(newBounds, true);
					}
					MoveWindow(handle, (int)newBounds.Left(), (int)newBounds.Top(), (int)newBounds.Width(), (int)newBounds.Height(), FALSE);
				}

				Size GetClientSize()
				{
					return GetClientBoundsInScreen().GetSize();
				}

				void SetClientSize(Size size)
				{
					RECT required={0,0,(int)size.x,(int)size.y};
					RECT bounds;
					GetWindowRect(handle, &bounds);
					AdjustWindowRect(&required, (DWORD)GetWindowLongPtr(handle, GWL_STYLE), FALSE);
					SetBounds(Rect(Point(bounds.left, bounds.top), Size(required.right-required.left, required.bottom-required.top)));
				}

				Rect GetClientBoundsInScreen()
				{
					if(customFrameMode)
					{
						return GetBounds();
					}
					else
					{
						RECT required={0,0,0,0};
						RECT bounds;
						GetWindowRect(handle, &bounds);
						AdjustWindowRect(&required, (DWORD)GetWindowLongPtr(handle, GWL_STYLE), FALSE);
						return Rect(
							Point(
								(bounds.left-required.left),
								(bounds.top-required.top)
								),
							Size(
								(bounds.right-bounds.left)-(required.right-required.left),
								(bounds.bottom-bounds.top)-(required.bottom-required.top)
								)
							);
					}
				}

				WString GetTitle()
				{
					return title;
				}

				void SetTitle(WString _title)
				{
					title=_title;
					SetWindowText(handle, title.Buffer());
				}

				INativeCursor* GetWindowCursor()
				{
					return cursor;
				}

				void SetWindowCursor(INativeCursor* _cursor)
				{
					WindowsCursor* newCursor=dynamic_cast<WindowsCursor*>(_cursor);
					if(newCursor && cursor!=newCursor)
					{
						cursor=newCursor;
						if(mouseHoving && IsVisible())
						{
							SetCursor(cursor->GetCursorHandle());
						}
					}
				}
				
				Point GetCaretPoint()
				{
					return caretPoint;
				}

				void SetCaretPoint(Point point)
				{
					caretPoint=point;
					UpdateCompositionForContent();
				}

				INativeWindow* GetParent()
				{
					return parentWindow;
				}

				void SetParent(INativeWindow* parent)
				{
					parentWindow=dynamic_cast<WindowsForm*>(parent);
					if(parentWindow)
					{
						SetWindowLongPtr(handle, GWLP_HWNDPARENT, (LONG_PTR)parentWindow->handle);
					}
					else
					{
						SetWindowLongPtr(handle, GWLP_HWNDPARENT, NULL);
					}
				}

				bool GetAlwaysPassFocusToParent()
				{
					return alwaysPassFocusToParent;
				}

				void SetAlwaysPassFocusToParent(bool value)
				{
					alwaysPassFocusToParent=value;
				}

				void EnableCustomFrameMode()
				{
					customFrameMode=true;
				}

				void DisableCustomFrameMode()
				{
					customFrameMode=false;
				}

				bool IsCustomFrameModeEnabled()
				{
					return customFrameMode;
				}

				WindowSizeState GetSizeState()
				{
					if(IsIconic(handle))
					{
						return INativeWindow::Minimized;
					}
					else if(IsZoomed(handle))
					{
						return INativeWindow::Maximized;
					}
					else
					{
						return INativeWindow::Restored;
					}
				}

				void Show()
				{
					ShowWindow(handle, SW_SHOWNORMAL);
				}

				void ShowDeactivated()
				{
					ShowWindow(handle, SW_SHOWNOACTIVATE);
					SetWindowPos(handle,HWND_TOP,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
				}

				void ShowRestored()
				{
					ShowWindow(handle, SW_RESTORE);
				}

				void ShowMaximized()
				{
					ShowWindow(handle, SW_SHOWMAXIMIZED);
				}

				void ShowMinimized()
				{
					ShowWindow(handle, SW_SHOWMINIMIZED);
				}

				void Hide()
				{
					SendMessage(handle, WM_CLOSE, NULL, NULL);
				}

				bool IsVisible()
				{
					return IsWindowVisible(handle)!=0;
				}

				void Enable()
				{
					EnableWindow(handle, TRUE);
				}

				void Disable()
				{
					EnableWindow(handle, FALSE);
				}

				bool IsEnabled()
				{
					return IsWindowEnabled(handle)!=0;
				}

				void SetFocus()
				{
					::SetFocus(handle);
				}

				bool IsFocused()
				{
					return GetFocus()==handle;
				}

				void SetActivate()
				{
					SetActiveWindow(handle);
				}

				bool IsActivated()
				{
					return GetActiveWindow()==handle;
				}

				void ShowInTaskBar()
				{
					SetExStyle(WS_EX_APPWINDOW, true);
				}

				void HideInTaskBar()
				{
					SetExStyle(WS_EX_APPWINDOW, false);
				}

				bool IsAppearedInTaskBar()
				{
					return GetExStyle(WS_EX_APPWINDOW);
				}

				void EnableActivate()
				{
					SetExStyle(WS_EX_NOACTIVATE, false);
				}

				void DisableActivate()
				{
					SetExStyle(WS_EX_NOACTIVATE, true);
				}

				bool IsEnabledActivate()
				{
					return !GetExStyle(WS_EX_NOACTIVATE);
				}

				bool RequireCapture()
				{
					SetCapture(handle);
					return true;
				}

				bool ReleaseCapture()
				{
					::ReleaseCapture();
					return true;
				}

				bool IsCapturing()
				{
					return GetCapture()==handle;
				}

				bool GetMaximizedBox()
				{
					return GetStyle(WS_MAXIMIZEBOX);
				}

				void SetMaximizedBox(bool visible)
				{
					SetStyle(WS_MAXIMIZEBOX, visible);
				}

				bool GetMinimizedBox()
				{
					return GetStyle(WS_MINIMIZEBOX);
				}

				void SetMinimizedBox(bool visible)
				{
					SetStyle(WS_MINIMIZEBOX, visible);
				}

				bool GetBorder()
				{
					return GetStyle(WS_BORDER);
				}

				void SetBorder(bool visible)
				{
					SetStyle(WS_BORDER, visible);
				}

				bool GetSizeBox()
				{
					return GetStyle(WS_SIZEBOX);
				}

				void SetSizeBox(bool visible)
				{
					SetStyle(WS_SIZEBOX, visible);
				}

				bool GetIconVisible()
				{
					return GetStyle(WS_SYSMENU);
				}

				void SetIconVisible(bool visible)
				{
					SetStyle(WS_SYSMENU, visible);
				}

				bool GetTitleBar()
				{
					return GetStyle(WS_CAPTION);
				}

				void SetTitleBar(bool visible)
				{
					SetStyle(WS_CAPTION, visible);
				}

				bool GetTopMost()
				{
					return GetExStyle(WS_EX_TOPMOST);
				}

				void SetTopMost(bool topmost)
				{
					SetWindowPos(handle, (topmost ? HWND_TOPMOST : HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
				}

				void SupressAlt()
				{
					if (!supressingAlt)
					{
						supressingAlt = true;
						PostMessage(handle, WM_SYSKEYDOWN, VK_MENU, 0);
						PostMessage(handle, WM_SYSKEYUP, VK_MENU, 0);
					}
				}

				bool InstallListener(INativeWindowListener* listener)
				{
					if(listeners.Contains(listener))
					{
						return false;
					}
					else
					{
						listeners.Add(listener);
						return true;
					}
				}

				bool UninstallListener(INativeWindowListener* listener)
				{
					if(listeners.Contains(listener))
					{
						listeners.Remove(listener);
						return true;
					}
					else
					{
						return false;
					}
				}

				void RedrawContent()
				{
					if(graphicsHandler)
					{
						SendMessage(this->handle, WM_PAINT, NULL, NULL);
					}
				}
			};

/***********************************************************************
WindowsController
***********************************************************************/

			LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			LRESULT CALLBACK GodProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			LRESULT CALLBACK MouseProc(int nCode , WPARAM wParam , LPARAM lParam);

			class WindowsController : public Object, public virtual INativeController, public virtual INativeWindowService
			{
			protected:
				WinClass							windowClass;
				WinClass							godClass;
				HINSTANCE							hInstance;
				HWND								godWindow;
				Dictionary<HWND, WindowsForm*>		windows;
				INativeWindow*						mainWindow;
				HWND								mainWindowHandle;

				WindowsCallbackService				callbackService;
				WindowsResourceService				resourceService;
				WindowsAsyncService					asyncService;
				WindowsClipboardService				clipboardService;
				WindowsImageService					imageService;
				WindowsScreenService				screenService;
				WindowsInputService					inputService;
				WindowsDialogService				dialogService;

			public:
				WindowsController(HINSTANCE _hInstance)
					:hInstance(_hInstance)
					,windowClass(L"VczhWindow", false, false, WndProc, _hInstance)
					,godClass(L"GodWindow", false, false, GodProc, _hInstance)
					,mainWindow(0)
					,mainWindowHandle(0)
					,screenService(&GetHWNDFromNativeWindowHandle)
					,inputService(&MouseProc)
					,dialogService(&GetHWNDFromNativeWindowHandle)
				{
					godWindow=CreateWindowEx(WS_EX_CONTROLPARENT, godClass.GetName().Buffer(), L"GodWindow", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
					clipboardService.SetOwnerHandle(godWindow);
					inputService.SetOwnerHandle(godWindow);
				}

				~WindowsController()
				{
					inputService.StopTimer();
					inputService.StopHookMouse();
					clipboardService.SetOwnerHandle(NULL);
					DestroyWindow(godWindow);
				}

				WindowsForm* GetWindowsFormFromHandle(HWND hwnd)
				{
					vint index = windows.Keys().IndexOf(hwnd);
					if (index == -1)return 0;
					return windows.Values()[index];
				}

				bool HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
				{
					bool skipDefaultProcedure=false;
					vint index=windows.Keys().IndexOf(hwnd);
					if(index!=-1)
					{
						WindowsForm* window=windows.Values().Get(index);
						skipDefaultProcedure=window->HandleMessage(hwnd, uMsg, wParam, lParam, result);
						switch(uMsg)
						{
						case WM_CLOSE:
							if(!skipDefaultProcedure)
							{
								ShowWindow(window->GetWindowHandle(), SW_HIDE);
								if(window!=mainWindow)
								{
									skipDefaultProcedure=true;
								}
							}
							break;
						case WM_DESTROY:
							DestroyNativeWindow(window);
							break;
						}
					}

					if(hwnd==mainWindowHandle && uMsg==WM_DESTROY)
					{
						for(vint i=0;i<windows.Count();i++)
						{
							if(windows.Values().Get(i)->IsVisible())
							{
								windows.Values().Get(i)->Hide();
							}
						}
						while(windows.Count())
						{
							DestroyNativeWindow(windows.Values().Get(0));
						}
						PostQuitMessage(0);
					}
					return skipDefaultProcedure;
				}

				//=======================================================================

				INativeWindow* CreateNativeWindow()
				{
					WindowsForm* window=new WindowsForm(godWindow, windowClass.GetName(), hInstance);
					windows.Add(window->GetWindowHandle(), window);
					callbackService.InvokeNativeWindowCreated(window);
					window->SetWindowCursor(resourceService.GetDefaultSystemCursor());
					return window;
				}

				void DestroyNativeWindow(INativeWindow* window)
				{
					WindowsForm* windowsForm=dynamic_cast<WindowsForm*>(window);
					windowsForm->InvokeDestroying();
					if(windowsForm!=0 && windows.Keys().Contains(windowsForm->GetWindowHandle()))
					{
						callbackService.InvokeNativeWindowDestroyed(window);
						windows.Remove(windowsForm->GetWindowHandle());
						delete windowsForm;
					}
				}

				INativeWindow* GetMainWindow()
				{
					return mainWindow;
				}

				void Run(INativeWindow* window)
				{
					mainWindow=window;
					mainWindowHandle=GetWindowsForm(window)->GetWindowHandle();
					mainWindow->Show();
					MSG message;
					while(GetMessage(&message, NULL, 0, 0))
					{
						TranslateMessage(&message);
						DispatchMessage(&message);
						asyncService.ExecuteAsyncTasks();
					}
				}

				INativeWindow* GetWindow(Point location)
				{
					POINT p;
					p.x=(int)location.x;
					p.y=(int)location.y;
					HWND handle=WindowFromPoint(p);
					vint index=windows.Keys().IndexOf(handle);
					if(index==-1)
					{
						return 0;
					}
					else
					{
						return windows.Values().Get(index);
					}
				}

				//=======================================================================

				INativeCallbackService* CallbackService()
				{
					return &callbackService;
				}

				INativeResourceService* ResourceService()
				{
					return &resourceService;
				}
				
				INativeAsyncService* AsyncService()
				{
					return &asyncService;
				}

				INativeClipboardService* ClipboardService()
				{
					return &clipboardService;
				}

				INativeImageService* ImageService()
				{
					return &imageService;
				}

				INativeScreenService* ScreenService()
				{
					return &screenService;
				}

				INativeWindowService* WindowService()
				{
					return this;
				}

				INativeInputService* InputService()
				{
					return &inputService;
				}

				INativeDialogService* DialogService()
				{
					return &dialogService;
				}

				bool IsWindowsVersionEqualOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
				{
					OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0 };
					DWORDLONG const dwlConditionMask = 
						VerSetConditionMask(
							VerSetConditionMask(
								VerSetConditionMask(
									0,
									VER_MAJORVERSION,
									VER_GREATER_EQUAL
									),
								VER_MINORVERSION,
								VER_GREATER_EQUAL
								),
							VER_SERVICEPACKMAJOR,
							VER_GREATER_EQUAL
							);

					osvi.dwMajorVersion = wMajorVersion;
					osvi.dwMinorVersion = wMinorVersion;
					osvi.wServicePackMajor = wServicePackMajor;

					return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
				}

				bool IsWindowsServer()
				{
					OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0, 0, VER_NT_WORKSTATION };
					DWORDLONG const dwlConditionMask = VerSetConditionMask( 0, VER_PRODUCT_TYPE, VER_EQUAL );

					return !VerifyVersionInfoW(&osvi, VER_PRODUCT_TYPE, dwlConditionMask);
				}

				WString GetOSVersion()
				{
					const vint SystemCount = 5;
					DWORD SystemVersions[SystemCount] = 
					{
						_WIN32_WINNT_WIN2K,
						_WIN32_WINNT_WINXP,
						_WIN32_WINNT_VISTA,
						_WIN32_WINNT_WIN7,
						_WIN32_WINNT_WIN8
					};
					vint SystemMaxServerPacks[SystemCount] =
					{
						4,
						3,
						2,
						1,
						0,
					};
					const wchar_t* SystemClientNames[SystemCount] =
					{
						L"Windows 2000",
						L"Windows XP",
						L"Windows Vista",
						L"Windows 7",
						L"Windows 8",
					};
					const wchar_t* SystemServerNames[SystemCount] =
					{
						L"Windows Server 2003",
						L"Windows Server 2003 R2",
						L"Windows Server 2008",
						L"Windows Server 2008 R2",
						L"Windows Server 2012",
					};

					bool isWindowsServer = IsWindowsServer();
					for(vint systemIndex = SystemCount-1; systemIndex >=0; systemIndex--)
					{
						DWORD systemVersion = SystemVersions[systemIndex];
						vint maxSp = SystemMaxServerPacks[systemIndex];
						for(vint sp = maxSp; sp>=0; sp--)
						{
							if(IsWindowsVersionEqualOrGreater(HIBYTE(systemVersion), LOBYTE(systemVersion), (WORD)sp))
							{
								WString systemName = isWindowsServer?SystemServerNames[systemIndex]:SystemClientNames[systemIndex];
								if(sp==0)
								{
									return systemName+L";";
								}
								else
								{
									return systemName+L";SP"+itow(sp);
								}
							}
						}
					}
					return L"Windows <Unknown Version>";
				}

				WString GetExecutablePath()
				{
					Array<wchar_t> buffer(65536);
					GetModuleFileName(NULL, &buffer[0], (DWORD)buffer.Count());
					return &buffer[0];
				}

				//=======================================================================

				void InvokeMouseHook(WPARAM message, Point location)
				{
					callbackService.InvokeMouseHook(message, location);
				}

				void InvokeGlobalTimer()
				{
					callbackService.InvokeGlobalTimer();
				}

				void InvokeClipboardUpdated()
				{
					callbackService.InvokeClipboardUpdated();
				}
			};

/***********************************************************************
Windows Procedure
***********************************************************************/

			LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				WindowsController* controller=dynamic_cast<WindowsController*>(GetCurrentController());
				if(controller)
				{
					LRESULT result=0;
					if(controller->HandleMessage(hwnd, uMsg, wParam, lParam, result))
					{
						return result;
					}
				}
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}

			LRESULT CALLBACK GodProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				WindowsController* controller=dynamic_cast<WindowsController*>(GetCurrentController());
				if(controller)
				{
					switch(uMsg)
					{
					case WM_TIMER:
						controller->InvokeGlobalTimer();
						break;
					case WM_CLIPBOARDUPDATE:
						controller->InvokeClipboardUpdated();
						break;
					}
				}
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}

			LRESULT CALLBACK MouseProc(int nCode , WPARAM wParam , LPARAM lParam)
			{
				WindowsController* controller=dynamic_cast<WindowsController*>(GetCurrentController());
				if(controller)
				{
					MSLLHOOKSTRUCT* mouseHookStruct=(MSLLHOOKSTRUCT*)lParam;
					Point location(mouseHookStruct->pt.x, mouseHookStruct->pt.y);
					controller->InvokeMouseHook(wParam, location);
				}
				return CallNextHookEx(NULL,nCode,wParam,lParam);
			}

/***********************************************************************
Windows Platform Native Controller
***********************************************************************/

			INativeController* CreateWindowsNativeController(HINSTANCE hInstance)
			{
				return new WindowsController(hInstance);
			}

			IWindowsForm* GetWindowsFormFromHandle(HWND hwnd)
			{
				auto controller = dynamic_cast<WindowsController*>(GetCurrentController());
				if (controller)
				{
					return controller->GetWindowsFormFromHandle(hwnd);
				}
				return 0;
			}

			IWindowsForm* GetWindowsForm(INativeWindow* window)
			{
				return dynamic_cast<WindowsForm*>(window);
			}

			void DestroyWindowsNativeController(INativeController* controller)
			{
				delete controller;
			}

			void EnableCrossKernelCrashing()
			{
				typedef BOOL (WINAPI *tGetPolicy)(LPDWORD lpFlags); 
				typedef BOOL (WINAPI *tSetPolicy)(DWORD dwFlags); 
				const DWORD EXCEPTION_SWALLOWING = 0x1;
 
				HMODULE kernel32 = LoadLibrary(L"kernel32.dll"); 
				tGetPolicy pGetPolicy = (tGetPolicy)GetProcAddress(kernel32, "GetProcessUserModeExceptionPolicy"); 
				tSetPolicy pSetPolicy = (tSetPolicy)GetProcAddress(kernel32, "SetProcessUserModeExceptionPolicy"); 
				if (pGetPolicy && pSetPolicy) 
				{ 
					DWORD dwFlags; 
					if (pGetPolicy(&dwFlags)) 
					{ 
						// Turn off the filter 
						pSetPolicy(dwFlags & ~EXCEPTION_SWALLOWING);
					} 
				} 
			}
		}
	}
}