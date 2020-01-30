#pragma once
#include "GeometryGenerator.h"
#include "IndexedTriangleList.h"

class AdvancedSphere
{
public:
	static IndexedTriangleList Make()
	{
		using namespace Dvtx;
		using Type = Dvtx::VertexLayout::ElementType;

		GeometryGenerator::MeshData _sphere;
		GeometryGenerator::CreateSphere(1, 40, 40, _sphere);

		Dvtx::VertexLayout layout;
		layout.Append(Type::Position3D).Append(Type::Normal).Append(Type::Texture2D).Append(Type::Tangent);

		Dvtx::VertexBuffer vertices(std::move(layout), _sphere.Vertices.size());

		for (int i = 0; i < _sphere.Vertices.size(); ++i)
		{
			auto& pos = _sphere.Vertices[i];
			vertices[i].Attr<Type::Position3D>() = pos.Position;
			vertices[i].Attr<Type::Normal>() = pos.Normal;
			vertices[i].Attr<Type::Texture2D>() = pos.TexC;
			vertices[i].Attr<Type::Tangent>() = pos.TangentU;
		}

		return { std::move(vertices), std::vector<unsigned short>(_sphere.Indices.begin(), _sphere.Indices.end()) };

	}
};