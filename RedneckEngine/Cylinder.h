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

		/*constexpr float side = 1.0f / 2.0f;

		Dvtx::VertexBuffer vertices(std::move(layout), 24u);
		vertices[0].Attr<Type::Position3D>() = { -side,-side,-side };// 0 near side
		vertices[1].Attr<Type::Position3D>() = { side,-side,-side };// 1
		vertices[2].Attr<Type::Position3D>() = { -side,side,-side };// 2
		vertices[3].Attr<Type::Position3D>() = { side,side,-side };// 3
		vertices[4].Attr<Type::Position3D>() = { -side,-side,side };// 4 far side
		vertices[5].Attr<Type::Position3D>() = { side,-side,side };// 5
		vertices[6].Attr<Type::Position3D>() = { -side,side,side };// 6
		vertices[7].Attr<Type::Position3D>() = { side,side,side };// 7
		vertices[8].Attr<Type::Position3D>() = { -side,-side,-side };// 8 left side
		vertices[9].Attr<Type::Position3D>() = { -side,side,-side };// 9
		vertices[10].Attr<Type::Position3D>() = { -side,-side,side };// 10
		vertices[11].Attr<Type::Position3D>() = { -side,side,side };// 11
		vertices[12].Attr<Type::Position3D>() = { side,-side,-side };// 12 right side
		vertices[13].Attr<Type::Position3D>() = { side,side,-side };// 13
		vertices[14].Attr<Type::Position3D>() = { side,-side,side };// 14
		vertices[15].Attr<Type::Position3D>() = { side,side,side };// 15
		vertices[16].Attr<Type::Position3D>() = { -side,-side,-side };// 16 bottom side
		vertices[17].Attr<Type::Position3D>() = { side,-side,-side };// 17
		vertices[18].Attr<Type::Position3D>() = { -side,-side,side };// 18
		vertices[19].Attr<Type::Position3D>() = { side,-side,side };// 19
		vertices[20].Attr<Type::Position3D>() = { -side,side,-side };// 20 top side
		vertices[21].Attr<Type::Position3D>() = { side,side,-side };// 21
		vertices[22].Attr<Type::Position3D>() = { -side,side,side };// 22
		vertices[23].Attr<Type::Position3D>() = { side,side,side };// 23

		return{
			std::move(vertices),{
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			}
		};*/
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