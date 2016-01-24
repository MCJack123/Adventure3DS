//
//  main.cpp
//  N3DS Program
//
//  Created by Jack Bruienne on 1/15/16.
//  Copyright © 2016 MCJack123. All rights reserved.
//

#include <cstdio>
#include <cstdlib>
#include <3ds.h>
#include <sys/unistd.h>
#include <cmath>
#include <string>
#include <vector>
#include <tuple>

int lives=5;
int gold=10;
int health=1000;
int enemyh=200;
int sub, xp;
int ebhealth=0;
std::string move="None";
bool rdone=false;
bool ldone=false;
int damagem=1;
std::vector<std::string> special;
int healthm=1;
PrintConsole topScreen, bottomScreen;
u32 kDown, kUp, kHeld;
u8 consoletype;

void cls() {
	consoleSelect(&topScreen);
	consoleClear();
}
int randint(int sinput, int einput) {return rand() % einput + sinput;}
void rendscreen(std::string text) {
    cls();
    printf("--------------------------------------------------------------------------------\n");
    printf("Gold: %i Lives: %i\n", gold, lives);
	printf("Special items: ");
	for (std::string item : special) printf("%s, ", item.c_str());
	printf("\n");
    printf("--------------------------------------------------------------------------------\n");
    printf(text.c_str());
	printf("\n");
}
void rendscreenf() {
    cls();
    printf("--------------------------------------------------------------------------------\n");
    printf("Gold: %i Lives: %i\n", gold, lives);
    printf("Special items: ");
	for (std::string item : special) printf("%s, ", item.c_str());
	printf("\n");
    printf("Health: %i Enemy Health: %i\n", health, enemyh);
    printf("--------------------------------------------------------------------------------\n");
}
void rendscreenx(std::string text) {
    cls();
    printf("--------------------------------------------------------------------------------\n");
    printf("Gold: %i Lives: %i XP: %i\n", gold, lives, xp);
	printf("Special items: ");
	for (std::string item : special) printf("%s, ", item.c_str());
	printf("\n");
    printf("--------------------------------------------------------------------------------\n");
    printf(text.c_str());
	printf("\n");
}
std::tuple<u32, u32, u32> dsIn() {
	hidScanInput();
	return std::make_tuple(hidKeysDown(), hidKeysHeld(), hidKeysUp());
}
bool isButtonPressed(char button, bool wait = true) {
	std::tuple<u32, u32, u32> input = dsIn();
	kDown = std::get<0>(input);
	kHeld = std::get<1>(input);
	kUp	  = std::get<2>(input);
	while (!kDown && wait) {hidScanInput(); u32 kDown = hidKeysDown(); u32 kHeld = hidKeysHeld(); u32 kUp = hidKeysUp();}
	if (kDown && KEY_A && button == 'a') return 0;
	else if (kDown && KEY_B && button == 'b') return 0;
	else if (kDown && KEY_X && button == 'x') return 0;
	else if (kDown && KEY_Y && button == 'y') return 0;
	else if (kDown && KEY_L && button == 'l') return 0;
	else if (kDown && KEY_R && button == 'r') return 0;
	else if (kDown && KEY_START && button == 't') return 0;
	else if (kDown && KEY_SELECT && button == 'e') return 0;
	else if (kDown && KEY_UP && button == 'u') return 0;
	else if (kDown && KEY_DOWN && button == 'd') return 0;
	else if (kDown && KEY_LEFT && button == 'q') return 0;
	else if (kDown && KEY_RIGHT && button == 'e') return 0;
	else if (consoletype) {
		if (kDown && KEY_ZR && button == 'c') return 0;
		else if (kDown && KEY_ZL && button == 'z') return 0;
	}
	return 1;
}
bool input(std::string text = "") {
	bool retval = false;
	printf(text.c_str());
	while (!isButtonPressed('a', false)) if (isButtonPressed('t', false)) retval = true;
	printf("\n");
	return retval;
}
void print(std::string text) {printf("%s\n", text.c_str());}
double random() {return randint(0, 1000) /(double) 100;}

int main(int argc, char* argv[]) {
	gfxInitDefault();
	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM,&bottomScreen);
	consoleSelect(&topScreen);
	APT_CheckNew3DS(&consoletype);
	printf("Console type:%s\n",consoletype?"New3DS. Good job!":"Old3DS. This may be a bit slow.");
	// Main loop
	while (aptMainLoop())
	{
		printf("Welcome to the adventure. Press A to continue, or any other key to exit.\n");
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();
		//hidKeysHeld returns information about which buttons have are held down in this frame
		u32 kHeld = hidKeysHeld();
		//hidKeysUp returns information about which buttons have been just released
		u32 kUp = hidKeysUp();
		while (!kDown) {hidScanInput(); u32 kDown = hidKeysDown(); u32 kHeld = hidKeysHeld(); u32 kUp = hidKeysUp(); if (kDown || kHeld || kUp) printf("Down:%d Held:%d Up:%d", kDown, kHeld, kUp);}
		if (kDown & !KEY_A) break;
		else if (kDown) {
			notknow:
			rendscreen("\n\nYou are walking in a forest. You come to a part you don't know. You can go three ways. Do you go left, right, or straight? (Use the D-Pad to select) ");
			hidScanInput();

			//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
			kDown = hidKeysDown();
			//hidKeysHeld returns information about which buttons have are held down in this frame
			kHeld = hidKeysHeld();
			//hidKeysUp returns information about which buttons have been just released
			kUp = hidKeysUp();
			while (!kDown) {hidScanInput(); u32 kDown = hidKeysDown(); u32 kHeld = hidKeysHeld(); u32 kUp = hidKeysUp();}
			if (kDown & KEY_LEFT) {
				if (ldone) {
					rendscreen("You've already gone here! (A)");
					isButtonPressed('a');
					move="0";
					goto notknow;
				}
				else {
					rendscreen("You found a chest! Inside it was 5 bars of gold. (A to continue)");
					if (input()) goto exit;
					gold +=5;
					rendscreen("It's at a dead end, however. You walk back.");
					if (input()) goto exit;
					move="0";
					ldone=true;
					goto notknow;
				}
			}
			else if (kDown & KEY_RIGHT) {
				if (rdone) {
					rendscreen("You've already gone here!");
					if (input()) goto exit;
					move="0";
				}
				else {
					rendscreen("You walk into a trap and lose a life.(A to continue)");
					if (input()) goto exit;
					lives+=-1;
					rendscreen("You walk back.");
					if (input()) goto exit;
					move="0";
					rdone=true;
				}
			}
			else if (kDown & KEY_UP) continue;
			else if (kDown & KEY_START) goto exit;
			else goto notknow;
			rendscreen("You walk forward for a while.(A to continue)");
			if (input()) goto exit;
			rendscreen("The ground begins to shake...(A to continue)");
			if (input()) goto exit;
			cls();
			print("Louder...");
			usleep(1000);
			rendscreen("You come up to a monster!");
			usleep(1000);
			rendscreen("'I will eat you!'");
			usleep(1000);
			rendscreen("How to fight: Press A to kick and B to punch.");
			if (input()) goto exit;
			rendscreen("When it's your enemy's turn, hope for the best!");
			if (input()) goto exit;
			rendscreen("Press A to start.");
			if (input()) goto exit;
			
			while (enemyh>0) {
				rendscreenf();
				char fmove='0';
				printf("Kick or punch? \n");
				while (fmove != 'K' && fmove != 'P' && fmove != 'a') {
					if (isButtonPressed('a')) fmove = 'K';
					else if (isButtonPressed('b')) fmove = 'P';
					else if (isButtonPressed('e')) {
						if (isButtonPressed('l')) fmove = 'a';
					}
					else if (isButtonPressed('t')) goto exit;
				}
				if (fmove=='K') {
					sub=floor(random()*10)+5;
					enemyh=enemyh-sub*10;
					printf("Damage dealt: %i\n",sub*10);
					if (input()) goto exit;
				}
				else if (fmove=='a') {
					enemyh=0;
				}
				else {
					sub=floor(random()*10);
					enemyh+=-sub*18;
					printf("Damage dealt: %i\n", sub*18);
					if (input()) goto exit;
				}
				rendscreenf();
				usleep(200);
				if (enemyh>0) {
					health+=-floor(random()*100);
					health+=-60;
					printf("Monster attacks! Damage dealt: %i\n", sub+60);
					if (input()) goto exit;
				}
			}
			rendscreen("You won the battle!");
			if (input()) goto exit;
			rendscreen("The monster had 20 gold and you gained two lives from defeating him!");
			gold+=20;
			lives+=2;
			if (input()) goto exit;
			rendscreen("You continue along the path.");
			if (input()) goto exit;
			char berries = 'x';
			// Entrypoint 2																											===============
			rendscreen("You find some berries.  Do you eat them? (A/B) ");
			while (berries != 'y' && berries != 'n') {
				if (isButtonPressed('a')) berries = 'y';
				else if (isButtonPressed('b')) berries = 'n';
				else if (isButtonPressed('t')) goto exit;
			}
			if (berries=='y') {
				rendscreen("Under the leaves, you found a glove!  It grants +50 health in all battles!");
				ebhealth+=50;
				special.push_back("Glove");
				if (input()) goto exit;
				rendscreen("After eating some berries, you continue walking.");
			}
			else {
				rendscreen("You continue walking.");
			}
			if (input()) goto exit;
			char river='0';
			rendscreen("After walking for quite some time, you come to a river.  There's a bear right behind you.  Do you try to cross the river? (A/B) ");
			while (river!='y' && river!='n') {
				if (isButtonPressed('a')) river = 'y';
				else if (isButtonPressed('b')) river = 'n';
				else if (isButtonPressed('t')) goto exit;
			}
			if (river=='y') {
				rendscreen("You manage to cross the river, but you get sick and lose a life.");
				lives+=-1;
			}
			else {
				rendscreen("You stay and fight the bear!");
				if (input()) goto exit;
				health=1000+ebhealth;
				enemyh=400;
				while (enemyh>0) {
					rendscreenf();
					char fmove='0';
					printf("Kick or punch? \n");
					while (fmove!='k' && fmove!='p' && fmove!='a') {
						if (isButtonPressed('a')) fmove = 'k';
						else if (isButtonPressed('b')) fmove = 'p';
						else if (isButtonPressed('e')) {if (isButtonPressed('l')) fmove = 'a';}
						else if (kDown & KEY_START) goto exit;
					}
					if (fmove=='k') {
						sub=floor(random()*10)+6;
						enemyh=enemyh-sub*10;
						printf("Damage dealt: %i\n", sub*10);
						if (input()) goto exit;
					}
					else if (fmove=='a') enemyh=0;
					else {
						sub=floor(random()*10);
						enemyh+=-sub*1.85*10;
						printf("Damage dealt: %d\n", sub*1.85*10);
						if (input()) goto exit;
					}
					rendscreenf();
					usleep(200);
					if (enemyh>0) {
						sub=floor(random()*10)*1.4*10;
						health+=-sub;
						health+=-4*10;
						printf("Bear attacks! Damage dealt: %d\n", sub+4*10);
						if (input()) goto exit;
					}
					if (health<0) {
						lives+=-1;
						health=1000+ebhealth;
					}
				}
				rendscreen("You won the battle!");
				if (input()) goto exit;
				rendscreen("You keep the fur coat from the bear.  It gives you +200 health in all battles!");
				if (input()) goto exit;
				ebhealth+=200;
				special.push_back("Fur Coat");
				rendscreen("You decide to put down a log and cross the river.");
			}
			if (input()) goto exit;
			printf("Unfortunately, this is an *alpha* release for debugging only. This is the end of the program.\n\n\n");
			usleep(2000);
			printf("Exiting...\n");
			usleep(500);
			goto exit;
		}
	}
exit:
	// Exit services
	gfxExit();
	return 0;
}