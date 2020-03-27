#include "ITunesWrapper.hpp"

#include <locale>
#include <codecvt>

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> Converter;

void ITunesWrapper::Init() {
	HRESULT  hRes;

	// Apps must be initialized before they can use CreateInstance!!!
	CoInitialize(nullptr);

	hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&m_pITunes);
}

void ITunesWrapper::Quit() {
	m_pITunes->Quit();
}

PlayerState ITunesWrapper::GetPlayerState() {
	m_pITunes->get_PlayerState(&m_PlayerState);
	return (PlayerState)m_PlayerState;
}

void ITunesWrapper::FetchCurrentTrack() {
	m_pITunes->get_CurrentTrack(&m_pCurrentTrack);

	m_pCurrentTrack->get_Name(&m_Title);
	m_pCurrentTrack->get_Album(&m_Album);
	m_pCurrentTrack->get_Artist(&m_Artist);

	// Complicated way of converting BSTRs to char*'s
	// iTunes only returns BSTR, Discord only accepts char* ¯\_(ツ)_/¯
	std::wstring wstrTitle(m_Title, SysStringLen(m_Title));
	std::wstring wstrAlbum(m_Album, SysStringLen(m_Album));
	std::wstring wstrArtist(m_Artist, SysStringLen(m_Artist));

	m_strTitle = Converter.to_bytes(wstrTitle);
	m_strAlbum = Converter.to_bytes(wstrAlbum);
	m_strArtist = Converter.to_bytes(wstrArtist);
}

std::string ITunesWrapper::GetTitle() {
	return m_strTitle;
}

std::string ITunesWrapper::GetAlbum() {
	return m_strAlbum;
}

std::string ITunesWrapper::GetArtist() {
	return m_strArtist;
}

long ITunesWrapper::GetPlayerTime() {
	m_pITunes->get_PlayerPosition(&m_lPlayerTime);
	return m_lPlayerTime;
}

ITunesWrapper* g_pITunesWrapper = new ITunesWrapper();