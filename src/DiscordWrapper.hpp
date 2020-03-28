#ifndef DISCORDWRAPPER_HPP
#define DISCORDWRAPPER_HPP

#ifdef _WIN32
#pragma once
#endif

#include "discord_rpc.h"

#include <string>

class DiscordWrapper {
public:
	// Initializes the DiscordRPC library and sets the handler functions
	void Init();

	// Shut down the DiscordRPC library and clean up
	void Quit();

	void RunCallbacks();

	void UpdatePresence(std::string strTitle, std::string strAlbum, std::string strArtist, long lPlayerTime);
	void ClearPresence();
private:
	// Handlers
	static void OnReady(const DiscordUser* pConnectedUser);
	static void OnDisconnected(int nErrorCode, const char* szMessage);
	static void OnError(int nErrorCode, const char* szMessage);
};

extern DiscordWrapper* g_pDiscordWrapper;

#endif