#pragma once
#include "GeometryGenerator.h"
#include "IndexedTriangleList.h"

class AdvancedPlane
{
public:
	static IndexedTriangleList Make(int divisions_x, int divisions_y)
	{
		using namespace Dvtx;
		using Type = Dvtx::VertexLayout::ElementType;

		GeometryGenerator::MeshData _plane;
		GeometryGenerator::CreateGrid(divisions_x, divisions_y, 80 , 80, _plane);

		Dvtx::VertexLayout layout;
		layout.Append(Type::Position3D).Append(Type::Normal).Append(Type::Texture2D).Append(Type::Tangent);

		Dvtx::VertexBuffer vertices(std::move(layout), _plane.Vertices.size());

		for (int i = 0; i < _plane.Vertices.size(); ++i)
		{
			auto& vertex = _plane.Vertices[i];
			vertices[i].Attr<Type::Position3D>() = vertex.Position;
			vertices[i].Attr<Type::Normal>() = vertex.Normal;
			vertices[i].Attr<Type::Texture2D>() = vertex.TexC;
			vertices[i].Attr<Type::Tangent>() = vertex.TangentU;
		}

		return { std::move(vertices), std::vector<unsigned short>(_plane.Indices.begin(), _plane.Indices.end()) };

	}
};