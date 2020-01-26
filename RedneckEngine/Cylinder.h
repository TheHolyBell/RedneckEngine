#pragma once
#include <optional>
#include "Vertex.h"
#include "IndexedTriangleList.h"
#include "Math.h"
#include <array>
#include "GeometryGenerator.h"


class Cylinder
{
public:
	static IndexedTriangleList Make(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Dvtx::VertexLayout layout)
	{
		using namespace Dvtx;
		using Type = Dvtx::VertexLayout::ElementType;

		GeometryGenerator::MeshData _cylinder;
		GeometryGenerator::CreateCylinder(bottomRadius, topRadius, height, sliceCount, stackCount, _cylinder);
		
		Dvtx::VertexBuffer vertices(std::move(layout), _cylinder.Vertices.size());
		
		for (int i = 0; i < _cylinder.Vertices.size(); ++i)
		{
			auto& pos = _cylinder.Vertices[i];
			vertices[i].Attr<Type::Position3D>() = pos.Position;
			vertices[i].Attr<Type::Normal>() = pos.Normal;
		}
		
		return { std::move(vertices), std::vector<unsigned short>(_cylinder.Indices.begin(), _cylinder.Indices.end()) };

	}
	/*static IndexedTriangleList MakeIndependentTextured()
	{
		using namespace Dvtx;
		using Type = Dvtx::VertexLayout::ElementType;

		auto itl = MakeIndependent(std::move(VertexLayout{}
			.Append(Type::Position3D)
			.Append(Type::Normal)
			.Append(Type::Texture2D)
		));

		itl.vertices[0].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[1].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[2].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[3].Attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[4].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[5].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[6].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[7].Attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[8].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[9].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[10].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[11].Attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[12].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[13].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[14].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[15].Attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[16].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[17].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[18].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[19].Attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[20].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[21].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[22].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[23].Attr<Type::Texture2D>() = { 1.0f,1.0f };

		return itl;
	}*/
};