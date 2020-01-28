#include "TestCube.h"
#include "Cube.h"
#include "BindableCodex.h"
#include "ImGui\imgui.h"
#include "BindableCommon.h"
#include "Texture.h"
#include "Sampler.h"
#include "TransformCbufDoubleBoi.h"
#include "DepthStencil.h"

using namespace DirectX;

TestCube::TestCube(Graphics& gfx, float size)
{
	using namespace Bind;

	auto model = Cube::MakeIndependentTextured();
	model.Transform(XMMatrixScaling(size, size, size));
	model.SetNormalsIndependentFlat();

	m_UID = "$cube." + std::to_string(size);
	AddBind(VertexBuffer::Resolve(gfx, m_UID, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, m_UID, model.indices));

	AddBind(Texture::Resolve(gfx, "Images\\brickwall.jpg"));
	AddBind(Texture::Resolve(gfx, "Images\\brickwall_normal.jpg", 1.0f));

	AddBind(Sampler::Resolve(gfx));

	auto pvs = VertexShader::Resolve(gfx, "PhongVS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "PhongPSNormalMap.cso"));

	AddBind(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1.0f));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(std::make_shared<TransformCbufDoubleBoi>(gfx, *this, 0, 2));

	AddBind(std::make_shared<Blender>(gfx, false, 0.5f));

	AddBind(Rasterizer::Resolve(gfx, false));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(DepthStencil::Resolve(gfx));
}

void TestCube::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	m_pos = pos;
}

void TestCube::SetRotation(float pitch, float yaw, float roll) noexcept
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}

void TestCube::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	Drawable::Draw(gfx);
}

DirectX::XMMATRIX TestCube::GetTransformXM() const noexcept
{
	return XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

std::string TestCube::GetUID() const noexcept
{
	return m_UID;
}

void TestCube::ItemSelected() noexcept
{
	m_bMenu = true;
}

bool TestCube::IsMenuDrawable() const noexcept
{
	return m_bMenu;
}

void TestCube::DrawMenu(Graphics& gfx) noexcept
{
	bool dirty = false;
	if (ImGui::Begin(m_UID.c_str(), &m_bMenu))
	{

		ImGui::Text("Position");
		dirty = ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f") || dirty;
		dirty = ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f") || dirty;
		dirty = ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f") || dirty;

		ImGui::Text("Orientation");
		dirty = ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f) || dirty;
		dirty = ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f) || dirty;
		dirty = ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f) || dirty;
		ImGui::Text("Shading");
		bool changed0 = ImGui::SliderFloat("Spec. Int.", &pmc.specularIntensity, 0.0f, 1.0f);
		bool changed1 = ImGui::SliderFloat("Spec. Power", &pmc.specularPower, 0.0f, 100.0f);
		bool checkState = pmc.normalMappingEnabled == TRUE;
		bool changed2 = ImGui::Checkbox("Enable Normal Map", &checkState);
		pmc.normalMappingEnabled = checkState ? TRUE : FALSE;
		if (changed0 || changed1 || changed2)
		{
			QueryBindable<Bind::PixelConstantBuffer<PSMaterialConstant>>()->Update(gfx, pmc);
		}
	}
	ImGui::End();

	if (dirty)
		SetPos(m_pos);
}
