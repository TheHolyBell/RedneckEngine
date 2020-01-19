#include "Audio.h"
#include "ImGui/imgui.h"
#include "OpenFileDialog.h"

static HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, nullptr, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;

}

static HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, nullptr, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, nullptr))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

HRESULT AudioEngine::Initialize()
{
	HRESULT hr = S_OK;
	if (!m_initialized)
	{
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		if (FAILED(hr = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
			return hr;
		if (FAILED(hr = m_pXAudio2->CreateMasteringVoice(&m_pMasterVoice)))
			return hr;
		m_initialized = true;
	}
	return hr;
}


HRESULT MusicClass::OpenFile(const std::wstring& filename)
{
	m_filename = std::string(filename.begin(), filename.end());

	delete[] m_buffer.pAudioData;

	HRESULT hr = S_OK;

	m_hFile = CreateFile2(filename.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		OPEN_EXISTING,
		nullptr);

	if (INVALID_HANDLE_VALUE == m_hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(m_hFile, 0, nullptr, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	// check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(m_hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(m_hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;

	FindChunk(m_hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(m_hFile, &m_wfx, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(m_hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(m_hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	m_buffer.AudioBytes = dwChunkSize; //buffer containing audio data
	m_buffer.pAudioData = pDataBuffer; // size of the audio buffer in bytes
	m_buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer


	if (m_pSourceVoice != nullptr)
		m_pSourceVoice->DestroyVoice();

	if (FAILED(hr = m_pXAudio2->CreateSourceVoice(&m_pSourceVoice, &m_wfx)))
		return hr;

	if (FAILED(hr = m_pSourceVoice->SubmitSourceBuffer(&m_buffer)))
		return hr;


	return S_OK;
}
HRESULT MusicClass::OpenFile(const std::string& filename)
{
	return OpenFile(std::wstring(filename.begin(), filename.end()));
}

void MusicClass::Initialize(const std::string& filename)
{
	HRESULT hr = S_OK;
	hr = AudioEngine::Initialize();
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "Audio engine initialization failed", "Error", S_OK);
		exit(0);
	}
	hr = OpenFile(filename);
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "Open file failed", "Error", S_OK);
		exit(0);
	}
}

void MusicClass::Play()
{
	if (m_pSourceVoice != nullptr && !m_bIsPlaying)
	{
		m_pSourceVoice->Start(0);
		m_bIsPlaying = true;
	}
}

void MusicClass::Stop()
{
	if (m_pSourceVoice != nullptr && m_bIsPlaying)
	{
		m_pSourceVoice->Stop();
		m_bIsPlaying = false;
	}
}

void MusicClass::SetVolume(float volume)
{
	if (m_pSourceVoice != nullptr)
		m_pSourceVoice->SetVolume(volume);
}

float MusicClass::GetVolume() const
{
	float volume = 0;
	if (m_pSourceVoice != nullptr)
		m_pSourceVoice->GetVolume(&volume);

	return volume;
}

bool MusicClass::IsPlaying() const
{
	return m_bIsPlaying;
}

void MusicClass::SpawnControlWindow()
{
	if (ImGui::Begin("Music control panel"))
	{
		
		ImGui::Text(m_filename.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Select File"))
		{
			OpenFileDialog::ShowDialogAsync([this] (const std::string& path) 
				{	
					Stop();
					Initialize(path);
				}, 
				"WAV (*.wav)\0*.wav"
			);
		}
		{
			if (m_pSourceVoice != nullptr)
			{
				static XAUDIO2_VOICE_STATE xstate = {};
				m_pSourceVoice->GetState(&xstate);
				double percents = (double)xstate.SamplesPlayed / (double)m_buffer.AudioBytes * 4.0;
				ImGui::ProgressBar(percents, ImVec2(170, 15));
				//ImGui::Text(std::to_string((double)xstate.SamplesPlayed / (double)m_buffer.AudioBytes * 100 * 4).c_str());
			}
		}
		if (ImGui::Button("Play"))
			Play();
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
			Stop();
		
	}
	ImGui::End();
}
