#ifndef ITUNESWRAPPER_HPP
#define ITUNESWRAPPER_HPP

#ifdef _WIN32
#pragma once
#endif

#include "iTunesCOMInterface.h"
#include <string>

enum PlayerState {
	PlayerStateStopped = 0,
	PlayerStatePlaying = 1,
	PlayerStateFastForward = 2,
	PlayerStateRewind = 3
};

class ITunesWrapper {
public:
	void Init();
	void Quit();

	PlayerState GetPlayerState();

	void FetchCurrentTrack();

	std::string GetTitle();
	std::string GetAlbum();
	std::string GetArtist();

	long GetPlayerTime();
private:
	IiTunes* m_pITunes;
	ITPlayerState m_PlayerState;
	IITTrack* m_pCurrentTrack;

	BSTR m_Title;
	BSTR m_Album;
	BSTR m_Artist;

	std::string m_strTitle;
	std::string m_strAlbum;
	std::string m_strArtist;

	long m_lPlayerTime;
};

extern ITunesWrapper* g_pITunesWrapper;

#endif