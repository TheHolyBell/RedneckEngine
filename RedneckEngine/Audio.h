#include <xaudio2.h>
#include <string>
#include <wrl/client.h>
#include "IMenuViewable.h"

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

class AudioEngine
{
	bool m_initialized = false;
protected:
	Microsoft::WRL::ComPtr<IXAudio2> m_pXAudio2 = nullptr;
	IXAudio2MasteringVoice* m_pMasterVoice = nullptr;
	
protected:
	HRESULT Initialize();
};

class MusicClass : protected AudioEngine, public IMenuViewable
{
private:
	HRESULT OpenFile(const std::wstring& filename);
	HRESULT OpenFile(const std::string& filename);

private:
	HANDLE m_hFile = INVALID_HANDLE_VALUE;
	WAVEFORMATEX m_wfx = {};
	XAUDIO2_BUFFER m_buffer = {};
	std::string m_filename = "*No file chosen*";

	IXAudio2SourceVoice* m_pSourceVoice = nullptr;

	bool m_bIsPlaying = false;
	bool m_bMenu = false;
public:
	void Initialize(const std::string& filename);
	void Play();
	void Stop();
	void SetVolume(float volume);
	float GetVolume() const;

	bool IsPlaying() const;

	virtual bool IsMenuDrawable() const noexcept override;
	virtual void DrawMenu(Graphics& gfx) noexcept override;
	virtual void ItemSelected() noexcept override;
	virtual std::string GetUID() const noexcept override;
};