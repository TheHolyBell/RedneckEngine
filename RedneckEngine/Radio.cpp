#include "Radio.h"
#include "Graphics.h"
#include "ImGui\imgui.h"
#include <vlc\libvlc.h>
#include <vlc\libvlc_media.h>
#include <vlc\libvlc_media_discoverer.h>
#include <vlc\libvlc_media_player.h>
#include <vlc\vlc.h>

#include <fstream>
#include <iomanip>
#include "json.hpp"

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlc.x64.lib")
#pragma comment(lib, "libvlccore.lib")
#pragma comment(lib, "libvlccore.x64.lib")


Radio::Radio()
{
	m_VLC = libvlc_new(0, nullptr);

	std::ifstream file("radio_stations.json");
	if (file.is_open())
	{
		using namespace nlohmann;
		json j;
		file >> j;

		for (auto it = j.begin(); it != j.end(); ++it)
			m_Stations[it.key()] = it->get<std::string>();
	}
	else
	{
		m_Stations["Europa Plus"] = "http://ep256.streamr.ru";
		m_Stations["Radio Energy"] = "http://ic7.101.ru:8000/v1_1?userid=0&setst=dchvi0vc1u5e4rl98o61vcv5h1&city=490";
		m_Stations["Nashe Radio"] = "http://nashe1.hostingradio.ru/nashe-256";
		m_Stations["Radio Record"] = "http://air.radiorecord.ru:805/rr_320";
		m_Stations["Comedy Radio"] = "http://ic7.101.ru:8000/s60";
		m_Stations["Radio Mayak"] = "http://icecast.vgtrk.cdnvideo.ru/mayakfm_mp3_192kbps";
	}
}

bool Radio::IsMenuDrawable() const noexcept
{
	return m_bMenu;
}

void Radio::DrawMenu(Graphics& gfx) noexcept
{
	if (ImGui::Begin(GetUID().c_str(), &m_bMenu))
	{
		static char chosen[256] = "Choose an radio station";
		if (ImGui::BeginCombo("Stations", chosen))
		{
			for (const auto& station : m_Stations)
			{
				const auto& name = station.first.c_str();
				if (ImGui::Selectable(name))
				{
					strcpy_s(chosen, name);
					break;
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Play"))
		{
			m_CurrentStation = m_Stations[chosen];
			Play();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
			Stop();

		if (ImGui::Button("Reload JSON"))
		{
			std::ifstream file("radio_stations.json");
			if (file.is_open())
			{
				m_Stations = std::unordered_map<std::string, std::string>();
				using namespace nlohmann;
				json j;
				file >> j;

				for (auto it = j.begin(); it != j.end(); ++it)
					m_Stations[it.key()] = it->get<std::string>();
			}
		}
	}

	ImGui::End();
}

void Radio::ItemSelected() noexcept
{
	m_bMenu = true;
}

std::string Radio::GetUID() const noexcept
{
	return "Radio";
}

void Radio::Play()
{
	libvlc_vlm_del_media(m_VLC, GetUID().c_str());
	libvlc_vlm_add_broadcast(m_VLC, GetUID().c_str(), m_CurrentStation.c_str(), nullptr, 0, nullptr,
		true, false);
	libvlc_vlm_play_media(m_VLC, GetUID().c_str());
}

void Radio::Stop()
{
	libvlc_vlm_stop_media(m_VLC, GetUID().c_str());
}

Radio::~Radio()
{
	using namespace nlohmann;
	std::ofstream fout("radio_stations.json");
	json j;
	for (auto& r_s : m_Stations)
		j[r_s.first] = r_s.second;

	fout  << std::setw(4) << j;
	fout.close();
}
