#include "TestCylinder.h"

#include "Cylinder.h"
#include "BindableCodex.h"
#include "ImGui\imgui.h"
#include "BindableCommon.h"
#include "Texture.h"
#include "Sampler.h"
#include "TransformCbuf.h"
#include "DepthStencil.h"

using namespace DirectX;

TestCylinder::TestCylinder(Graphics& gfx, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount)
{
	using namespace Bind;

	auto model = Cylinder::Make(bottomRadius, topRadius, height, sliceCount, stackCount, 
		Dvtx::VertexLayout{}
		.Append(Dvtx::VertexLayout::ElementType::Position3D)
		.Append(Dvtx::VertexLayout::ElementType::Normal));
	model.SetNormalsIndependentFlat();

	m_UID = "$cylinder." + std::to_string(bottomRadius) + "." + std::to_string(topRadius);
	AddBind(VertexBuffer::Resolve(gfx, m_UID, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, m_UID, model.indices));

	AddBind(Sampler::Resolve(gfx));

	auto pvs = VertexShader::Resolve(gfx, "PhongVSNotex.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "PhongPSNotex.cso"));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));

	AddBind(std::make_shared<Blender>(gfx, false, 0.5f));

	AddBind(Rasterizer::Resolve(gfx, false));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(DepthStencil::Resolve(gfx));
}

void TestCylinder::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	m_pos = pos;
}

void TestCylinder::SetRotation(float pitch, float yaw, float roll) noexcept
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}

void TestCylinder::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	Drawable::Draw(gfx);
}

DirectX::XMMATRIX TestCylinder::GetTransformXM() const noexcept
{
	return XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

std::string TestCylinder::GetUID() const noexcept
{
	return m_UID;
}

void TestCylinder::ItemSelected() noexcept
{
	m_bMenu = true;
}

bool TestCylinder::IsMenuDrawable() const noexcept
{
	return m_bMenu;
}

void TestCylinder::DrawMenu(Graphics& gfx) noexcept
{
	if (ImGui::Begin(m_UID.c_str(), &m_bMenu))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
	}
	ImGui::End();
}
