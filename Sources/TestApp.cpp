#define NOMINMAX
#include "TestApp.h"
#include "Scene.h"
#include "Model.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Viewport.h"
#include "Renderer.h"
#include "Controller.h"
#include "Material.h"

#include "GLFW/glfw3.h"

#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <filesystem>

#include "Texture2D.h"

bool TestApp::Init()
{
	glfwSetInputMode(GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Scene* scene = this->GetScene();
	ModelLoadParams sponzaLoadParams{
	   .CalcTangentSpace = true,
	   .ConvertToLeftHanded = true,
	   .GenSmoothNormals=true,
	   .GenUVs = false,
	   .PreTransformVertices=false,
	   .Triangulate=false};
	m_sponza = scene->LoadModel("Sponza", "Resources/Models/Sponza/Sponza.gltf", sponzaLoadParams);
	m_sponza->SetScale(glm::vec3(0.045f));

	const auto& sponzaMaterials = m_sponza->GetMaterials();
	for (auto material : sponzaMaterials)
	{
		const std::string_view baseColorPath = material->GetBaseColor()->GetURI();
		// Lion : Gold
		if (baseColorPath == "Resources/Models/Sponza/6772804448157695701.jpg")
		{
			material->SetForceFactor(EMaterialTexture::MetallicRoughness, true);
			material->SetRoughnessFactor(0.2f);
			material->SetMetallicFactor(1.0f);

			material->SetForceFactor(EMaterialTexture::BaseColor, true);
			material->SetBaseColorFactor(glm::vec4(1.0f, 0.766f, 0.336f, 1.0f));
		}

		 //Floor
		if (baseColorPath == "Resources/Models/Sponza/5823059166183034438.jpg")
		{
			material->SetForceFactor(EMaterialTexture::MetallicRoughness, true);
			material->SetRoughnessFactor(0.7f);
			material->SetMetallicFactor(0.0f);

			//material->SetForceFactor(EMaterialTexture::BaseColor, true);
			//material->SetBaseColorFactor(glm::vec4(0.55f, 0.556f, 0.554f, 1.0f));
		}
	}

	const ModelLoadParams sphereLoadParams{ };
	m_sphere = scene->LoadModel("Sphere", "Resources/Models/sphere.obj", sphereLoadParams);
	m_sphere->bCastShadow = false;
	m_sphere->SetActive(false);
	auto sphereMat = m_sphere->GetMaterial(0);
	sphereMat->SetForceFactor(EMaterialTexture::Emissive, true);
	sphereMat->SetForceFactor(EMaterialTexture::BaseColor, true);
	sphereMat->SetBaseColorFactor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	const ModelLoadParams quadLoadParams{
		.CalcTangentSpace = true,
		.ConvertToLeftHanded = true,
		.GenSmoothNormals = true,
		.GenUVs = true,
		.PreTransformVertices = false,
		.Triangulate = true
	};
	m_quad = scene->LoadModel("Quad", "Resources/Models/quad.obj", quadLoadParams);
	m_quad->bCastShadow = false;
	m_quad->bDoubleSided = true;
	m_quad->SetPosition(glm::vec3(-55.0f, 5.5f, 1.5f));
	m_quad->SetRotation(glm::rotate(glm::quat(), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_quad->SetScale(glm::vec3(7.5f, 4.5f, 1.0f));
	auto quadMat = m_quad->GetMaterial(0);
	quadMat->SetForceFactor(EMaterialTexture::BaseColor, true);
	quadMat->SetBaseColorFactor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	std::filesystem::path animeEmissiveTexture = "Resources/Textures/67243791_p0.jpg";
	if (std::filesystem::exists(animeEmissiveTexture))
	{
		quadMat->SetEmissive(new Texture2D("Resources/Textures/67243791_p0.jpg"));
	}
	else
	{
		quadMat->SetForceFactor(EMaterialTexture::Emissive, true);
		quadMat->SetEmissiveFactor(glm::vec3(1.0f));
	}
	quadMat->SetEmissiveIntensity(5.0f);

	m_mainLight = scene->CreateLight("Main");
	// Direct Sunlight
	//m_mainLight->SetIntensity(glm::vec3{ 10.0f });
	// Clear Blue Sky
	//m_mainLight->SetIntensity(10.0f * glm::vec3{0.25098f, 0.611765f, 1.0f});
	// 100W Tungsten
	m_mainLight->SetIntensity(10.0f * glm::vec3(1.0f, 0.945f, 0.6667f));
	this->UpdateLightRotation();

	m_cam = scene->GetMainCamera();
	m_cam->SetPosition(glm::vec3(0.0f, 6.0f, 0.f));
	m_controller = new Controller(m_cam, this->GetWindow());
	m_controller->CameraSpeed = 15.0f;

	unsigned int width = this->GetWidth();
	unsigned int height = this->GetHeight();
	Viewport* mainViewport = m_cam->GetViewport();
	mainViewport->SetWidth(width);
	mainViewport->SetHeight(height);

	return true;
}

void TestApp::Update(float dt)
{
	m_elapsedTime += dt;
	float elapsedTimeCos = std::cos(m_elapsedTime);
	float elapsedTimeSin = std::sin(m_elapsedTime);

	if (m_sphere->IsActivated())
	{
		m_sphere->SetPosition(m_sphereOrbitRad * (glm::vec3(0.0f, 1.0f, 0.0f) + (0.5f * glm::vec3(1.5f * elapsedTimeCos, elapsedTimeSin, 2.0f * elapsedTimeCos * elapsedTimeSin))));
		m_sphere->GetMaterial(0)->SetEmissiveFactor(glm::vec3(elapsedTimeCos * elapsedTimeSin, elapsedTimeCos, elapsedTimeSin * elapsedTimeSin));
	}

	m_controller->Update(dt);
}

void TestApp::WindowResizeCallback(GLFWwindow * window, int width, int height)
{
	Viewport* mainViewport = m_cam->GetViewport();
	mainViewport->SetWidth(width);
	mainViewport->SetHeight(height);

	std::cout << "Window resized: " << width << " , " << height << std::endl;
}

void TestApp::KeyCallback(GLFWwindow * window, int key, int scanCode, int action, int mods)
{
	if (m_controller != nullptr)
	{
		m_controller->KeyCallback(window, key, scanCode, action, mods);
	}

	auto renderer = this->GetRenderer();
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_L:
			break;

		case GLFW_KEY_ESCAPE:
			this->Stop();
			break;

		case GLFW_KEY_LEFT_BRACKET:
			switch(renderer->GetRenderMode())
			{
			case ERenderMode::Deferred:
				renderer->SetRenderMode(ERenderMode::VCT);
				break;

			case ERenderMode::VoxelVisualization:
				renderer->SetRenderMode(ERenderMode::Deferred);
				break;

			default:
				break;
			}
			break;

		case GLFW_KEY_RIGHT_BRACKET:
			switch (renderer->GetRenderMode())
			{
			case ERenderMode::VCT:
				renderer->SetRenderMode(ERenderMode::Deferred);
				break;

			case ERenderMode::Deferred:
				renderer->SetRenderMode(ERenderMode::VoxelVisualization);
				break;

			default:
				break;
			}
			break;

		case GLFW_KEY_R:
			this->m_cam->SetPosition(glm::vec3(0.0f));
			break;

		case GLFW_KEY_V:
			this->GetRenderer()->SetVoxelizeEveryFrame(!this->GetRenderer()->IsVoxelizeEveryFrame());
			break;

		case GLFW_KEY_RIGHT:
			m_lightRotationX += 5.0f;
			this->UpdateLightRotation();
			break;

		case GLFW_KEY_LEFT:
			m_lightRotationX -= 5.0f;
			this->UpdateLightRotation();
			break;

		case GLFW_KEY_DOWN:
			m_lightRotationY -= 10.0f;
			this->UpdateLightRotation();
			break;

		case GLFW_KEY_UP:
			m_lightRotationY += 10.0f;
			this->UpdateLightRotation();
			break;

		case GLFW_KEY_E:
			m_sphere->SetActive(!m_sphere->IsActivated());
			break;
		}
	}
}

void TestApp::UpdateLightRotation()
{
	const glm::quat lightXRot = glm::rotate(glm::quat(),
		glm::radians(m_lightRotationX), glm::vec3{ 1.0f, 0.0f, 0.0f });

	const glm::quat lightYRot = glm::rotate(glm::quat(),
		glm::radians(m_lightRotationY), glm::vec3{ 0.0f, 1.0f, 0.0f });

	m_mainLight->SetRotation(lightXRot* lightYRot);

	std::cout << "Light rotation X : " << m_lightRotationX << " Y : " << m_lightRotationY << std::endl;
}
