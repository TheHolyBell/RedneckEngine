#pragma once
#include <DirectXMath.h>

class MatrixManager
{
public:
	static DirectX::XMMATRIX GetView();
	static DirectX::XMMATRIX GetProjection();

	static void SetView(DirectX::XMMATRIX mat);
	static void SetProjection(DirectX::XMMATRIX mat);

private:
	static DirectX::XMMATRIX m_View;
	static DirectX::XMMATRIX m_Projection;
};