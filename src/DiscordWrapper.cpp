#include "DiscordWrapper.hpp"
#include "CRC32.hpp"
#include "Xor.hpp"

// Defines memset()
#include <string.h>

#include <stdio.h>
#include <chrono>

void DiscordWrapper::Init() {
	DiscordEventHandlers EventHandlers;
	memset(&EventHandlers, 0, sizeof(DiscordEventHandlers));

	EventHandlers.ready = OnReady;
	EventHandlers.disconnected = OnDisconnected;
	EventHandlers.errored = OnError;

	Discord_Initialize(Xor("693101484845563925"), &EventHandlers, 1, NULL);
}

void DiscordWrapper::Quit() {
	Discord_Shutdown();
}

void DiscordWrapper::RunCallbacks() {
	Discord_RunCallbacks();
}

void DiscordWrapper::UpdatePresence(std::string strTitle, std::string strAlbum, std::string strArtist, long lPlayerTime) {
	DiscordRichPresence RichPresence;
	memset(&RichPresence, 0, sizeof(DiscordRichPresence));

	const char* szTitle = strTitle.c_str();
	const char* szAlbum = strAlbum.c_str();
	const char* szArtist = strArtist.c_str();

	char szArtistFormatted[64] = { 0 };
	sprintf_s(szArtistFormatted, Xor("by %s"), szArtist);

	char szAlbumHash[64] = { 0 };
	uint32_t nAlbumHash = CRC32((unsigned char*)szAlbum, strlen(szAlbum));
	sprintf_s(szAlbumHash, Xor("%x"), nAlbumHash);

	char szArtistHash[64] = { 0 };
	uint32_t nArtistHash = CRC32((unsigned char*)szAlbum, strlen(szAlbum));
	sprintf_s(szArtistHash, Xor("%x"), szArtistHash);

	using namespace std::chrono;
	long lCurrentEpochTime = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

	if (strTitle.length() > 0)
		RichPresence.details = szTitle;

	if (strArtist.length() > 0)
		RichPresence.state = szArtistFormatted;

	if (strAlbum.length() > 0) {
		RichPresence.largeImageKey = szAlbumHash;
		RichPresence.largeImageText = szAlbum;
	}

	if (lPlayerTime >= 0)
		RichPresence.startTimestamp = lCurrentEpochTime - lPlayerTime;

	/*RichPresence.smallImageKey = szArtistHash;
	RichPresence.smallImageText = szArtist;*/

	printf(Xor("%s\n"), szAlbumHash);

	Discord_UpdatePresence(&RichPresence);
}

void DiscordWrapper::ClearPresence() {
	Discord_ClearPresence();
}

void DiscordWrapper::OnReady(const DiscordUser* pConnectedUser) {
	printf(Xor("Ready! Connected user: %s#%s (%s)\n"), pConnectedUser->username, pConnectedUser->discriminator, pConnectedUser->userId);
}

void DiscordWrapper::OnDisconnected(int nErrorCode, const char* szMessage) {
	printf(Xor("Disconnected! Error %i: %s\n"), nErrorCode, szMessage);
}

void DiscordWrapper::OnError(int nErrorCode, const char* szMessage) {
	printf(Xor("Error! Error %i: %s\n"), nErrorCode, szMessage);
}

DiscordWrapper* g_pDiscordWrapper = new DiscordWrapper();