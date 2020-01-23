#pragma once
#include "IBindable.h"
#include <array>
#include <optional>


namespace Bind
{
	class Blender : public IBindable
	{
	public:
		Blender(Graphics& gfx, bool blending, std::optional<float> factors_in = {});

		virtual void Bind(Graphics& gfx) noexcept override;

		void SetFactor(float factor) noexcept(!IS_DEBUG);
		float GetFactor() const noexcept(!IS_DEBUG);

		static std::shared_ptr<Blender> Resolve(Graphics& gfx, bool blending, std::optional<float> factor = {});
		static std::string GenerateUID(bool blending, std::optional<float> factor);

		virtual std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlender;
		bool m_blending = false;
		std::optional<std::array<float, 4>> m_factors;
	};
}