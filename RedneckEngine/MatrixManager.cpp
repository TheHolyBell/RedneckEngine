#include "MatrixManager.h"

DirectX::XMMATRIX MatrixManager::m_View;
DirectX::XMMATRIX MatrixManager::m_Projection;

DirectX::XMMATRIX MatrixManager::GetView()
{
	return m_View;
}

DirectX::XMMATRIX MatrixManager::GetProjection()
{
	return m_Projection;
}

void MatrixManager::SetView(DirectX::XMMATRIX mat)
{
	m_View = mat;
}

void MatrixManager::SetProjection(DirectX::XMMATRIX mat)
{
	m_Projection = mat;
}
