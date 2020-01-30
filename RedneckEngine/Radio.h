#pragma once
#include "IMenuViewable.h"
#include <unordered_map>
#include <vlc\libvlc.h>



class Radio : public IMenuViewable
{
public:
	Radio();

	virtual bool IsMenuDrawable() const noexcept override;
	virtual void DrawMenu(Graphics& gfx) noexcept override;
	virtual void ItemSelected() noexcept override;
	virtual std::string GetUID() const noexcept override;

	void Play();
	void Stop();

	virtual ~Radio();
private:
	std::unordered_map<std::string, std::string> m_Stations;
	std::string m_CurrentStation;
	libvlc_instance_t* m_VLC;
	bool m_bMenu = false;
};