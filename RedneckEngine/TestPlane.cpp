#include "TestPlane.h"
#include "Plane.h"
#include "BindableCommon.h"
#include "ImGui\imgui.h"
#include "TransformCbufDoubleBoi.h"
#include "Texture.h"
#include "Sampler.h"
#include "Blender.h"
#include "DepthStencil.h"

TestPlane::TestPlane(Graphics& gfx, float size, DirectX::XMFLOAT4 color)
	: pmc({color})
{
	using namespace Bind;
	using namespace DirectX;

	auto model = Plane::Make();
	model.Transform(XMMatrixScaling(size, size, 1.0f) * XMMatrixRotationX(XM_PIDIV2));
	m_UID = "$plane." + std::to_string(size);
	AddBind(VertexBuffer::Resolve(gfx, m_UID, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, m_UID, model.indices));

	auto pvs = VertexShader::Resolve(gfx, "SolidVS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "SolidPS.cso"));

	AddBind(std::make_shared<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));

	AddBind(std::make_shared<Blender>(gfx, true, 0.5f));

	AddBind(Rasterizer::Resolve(gfx, true));

	AddBind(DepthStencil::Resolve(gfx));
}

bool TestPlane::IsMenuDrawable() const noexcept
{
	return m_bMenu;
}

void TestPlane::DrawMenu(Graphics& gfx) noexcept
{
	if (ImGui::Begin(m_UID.c_str(), &m_bMenu))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::Text("Shading");

		if (ImGui::ColorEdit4("Solid color", (float*)&pmc.color))
		{
			auto pConstant = QueryBindable<Bind::PixelConstantBuffer<PSMaterialConstant>>();
			if (pConstant != nullptr)
				pConstant->Update(gfx, pmc);
		}


		auto pBlender = QueryBindable<Bind::Blender>();
		float factor = pBlender->GetFactor();
		if (ImGui::SliderFloat("Translucency", &factor, 0.0f, 1.0f))
			pBlender->SetFactor(factor);
	}
	ImGui::End();
}

void TestPlane::ItemSelected() noexcept
{
	m_bMenu = true;
}

void TestPlane::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	Drawable::Draw(gfx);
}

DirectX::XMMATRIX TestPlane::GetTransformXM() const noexcept
{
	using namespace DirectX;
	return XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		XMMatrixTranslation(pos.x, pos.y, pos.z);
}

std::string TestPlane::GetUID() const noexcept
{
	return m_UID;
}
