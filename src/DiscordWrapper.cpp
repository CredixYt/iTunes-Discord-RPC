#include "DiscordWrapper.hpp"
#include "CRC32.hpp"

// Defines memset()
#include <string.h>

#include <stdio.h>

void DiscordWrapper::Init() {
	DiscordEventHandlers EventHandlers;
	memset(&EventHandlers, 0, sizeof(DiscordEventHandlers));

	EventHandlers.ready = OnReady;
	EventHandlers.disconnected = OnDisconnected;
	EventHandlers.errored = OnError;

	Discord_Initialize("693101484845563925", &EventHandlers, 1, NULL);
}

void DiscordWrapper::Quit() {
	Discord_Shutdown();
}

void DiscordWrapper::RunCallbacks() {
	Discord_RunCallbacks();
}

void DiscordWrapper::UpdatePresence(const char* szTitle, const char* szAlbum, const char* szArtist) {
	DiscordRichPresence RichPresence;
	memset(&RichPresence, 0, sizeof(DiscordRichPresence));

	char szArtistFormatted[64] = { 0 };
	sprintf_s(szArtistFormatted, "by %s", szArtist);

	char szAlbumHash[64] = { 0 };
	uint32_t nAlbumHash = CRC32((unsigned char*)szAlbum, strlen(szAlbum));
	sprintf_s(szAlbumHash, "%x", nAlbumHash);

	char szArtistHash[64] = { 0 };
	uint32_t nArtistHash = CRC32((unsigned char*)szAlbum, strlen(szAlbum));
	sprintf_s(szArtistHash, "%x", szArtistHash);

	RichPresence.details = szTitle;
	RichPresence.state = szArtistFormatted;
	RichPresence.largeImageKey = szAlbumHash;
	RichPresence.largeImageText = szAlbum;
	RichPresence.smallImageKey = szArtistHash;
	RichPresence.smallImageText = szArtist;

	Discord_UpdatePresence(&RichPresence);
}

void DiscordWrapper::OnReady(const DiscordUser* pConnectedUser) {
	printf("Ready! Connected user: %s#%s (%s)\n", pConnectedUser->username, pConnectedUser->discriminator, pConnectedUser->userId);
}

void DiscordWrapper::OnDisconnected(int nErrorCode, const char* szMessage) {
	printf("Disconnected! Error %i: %s\n", nErrorCode, szMessage);
}

void DiscordWrapper::OnError(int nErrorCode, const char* szMessage) {
	printf("Error! Error %i: %s\n", nErrorCode, szMessage);
}

DiscordWrapper* g_pDiscordWrapper = new DiscordWrapper();