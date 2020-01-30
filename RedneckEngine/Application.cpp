#include "Application.h"
#include "EntityManager.h"
#include "OpenFileDialog.h"
#include "TestCube.h"
#include "TestPlane.h"
#include "InputSystem.h"
#include "ConsoleClass.h"
#include "Menu.h"
#include "Cubemap.h"
#include "TestCylinder.h"
#include <random>
#include "Math.h"
#include "PhysicsSphere.h"
#include "PhysicsWorld.h"
#include "PhysicsPlane.h"
#include "PhysicsBox.h"
#include "PhysicsCylinder.h"
#include "PyScriptManager.h"
#include "PBRSphere.h"
#include "PBRPlane.h"
#include "MatrixManager.h"
#include "Radio.h"

enum class EntityType
{
	Box,
	Plane,
	Cylinder,
	Sphere
};

std::shared_ptr<Entity> EntityFactory(Graphics& gfx, EntityType type)
{
	static std::mt19937 rng{ std::random_device{}() };
	static std::uniform_int_distribution<int> boxdist{ 10,30 };
	static std::uniform_int_distribution<int> planedist{ 50,300 };
	static std::uniform_real_distribution<float> adist{ 0.0f,Math::PI * 2.0f };
	static std::uniform_real_distribution<float> ddist{ 0.0f,Math::PI * 0.5f };
	static std::uniform_real_distribution<float> odist{ 0.0f,Math::PI * 0.08f };
	static std::uniform_real_distribution<float> xdist{ 10.0f,70.0f };
	static std::uniform_real_distribution<float> ydist{ 70.0f,100.0f };
	static std::uniform_real_distribution<float> zdist{ 10.0f,60.0f };
	static std::uniform_int_distribution<int> tdist{ 3,30 };

	

	switch (type)
	{
	case EntityType::Box:
	{
		/*auto pBox = std::make_shared<TestCube>(gfx, boxdist(rng));
		pBox->SetPos({ xdist(rng), ydist(rng), zdist(rng)});
		return pBox;*/

		auto pBox = std::make_shared<PhysicsBox>(gfx, boxdist(rng));
		pBox->SetPos({ xdist(rng), ydist(rng), zdist(rng) });
		PhysicsWorld::AddRigidBody(pBox);
		return pBox;
	}
	case EntityType::Cylinder:
	{
		/*auto pCylinder = std::make_shared<TestCylinder>(gfx, 2, 0.01, 7);
		pCylinder->SetPos({ xdist(rng), ydist(rng), zdist(rng) });
		return pCylinder;*/
		
		auto pCylinder = std::make_shared<PhysicsCylinder>(gfx, 2, 0.01, 7);
		pCylinder->SetPos({ xdist(rng), ydist(rng), zdist(rng) });
		PhysicsWorld::AddRigidBody(pCylinder);
		return pCylinder;
	}
	case EntityType::Plane:
	{
		auto pPlane = std::make_shared<PhysicsPlane>(gfx, planedist(rng));
		PhysicsWorld::AddRigidBody(pPlane);
		return pPlane;
	}
	case EntityType::Sphere:
	{
		auto pSphere = std::make_shared<PhysicsSphere>(gfx, 5, 1000);
		pSphere->SetPos({ xdist(rng), ydist(rng), zdist(rng) });
		PhysicsWorld::AddRigidBody(pSphere);
		return pSphere;
	}
	}
}

App::App(int width, int height, const char* title, const std::string& commandLine)
	:
	m_commandLine(commandLine)
{
	m_wnd = Window::CreateInstance(width, height, title);
	m_pGfx = std::make_unique<Graphics>(m_wnd);

	PhysicsWorld::Initialize();

	m_pGfx->m_camera.Initialize(*m_pGfx);
	PhysicsWorld::AddRigidBody(&m_pGfx->m_camera);


	InputSystem::RegisterHotkey(VK_ADD, [&]
	{
		static float r = 5.001f;
		auto pSphere = std::make_shared<PhysicsSphere>(*m_pGfx, r, 5);
		auto& camera = m_pGfx->m_camera;
		auto camPos = camera.GetPos();
		pSphere->SetPos({camPos.x, camPos.y, camPos.z});
		static DirectX::XMFLOAT3 look;
		DirectX::XMStoreFloat3(&look, camera.GetLookVector() * 30);
		pSphere->GetRigidBody()->setLinearVelocity(btVector3(look.x, look.y, look.z));
		PhysicsWorld::AddRigidBody(pSphere);
		m_BasicScene.AddEntity(pSphere);
		r += 0.001f;
	});

	//EntityManager::AddEntity(std::make_shared<Model>(*m_pGfx, OpenFileDialog::ShowDialog(), 0.5f));
	//EntityManager::AddEntity(std::make_shared<SolidSphere>(*m_pGfx, 1));
	
	m_cubemap = std::make_shared<Cubemap>(*m_pGfx, "Images\\grasscube1024.dds");

	/*try
	{

		m_BasicScene.AddEntity(EntityFactory(*m_pGfx, EntityType::Box));
		m_BasicScene.AddEntity(EntityFactory(*m_pGfx, EntityType::Box));
		m_BasicScene.AddEntity(EntityFactory(*m_pGfx, EntityType::Box));
		m_BasicScene.AddEntity(EntityFactory(*m_pGfx, EntityType::Cylinder));
		m_BasicScene.AddEntity(EntityFactory(*m_pGfx, EntityType::Cylinder));
		m_BasicScene.AddEntity(EntityFactory(*m_pGfx, EntityType::Sphere));
		m_BasicScene.AddEntity(EntityFactory(*m_pGfx, EntityType::Plane));
		m_PBRScene.AddEntity(std::make_shared<PointLight>(*m_pGfx, 2));
		
		PBRInitInfo info;
		info.albedo_path = "Images\\PBR\\rusted_iron\\albedo.png";
		info.normal_path = "Images\\PBR\\rusted_iron\\normal.png";
		info.metallic_path = "Images\\PBR\\rusted_iron\\metallic.png";
		info.roughness_path = "Images\\PBR\\rusted_iron\\roughness.png";
		info.ao_path = "Images\\PBR\\rusted_iron\\ao.png";

		auto sp = std::make_shared<PBRSphere>(*m_pGfx, 4, info);
		sp->SetPos({ 5.0f, 4.0f, 10.0f });
		m_PBRScene.AddEntity(sp);

		info.albedo_path = "Images\\PBR\\plastic\\albedo.png";
		info.normal_path = "Images\\PBR\\plastic\\normal.png";
		info.metallic_path = "Images\\PBR\\plastic\\metallic.png";
		info.roughness_path = "Images\\PBR\\plastic\\roughness.png";
		info.ao_path = "Images\\PBR\\plastic\\ao.png";

		m_PBRScene.AddEntity(std::make_shared<PBRPlane>(*m_pGfx, 20, 20, info));

		m_BasicScene.AddItem(std::make_shared<MusicClass>());
		m_BasicScene.AddItem(m_cubemap);
	}
	catch (std::exception & exc)
	{
		Console::WriteLine(exc.what());
		Console::ReadKey();
	}*/


	m_PBRScene.AddEntity(std::make_shared<PointLight>(*m_pGfx, 2));

	PBRInitInfo info;
	info.albedo_path = "Images\\PBR\\rusted_iron\\albedo.png";
	info.normal_path = "Images\\PBR\\rusted_iron\\normal.png";
	info.metallic_path = "Images\\PBR\\rusted_iron\\metallic.png";
	info.roughness_path = "Images\\PBR\\rusted_iron\\roughness.png";
	info.ao_path = "Images\\PBR\\rusted_iron\\ao.png";

	auto sp = std::make_shared<PBRSphere>(*m_pGfx, 4, info);
	sp->SetPos({ 0.0f, 6.0f, 10.0f });
	m_PBRScene.AddEntity(sp);

	info.albedo_path = "Images\\PBR\\granite\\albedo.png";
	info.normal_path = "Images\\PBR\\granite\\normal.png";
	info.metallic_path = "Images\\PBR\\granite\\metallic.png";
	info.roughness_path = "Images\\PBR\\granite\\roughness.png";
	info.ao_path = "Images\\PBR\\granite\\ao.png";
	m_PBRScene.AddEntity(std::make_shared<PBRPlane>(*m_pGfx, 100, 100, info));


	info.albedo_path = "Images\\PBR\\bamboo\\albedo.png";
	info.normal_path = "Images\\PBR\\bamboo\\normal.png";
	info.metallic_path = "Images\\PBR\\bamboo\\metallic.png";
	info.roughness_path = "Images\\PBR\\bamboo\\roughness.png";
	info.ao_path = "Images\\PBR\\bamboo\\ao.png";

	sp = std::make_shared<PBRSphere>(*m_pGfx, 4.01, info);
	sp->SetPos({ 9.0f, 6.0f, 10.0f });
	m_PBRScene.AddEntity(sp);

	info.albedo_path = "Images\\PBR\\grass\\albedo.png";
	info.normal_path = "Images\\PBR\\grass\\normal.png";
	info.metallic_path = "Images\\PBR\\grass\\metallic.png";
	info.roughness_path = "Images\\PBR\\grass\\roughness.png";
	info.ao_path = "Images\\PBR\\grass\\ao.png";

	sp = std::make_shared<PBRSphere>(*m_pGfx, 4.02, info);
	sp->SetPos({ 19.0f, 6.0f, 10.0f });
	m_PBRScene.AddEntity(sp);

	info.albedo_path = "Images\\PBR\\plastic\\albedo.png";
	info.normal_path = "Images\\PBR\\plastic\\normal.png";
	info.metallic_path = "Images\\PBR\\plastic\\metallic.png";
	info.roughness_path = "Images\\PBR\\plastic\\roughness.png";
	info.ao_path = "Images\\PBR\\plastic\\ao.png";

	sp = std::make_shared<PBRSphere>(*m_pGfx, 4.03, info);
	sp->SetPos({ 28.0f, 6.0f, 10.0f });
	m_PBRScene.AddEntity(sp);

	//m_PBRScene.AddItem(std::make_shared<MusicClass>());

	m_PBRScene.AddItem(std::make_shared<Radio>());
}

int App::Run()
{
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			DoFrame();
	}
	return -1;
}

void App::DoFrame()
{
	auto dt = m_timer.Mark();
	m_pGfx->BeginFrame(m_clearColor);
 

	auto& camera = m_pGfx->m_camera;
	camera.Update(dt * 2);
	MatrixManager::SetProjection(camera.GetProjection());
	MatrixManager::SetView(camera.GetView());

	auto camPos = camera.GetPos();
	m_cubemap->SetTransform(DirectX::XMMatrixTranslation(camPos.x, camPos.y, camPos.z));
	m_cubemap->Draw(*m_pGfx);

	PhysicsWorld::Update(dt);

	m_pGfx->m_camera.Bind(*m_pGfx);

	m_PBRScene.Update(dt);
	m_PBRScene.Render(*m_pGfx);

	m_pGfx->EndFrame();
}
