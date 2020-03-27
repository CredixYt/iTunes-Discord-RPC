#include "DiscordWrapper.hpp"
#include "ITunesWrapper.hpp"

#include <thread>
#include <chrono>

#include <iostream>

bool bShouldRun;
std::thread UpdateThread;
std::thread EventThread;

// Fetches data from iTunes and sends it as a Discord RPC
void Update();

// Listens for quit keyboard shortcut and sets bShouldRun accordingly
void Event();

int main() {
	printf("Quit: Ctrl + Alt + Backspace\n");
	bShouldRun = true;

	EventThread = std::thread(Event);
	UpdateThread = std::thread(Update);

	EventThread.join();
	UpdateThread.join();

	return 0;
}

void Update() {
	g_pDiscordWrapper->Init();
	g_pITunesWrapper->Init();

	while (bShouldRun) {
		g_pDiscordWrapper->RunCallbacks();
		
		if (g_pITunesWrapper->GetPlayerState() == PlayerStatePlaying) {
			g_pITunesWrapper->FetchCurrentTrack();

			std::string strTitle = g_pITunesWrapper->GetTitle();
			std::string strAlbum = g_pITunesWrapper->GetAlbum();
			std::string strArtist = g_pITunesWrapper->GetArtist();
			long lPlayerTime = g_pITunesWrapper->GetPlayerTime();

			g_pDiscordWrapper->UpdatePresence(strTitle, strAlbum, strArtist, lPlayerTime);
		}

		std::this_thread::sleep_for(std::chrono::seconds(5));
	}

	g_pDiscordWrapper->Quit();
	g_pITunesWrapper->Quit();
}

void Event() {
	while (!(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_BACK)));

	printf("Quitting...\n");
	bShouldRun = false;
}