#include "scene.h"
#include "Application.h"
#include <exception>
#include "window/IWindow.h"
#include "graphics/Renderer.h"

#include <iostream>

chaos::Application::Application() : settings() {}

chaos::Application::~Application()
{


	for (int i = 0; i < 100; i++)
		std::cout << "*";
	std::cout << std::endl;
	std::cout << "\n\t\tChaos Engine -- Diagnostics\n" << std::endl;

	try
	{
		IWindow* window =
			IWindow::Factory::Create(
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

		auto scene = this->settings.Project.StartingScene;
		scene->Initialize();

		window->Show();
		renderer.SetScene(scene);

		while (window->ProcessEvents())
		{
			renderer.BeginFrame();
			scene->Update();
			renderer.EndFrame();
		}

		scene->Destroy();

		delete window;
		delete scene;

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}

	for (int i = 0; i < 100; i++)
		std::cout << "*";
	std::cout << std::endl;

}
void chaos::Application::ConfigureSettings(Settings& settings)
{
	this->settings = settings;
}

