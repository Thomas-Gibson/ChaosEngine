#include <chaos.h>
#include <transform.h>
#include <Windows.h>

#include <iostream>

class MyScene : public chaos::Scene
{
private:

public:

	// Inherited via Scene
	void Initialize() override
	{
		SetClearColor({ 0,0,0 });

		chaos::Camera camera = chaos::Camera(glm::vec3(0, 500, -500), glm::vec3(0, 0, 0), 110.0f, 16.0f / 9.0f, 0.1f, 1000);
		SetCamera(camera);

		entities[0] = { {}, {}, new chaos::Mesh("old-rusty-car/source/rusty.rar/oldcar.FBX")};
	}

	void Destroy() override
	{
		delete entities[0].mesh;
	}

	void Update() override
	{
		ImGui::Begin("Menu");

		if (chaos::keyboard->IsKeyPressed('W'))
			std::cout << "W" << std::endl;


		if (ImGui::CollapsingHeader("Object Rotation"))
		{
			ImGui::SliderFloat("Rotation Roll", &entities[0].worldTransform.rotation.z, 0, 360);
			ImGui::SliderFloat("Rotation Pitch", &entities[0].worldTransform.rotation.x, 0, 360);
			ImGui::SliderFloat("Rotation Yaw", &entities[0].worldTransform.rotation.y, 0, 360);
		}

		if (ImGui::CollapsingHeader("Object Position"))
		{
			ImGui::SliderFloat("position x", &entities[0].worldTransform.position.x, -100, 100);
			ImGui::SliderFloat("position y", &entities[0].worldTransform.position.y, -100, 100);
			ImGui::SliderFloat("position z", &entities[0].worldTransform.position.z, -100, 100);
		}

		if (ImGui::CollapsingHeader("Object Scale"))
		{
			ImGui::SliderFloat("scale x", &entities[0].worldTransform.scale.z, 0, 5);
			ImGui::SliderFloat("scale y", &entities[0].worldTransform.scale.x, 0, 5);
			ImGui::SliderFloat("scale z", &entities[0].worldTransform.scale.y, 0, 5);
		}

		ImGui::End();

	}

};

class SandBox : public chaos::Application
{
public:
	SandBox()
	{
		chaos::Settings s = {};

		s.Project.name = L"SandBox";
		s.Project.icon = L"RedSquare.ico";
		s.Project.StartingScene = new MyScene;

		s.Display.width = 1920;
		s.Display.height = 1080;
		s.Display.caption_color = { 0,0,0 };
		s.Display.border_color = { 0,0,0 };
		s.Display.rounded_edges = true;
		s.Display.resizable = true;
		s.Display.display_mode = chaos::Settings::DisplayModes::STANDARD;
		ConfigureSettings(s);
	}
};

static SandBox sandbox;
