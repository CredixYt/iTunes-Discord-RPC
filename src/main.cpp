#include "DiscordWrapper.hpp"

#include <stdio.h>
#include <Windows.h>

#include <thread>
#include <chrono>

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

	while (bShouldRun) {
		g_pDiscordWrapper->RunCallbacks();
		
		if (true) {
			g_pDiscordWrapper->UpdatePresence("Title", "Album", "Artist");
		}

		std::this_thread::sleep_for(std::chrono::seconds(5));
	}

	g_pDiscordWrapper->Quit();
}

void Event() {
	while (!(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_BACK)));

	printf("Quitting...\n");
	bShouldRun = false;
}