#include "Camera.h"
#include "ImGui/imgui.h"
#include "Math.h"
#include <algorithm>
#include "Event.h"
#include "MouseEvents.h"
#include "KeyboardEvents.h"
#include "InputSystem.h"
#include "ConsoleClass.h"

using namespace DirectX;

float Camera::travelSpeed = 30.0f;


Camera::Camera() noexcept
{
	Reset();
	EventDispatcher::AddGlobalEventListener<MouseMovedEvent>(*this);
}

DirectX::XMMATRIX Camera::GetProjection() const noexcept
{
	return m_projection;
}

DirectX::XMMATRIX Camera::GetView() const noexcept
{
	auto rotMat = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);
	auto transMat = XMMatrixTranslation(pos.x, pos.y, pos.z);

	auto cameraMat = rotMat * transMat;
	cameraMat = XMMatrixInverse(&XMMatrixDeterminant(cameraMat), cameraMat);

	return cameraMat;

	/*namespace dx = DirectX;

	const dx::XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// apply the camera rotations to a base vector
	const auto lookVector = XMVector3Transform(forwardBaseVector,
		XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f)
	);
	// generate camera transform (applied to all objects to arrange them relative
	// to camera position/orientation in world) from cam position and direction
	// camera "top" always faces towards +Y (cannot do a barrel roll)
	const auto camPosition = XMLoadFloat3(&pos);
	const auto camTarget = camPosition + lookVector;
	return XMMatrixLookAtLH(camPosition, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));*/
}

void Camera::SpawnControlWindow() noexcept
{
	static constexpr float position_range = 80.0f;
	if (ImGui::Begin("Camera"))
	{
		ImGui::SliderFloat("Speed", &travelSpeed, 10.0f, 550.0f, "%.3f");
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -position_range, position_range, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -position_range, position_range, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -position_range, position_range, "%.1f");

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Pitch", &pitch, 0.995f * -90.0f, 0.995f * 90.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);

		if (ImGui::Button("Reset"))
			Reset();
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	pos = { -13.5f, 6.0f, 3.5f };
	pitch = 0.5f;
	yaw = Math::PI / 2.0f;
	travelSpeed = 30.0f;
}

void Camera::Rotate(float dx, float dy) noexcept
{
	yaw = Math::wrap_angle(yaw + dx * rotationSpeed);
	pitch = std::clamp(pitch + dy * rotationSpeed, 0.995f * -Math::PI / 2.0f, 0.995f * Math::PI / 2.0f);
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
	XMStoreFloat3(&translation, XMVector3Transform(
		XMLoadFloat3(&translation),
		XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
		XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
	));
	pos = {
		pos.x + translation.x,
		pos.y + translation.y,
		pos.z + translation.z
	};
}

DirectX::XMFLOAT3 Camera::GetPos() const noexcept
{
	return pos;
}

void Camera::GenerateProjection(int width, int height, float fov)
{
	m_projection = DirectX::XMMatrixPerspectiveFovLH(fov, (float)width / (float)height, 1.0f, 1000.0f);
}

void Camera::Update(float dt)
{
	if (GetAsyncKeyState('W'))
		Translate({ 0.0f, 0.0f, dt });
	if (GetAsyncKeyState('S'))
		Translate({ 0.0f, 0.0f, -dt });
	if (GetAsyncKeyState('A'))
		Translate({ -dt, 0.0f, 0.0f });
	if (GetAsyncKeyState('D'))
		Translate({ dt, 0.0f, 0.0f });
	if (GetAsyncKeyState(VK_SPACE))
		Translate({ 0.0f, dt, 0.0f });
	if (GetAsyncKeyState(VK_SHIFT))
		Translate({ 0.0f, -dt, 0.0f });
}

DirectX::XMVECTOR Camera::GetLookVector() const noexcept
{
	using namespace DirectX;
	XMVECTOR lookVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	lookVector = XMVector3Transform(lookVector, XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f));

	return XMVector3Normalize(lookVector);
}

void Camera::OnEvent(const MouseMovedEvent& event)
{
	if (InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Right))
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(event.pos.x - event.old_pos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(event.pos.y - event.old_pos.y));

		dx *= 250;
		dy *= 250;

		Rotate(dx, dy);
	}
}
