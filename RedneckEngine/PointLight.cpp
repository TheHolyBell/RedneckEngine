#include "PointLight.h"
#include "ImGui\imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
	: m_mesh(gfx, radius),
	m_cbuf(gfx)
{
	m_cbData = {
		{ 10.0f,9.0f,2.5f },
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.0045f,
		0.00075f,
	};
}

void PointLight::Update(float dt)
{
}

bool PointLight::IsMenuDrawable() const noexcept
{
	return m_bMenu;
}

void PointLight::DrawMenu(Graphics& gfx) noexcept
{
	
	if (ImGui::Begin(GetUID().c_str(), &m_bMenu))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_cbData.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_cbData.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_cbData.pos.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &m_cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
		ImGui::ColorEdit3("Diffuse Color", &m_cbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &m_cbData.ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &m_cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &m_cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &m_cbData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);
	}
	ImGui::End();
}

void PointLight::ItemSelected() noexcept
{
	m_bMenu = true;
}

std::string PointLight::GetUID() const noexcept
{
	return "PointLight";
}

void PointLight::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	m_cbuf.Update(gfx, m_cbData);
	m_cbuf.Bind(gfx);
	m_mesh.SetPos(m_cbData.pos);
	m_mesh.Draw(gfx);
}

DirectX::XMFLOAT3 PointLight::GetPos() const noexcept
{
	return m_mesh.GetPos();
}
