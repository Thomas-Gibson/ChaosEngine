#include "Application.h"
#include <exception>
#include "IWindow.h"
#include "D3D11_Graphics.h"

chaos::Settings chaos::Application::settings;

chaos::Application::~Application()
{

	try
	{
		IWindow* window = IWindow::Factory::Create(
			this->settings.Display.width,
			this->settings.Display.height,
			this->settings.Project.name);

		window->SetIcon(this->settings.Project.icon);
		window->SetCaptionColor(this->settings.Display.caption_color);
		window->SetBoarderColor(this->settings.Display.border_color);
		window->SetBorderEdges(this->settings.Display.rounded_edges);
		window->SetResizable(this->settings.Display.resizable);
		window->SetStyle(this->settings.Display.display_mode);

		Renderer3D renderer(window);

		window->Show();

		while (window->ProcessEvents())
		{
			renderer.Update();
		}

	}
	catch (const std::exception& e)
	{

	}
}
void chaos::Application::ConfigureSettings(Settings& settings)
{
	this->settings = settings;
}