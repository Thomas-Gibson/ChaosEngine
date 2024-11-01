#include "IWindow.h"
#include "Win32Window.h"

#ifdef CreateWindow
#undef CreateWindow
#endif // CreateWindow

namespace chaos
{
	IWindow* IWindow::Factory::Create(u32 width, u32 height, wstring title)
	{
		#ifdef _WIN32
		return new Win32Window(width, height, title);
		#endif
	}

}