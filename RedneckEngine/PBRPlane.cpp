#include "PBRPlane.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "AdvancedPlane.h"
#include "Vertex.h"
#include "DepthStencil.h"
#include "ImGui\imgui.h"

PBRPlane::PBRPlane(Graphics& gfx, int width, int depth, PBRInitInfo info)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = AdvancedPlane::Make(width, depth);
	//model.Transform(dx::XMMatrixScaling(radius, radius, radius));
	const auto geometryTag = "$PBRPlane." + std::to_string(width) + "." + std::to_string(depth);

	m_UID = geometryTag;

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	auto pvs = VertexShader::Resolve(gfx, "PBR_VS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "PBR_PS.cso"));

	AddBind(Texture::Resolve(gfx, info.albedo_path, 0));
	AddBind(Texture::Resolve(gfx, info.normal_path, 1));
	AddBind(Texture::Resolve(gfx, info.metallic_path, 2));
	AddBind(Texture::Resolve(gfx, info.roughness_path, 3));
	AddBind(Texture::Resolve(gfx, info.ao_path, 4));
	AddBind(Sampler::Resolve(gfx));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));

	AddBind(Blender::Resolve(gfx, false));

	AddBind(Rasterizer::Resolve(gfx, false));

	AddBind(DepthStencil::Resolve(gfx));
}

DirectX::XMMATRIX PBRPlane::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

void PBRPlane::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	Drawable::Draw(gfx);
}

std::string PBRPlane::GetUID() const noexcept
{
	return m_UID;
}

void PBRPlane::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	m_pos = pos;
}

bool PBRPlane::IsMenuDrawable() const noexcept
{
	return m_bMenu;
}

void PBRPlane::DrawMenu(Graphics& gfx) noexcept
{
	if (ImGui::Begin(GetUID().c_str(), &m_bMenu))
	{
		bool b_X = false, b_Y = false, b_Z = false;

		ImGui::Text("Position");
		b_X = ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f");
		b_Y = ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f");
		b_Z = ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f");

		if (b_X || b_Y || b_Z)
		{
			SetPos(m_pos);
		}
	}
	ImGui::End();
}

void PBRPlane::ItemSelected() noexcept
{
	m_bMenu = true;
}