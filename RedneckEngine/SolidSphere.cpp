#include "SolidSphere.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"
#include "Vertex.h"
#include "DepthStencil.h"
#include "ImGui\imgui.h"

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Sphere::Make();
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));
	const auto geometryTag = "$sphere." + std::to_string(radius);
	
	m_UID = geometryTag;
	
	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	auto pvs = VertexShader::Resolve(gfx, "SolidVS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "SolidPS.cso"));

	//AddBind(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, pmc, 1));
	AddBind(std::make_shared<PixelConstantBuffer<PSColorConstant>>(gfx, pmc, 1));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));

	AddBind(Blender::Resolve(gfx, false));

	AddBind(Rasterizer::Resolve(gfx, false));

	AddBind(DepthStencil::Resolve(gfx));
}


DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

void SolidSphere::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	Drawable::Draw(gfx);
}

std::string SolidSphere::GetUID() const noexcept
{
	return m_UID;
}

void SolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	m_pos = pos;
}

bool SolidSphere::IsMenuDrawable() const noexcept
{
	return m_bMenu;
}

void SolidSphere::DrawMenu(Graphics& gfx) noexcept
{
	if (ImGui::Begin(m_UID.c_str(), &m_bMenu))
	{
		bool b_X = false, b_Y = false, b_Z = false;

		ImGui::Text("Position");
		b_X = ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f");
		b_Y = ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f");
		b_Z = ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f");

		ImGui::Text("Shading");

		if (ImGui::ColorEdit4("Solid color", (float*)&pmc.color))
		{
			auto pConstant = QueryBindable<Bind::PixelConstantBuffer<PSColorConstant>>();
			if (pConstant != nullptr)
				pConstant->Update(gfx, pmc);
		}

		if (b_X || b_Y || b_Z)
		{
			SetPos(m_pos);
		}
	}
	ImGui::End();
}

void SolidSphere::ItemSelected() noexcept
{
	m_bMenu = true;
}

DirectX::XMFLOAT3 SolidSphere::GetPos() const noexcept
{
	return m_pos;
}
