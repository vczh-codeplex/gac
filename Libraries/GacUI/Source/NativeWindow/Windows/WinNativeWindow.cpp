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
				WString					name;
				WNDCLASSEX				windowClass;
				ATOM					windowAtom;
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
					return GetWindowLongPtr(handle,GWL_EXSTYLE);
				}

				void InternalSetExStyle(DWORD exStyle)
				{
					LONG result=SetWindowLongPtr(handle,GWL_EXSTYLE,exStyle);
					SetWindowPos(handle,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}

				bool GetExStyle(DWORD exStyle)
				{
					LONG_PTR Long=InternalGetExStyle();
					return (Long & exStyle) != 0;
				}

				void SetExStyle(DWORD exStyle, bool available)
				{
					LONG_PTR Long=InternalGetExStyle();
					if(available)
					{
						Long|=exStyle;
					}
					else
					{
						Long&=~exStyle;
					}
					InternalSetExStyle(Long);
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

				NativeWindowMouseInfo ConvertMouse(WPARAM wParam, LPARAM lParam, bool wheelMessage)
				{
					NativeWindowMouseInfo info;
					if(wheelMessage)
					{
						info.wheel=GET_WHEEL_DELTA_WPARAM(wParam);
						wParam=GET_KEYSTATE_WPARAM(wParam);
					}
					else
					{
						info.wheel=0;
					}
					info.ctrl=(wParam & MK_CONTROL)!=0;
					info.shift=(wParam & MK_SHIFT)!=0;
					info.left=(wParam & MK_LBUTTON)!=0;
					info.middle=(wParam & MK_MBUTTON)!=0;
					info.right=(wParam & MK_RBUTTON)!=0;
					POINTS Point=MAKEPOINTS(lParam);
					info.x=Point.x;
					info.y=Point.y;
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
					info.code=wParam;
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
					cf.ptCurrentPos.x = caretPoint.x;
					cf.ptCurrentPos.y = caretPoint.y;
					ImmSetCompositionWindow(imc, &cf);
					ImmReleaseContext(handle, imc);
				}

				bool HandleMessageInternal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
				{
					bool transferFocusEvent=false;
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
					case WM_NCHITTEST:
						{
							POINTS location=MAKEPOINTS(lParam);
							for(int i=0;i<listeners.Count();i++)
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
								}
							}
						}
						break;
					case WM_MOVING:case WM_SIZING:
						{
							LPRECT rawBounds=(LPRECT)lParam;
							Rect bounds(rawBounds->left, rawBounds->top, rawBounds->right, rawBounds->bottom);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Moving(bounds, false);
							}
							if(		rawBounds->left!=bounds.Left()
								||	rawBounds->top!=bounds.Top()
								||	rawBounds->right!=bounds.Right()
								||	rawBounds->bottom!=bounds.Bottom())
							{
								rawBounds->left=bounds.Left();
								rawBounds->top=bounds.Top();
								rawBounds->right=bounds.Right();
								rawBounds->bottom=bounds.Bottom();
								result=TRUE;
							}
						}
						break;
					case WM_MOVE:case WM_SIZE:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Moved();
							}
						}
						break;
					case WM_ENABLE:
						{
							for(int i=0;i<listeners.Count();i++)
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
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->GotFocus();
							}
						}
						break;
					case WM_KILLFOCUS:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LostFocus();
							}
						}
						break;
					case WM_ACTIVATE:
						{
							for(int i=0;i<listeners.Count();i++)
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
								for(int i=0;i<listeners.Count();i++)
								{
									listeners[i]->Opened();
								}
							}
							else
							{
								for(int i=0;i<listeners.Count();i++)
								{
									listeners[i]->Closed();
								}
							}
						}
						break;
					case WM_CLOSE:
						{
							bool cancel=false;
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Closing(cancel);
							}
							return cancel;
						}
						break;
					case WM_LBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonDown(info);
							}
						}
						break;
					case WM_LBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonUp(info);
							}
						}
						break;
					case WM_LBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonDoubleClick(info);
							}
						}
						break;
					case WM_RBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonDown(info);
							}
						}
						break;
					case WM_RBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonUp(info);
							}
						}
						break;
					case WM_RBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonDoubleClick(info);
							}
						}
						break;
					case WM_MBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonDown(info);
							}
						}
						break;
					case WM_MBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonUp(info);
							}
						}
						break;
					case WM_MBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonDoubleClick(info);
							}
						}
						break;
					case WM_MOUSEHWHEEL:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, true);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->HorizontalWheel(info);
							}
						}
						break;
					case WM_MOUSEWHEEL:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, true);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->VerticalWheel(info);
							}
						}
						break;
					case WM_MOUSEMOVE:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							if(info.x!=mouseLastX || info.y!=mouseLastY)
							{
								if(!mouseHoving)
								{
									mouseHoving=true;
									for(int i=0;i<listeners.Count();i++)
									{
										listeners[i]->MouseEntered();
									}
									TrackMouse(true);
								}
								for(int i=0;i<listeners.Count();i++)
								{
									listeners[i]->MouseMoving(info);
								}
							}
							if(GetCursor()!=cursor->GetCursorHandle())
							{
								SetCursor(cursor->GetCursorHandle());
							}
						}
						break;
					case WM_MOUSELEAVE:
						{
							mouseLastX=-1;
							mouseLastY=-1;
							mouseHoving=false;
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MouseLeaved();
							}
						}
						break;
					case WM_MOUSEHOVER:
						{
							TrackMouse(true);
						}
						break;
					case WM_KEYUP:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->KeyUp(info);
							}
						}
						break;
					case WM_KEYDOWN:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->KeyDown(info);
							}
						}
						break;
					case WM_SYSKEYUP:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->SysKeyUp(info);
							}
						}
						break;
					case WM_SYSKEYDOWN:
						{
							NativeWindowKeyInfo info=ConvertKey(wParam, lParam);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->SysKeyDown(info);
							}
						}
						break;
					case WM_CHAR:
						{
							NativeWindowCharInfo info=ConvertChar(wParam);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Char(info);
							}
						}
						break;
					case WM_PAINT:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Paint();
							}
						}
						break;
					case WM_ERASEBKGND:
						return true;
					case WM_IME_STARTCOMPOSITION:
						UpdateCompositionForContent();
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
				int									mouseLastX;
				int									mouseLastY;
				int									mouseHoving;
				Interface*							graphicsHandler;
			public:
				WindowsForm(HWND parent, WString className, HINSTANCE hInstance)
					:cursor(0)
					,parentWindow(0)
					,alwaysPassFocusToParent(false)
					,mouseLastX(-1)
					,mouseLastY(-1)
					,mouseHoving(false)
					,graphicsHandler(0)
				{
					DWORD exStyle=WS_EX_APPWINDOW | WS_EX_CONTROLPARENT;
					DWORD style=WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
					handle=CreateWindowEx(exStyle, className.Buffer(), L"", style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, NULL, hInstance, NULL);
				}

				~WindowsForm()
				{
					List<INativeWindowListener*> copiedListeners;
					CopyFrom(copiedListeners.Wrap(), listeners.Wrap());
					for(int i=0;i<copiedListeners.Count();i++)
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
					for(int i=0;i<listeners.Count();i++)
					{
						listeners[i]->Destroying();
					}
				}

				bool HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
				{
					return HandleMessageInternal(hwnd, uMsg, wParam, lParam, result);
				}

				HWND GetWindowHandle()
				{
					return handle;
				}

				Interface* GetGraphicsHandler()
				{
					return graphicsHandler;
				}

				void SetGraphicsHandler(Interface* handler)
				{
					graphicsHandler=handler;
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
					for(int i=0;i<listeners.Count();i++)
					{
						listeners[i]->Moving(newBounds, true);
					}
					MoveWindow(handle, newBounds.Left(), newBounds.Top(), newBounds.Width(), newBounds.Height(), FALSE);
				}

				Size GetClientSize()
				{
					return GetClientBoundsInScreen().GetSize();
				}

				void SetClientSize(Size size)
				{
					RECT required={0,0,size.x,size.y};
					RECT bounds;
					GetWindowRect(handle, &bounds);
					AdjustWindowRect(&required, GetWindowLongPtr(handle, GWL_STYLE), FALSE);
					SetBounds(Rect(Point(bounds.left, bounds.top), Size(required.right-required.left, required.bottom-required.top)));
				}

				Rect GetClientBoundsInScreen()
				{
					RECT required={0,0,0,0};
					RECT bounds;
					GetWindowRect(handle, &bounds);
					AdjustWindowRect(&required, GetWindowLongPtr(handle, GWL_STYLE), FALSE);
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
					WindowsForm* window=dynamic_cast<WindowsForm*>(parent);
					if(window)
					{
						parentWindow=window;
						if(parentWindow)
						{
							SetWindowLongPtr(handle, GWL_HWNDPARENT, (LONG_PTR)window->handle);
						}
						else
						{
							SetWindowLongPtr(handle, GWL_HWNDPARENT, NULL);
						}
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
					SetWindowPos(handle,(topmost?HWND_TOPMOST:HWND_NOTOPMOST),0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
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

				bool HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
				{
					bool skipDefaultProcedure=false;
					int index=windows.Keys().IndexOf(hwnd);
					if(index!=-1)
					{
						WindowsForm* window=windows.Values()[index];
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
							if(window==mainWindow)
							{
								for(int i=0;i<windows.Count();i++)
								{
									if(windows.Values()[i]->IsVisible())
									{
										windows.Values()[i]->Hide();
									}
								}
								while(windows.Count())
								{
									DestroyNativeWindow(windows.Values()[0]);
								}
								PostQuitMessage(0);
							}
							break;
						}
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
					p.x=location.x;
					p.y=location.y;
					HWND handle=WindowFromPoint(p);
					int index=windows.Keys().IndexOf(handle);
					if(index==-1)
					{
						return 0;
					}
					else
					{
						return windows.Values()[index];
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

			IWindowsForm* GetWindowsForm(INativeWindow* window)
			{
				return dynamic_cast<WindowsForm*>(window);
			}

			void DestroyWindowsNativeController(INativeController* controller)
			{
				delete controller;
			}
		}
	}
}