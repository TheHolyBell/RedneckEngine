#include "Cubemap.h"
#include <DirectXTex.h>
#include "DDSTextureLoader.h"
#include "Graphics.h"
#include "BindableCommon.h"
#include "Cube.h"
#include "DepthStencil.h"
#include "ImGui\imgui.h"
#include "OpenFileDialog.h"

Cubemap::Cubemap(Graphics& gfx, const std::string& _cubemap_path)
{
	m_UID = "Cubemap";

	auto model = Cube::MakeIndependent(Dvtx::VertexLayout{}.Append(Dvtx::VertexLayout::Position3D));

	AddBind(VertexBuffer::Resolve(gfx, m_UID, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, m_UID, model.indices));
	
	m_Texture = Texture3D::Resolve(gfx, _cubemap_path);

	AddBind(Sampler::Resolve(gfx));

	auto pvs = VertexShader::Resolve(gfx, "CubeMapVS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "CubeMapPS.cso"));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(Rasterizer::Resolve(gfx, true));
	AddBind(DepthStencil::Resolve(gfx, DepthStencil::Func::LEQUAL, false));

	AddBind(std::make_shared<Blender>(gfx, false, 0.0f));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));
}

DirectX::XMMATRIX Cubemap::GetTransformXM() const noexcept
{
	return m_Transform;
}

void Cubemap::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	m_Texture->Bind(gfx);
	Drawable::Draw(gfx);
}

bool Cubemap::IsMenuDrawable() const noexcept
{
	return m_bMenu;
}

void Cubemap::DrawMenu(Graphics& gfx) noexcept
{
	if (ImGui::Begin(m_UID.c_str(), &m_bMenu))
	{

		if (ImGui::Button("Load new cubemap"))
		{
			m_Texture = Texture3D::Resolve(gfx,*OpenFileDialog::ShowDialog("DDS (.dds)\0*.dds\0"));
		}
	}
	ImGui::End();
}

void Cubemap::ItemSelected() noexcept
{
	m_bMenu = true;
}

void Cubemap::SetTransform(DirectX::XMMATRIX Transform)
{
	m_Transform = Transform;
}

std::string Cubemap::GetUID() const noexcept
{
	return m_UID;
}
