/*
    Pok�mon Emerald 2 Version
    ------------------------------

    file        : scrnloader.h
    author      : Philip Wellnitz (RedArceus)
    description : Header file. See corresponding source file for details.

    Copyright (C) 2012 - 2014
        Philip Wellnitz (RedArceus)

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any
    damages arising from the use of this software.

    Permission is granted to anyone to use this software for any
    purpose, including commercial applications, and to alter it and
    redistribute it freely, subject to the following restrictions:


    1.	The origin of this software must not be misrepresented; you
        must not claim that you wrote the original software. If you use
        this software in a product, an acknowledgment in the product
        is required.

    2.	Altered source versions must be plainly marked as such, and
        must not be misrepresented as being the original software.

    3.	This notice may not be removed or altered from any source
        distribution.
*/

#pragma once

#include <nds.h>

#include "sprite.h"
#include <string>

#ifndef __SCRN__
#define __SCRN__

#define sq(a) ((a)*(a))
#define TIMER_SPEED (BUS_CLOCK/1024)

extern int bg3sub;
extern int bg3;
extern int bg2sub;
extern int bg2;
class savgm;
extern savgm SAV;

extern PrintConsole Top,Bottom;
extern ConsoleFont cfont;
extern int achours, acseconds, acminutes, acday, acmonth, acyear;
extern int hours, seconds, minutes, day, month, year;
extern unsigned int ticks;

#define MAXMAPPOS 75
struct MapRegionPos{int lx,ly,rx,ry,ind;};
extern MapRegionPos MapLocations[3][MAXMAPPOS];
void printMapLocation(const MapRegionPos& m);

class attack;
extern attack* AttackList[560];
extern void shoUseAttack(int pkmIdx,bool female, bool shiny);

namespace POKEMON{bool drawInfoSub(u16* layer,int PKMN);}

enum Region{
    NONE = 0,
    HOENN = 1,
    KANTO = 2,
    JOHTO = 3
};

#define BACK_ID  0
#define SAVE_ID  1
#define PKMN_ID  2
#define ID_ID  3
#define DEX_ID  4
#define BAG_ID  5
#define OPTS_ID  6
#define NAV_ID  7
#define A_ID  8
#define M_ID  9
// 10 - used
// 11 - used
// 12 - used
#define FWD_ID  13
#define BWD_ID  14
#define CHOICE_ID  15
// 14 additional spaces used
#define BORDER_ID 31
// 9 additional spaces used
#define SQCH_ID 41
// 42 used

#define MAXBG 9
struct BG_set {
    std::string Name;
    const unsigned int *MainMenu;
    const unsigned short* MainMenuPal;
    bool load_from_rom;
    bool allowsOverlay;
};
extern BG_set BGs[MAXBG];
extern int BG_ind;

void vramSetup();

void updateTime(int mapMode = 0);
void animateMap(u8 frame);

void initVideoSub();
void drawSub();

void animateBack();
void setMainSpriteVisibility(bool hidden);

void drawItem(OAMTable* oam,SpriteInfo* spriteInfo,const std::string& item_name,const int posX,const int posY,const int cnt,
              int& oamIndex,int& palcnt, int& nextAvailableTileIdx,bool subScreen,bool showcnt);
void drawItemIcon(OAMTable* oam,SpriteInfo* spriteInfo,const std::string& item_name,const int posX,const int posY,
                  int& oamIndex,int& palcnt, int& nextAvailableTileIdx,bool subScreen = true);
void drawPKMNIcon(OAMTable* oam,SpriteInfo* spriteInfo,const int& pkmn_no,const int posX,const int posY,
                  int& oamIndex,int& palcnt, int& nextAvailableTileIdx,bool subScreen);
void drawEggIcon(OAMTable* oam,SpriteInfo* spriteInfo,const int posX,const int posY,
                 int& oamIndex,int& palcnt, int& nextAvailableTileIdx,bool subScreen);

bool loadNavScreen(u16* layer,const char* Name,int no);
bool loadPicture(u16* layer,const char* Path, const char* Name,int palsize = 512,int tilecnt = 192*256);
bool loadPictureSub(u16* layer,const char* Path, const char* Name,int palsize = 512,int tilecnt = 192*256);
bool loadSprite(SpriteInfo* spriteInfo,const char* Path, const char* Name,const int TileCnt,const int PalCnt);
bool loadSpriteSub(SpriteInfo* spriteInfo,const char* Path, const char* Name,const int TileCnt,const int PalCnt);
bool loadPKMNSprite(OAMTable* oam,SpriteInfo* spriteInfo, const char* Path,const int& pkmn_no,const int posX,
                    const int posY, int& oamIndex,int& palcnt, int& nextAvailableTileIdx,bool bottom,bool shiny = false,bool female = false,bool flipx = false);
bool loadPKMNSpriteTop(OAMTable* oam,SpriteInfo* spriteInfo, const char* Path,const int& pkmn_no,const int posX,
                       const int posY, int& oamIndex,int& palcnt, int& nextAvailableTileIdx,bool bottom,bool shiny = false,bool female = false,bool flipx = false);

bool loadTrainerSprite(OAMTable* oam,SpriteInfo* spriteInfo, const char* Path,const char* name,const int posX,
                       const int posY, int& oamIndex,int& palcnt, int& nextAvailableTileIdx,bool bottom,bool flipx = false);
bool loadTrainerSpriteTop(OAMTable* oam,SpriteInfo* spriteInfo, const char* Path,const char* name,const int posX,
                          const int posY, int& oamIndex,int& palcnt, int& nextAvailableTileIdx,bool bottom,bool flipx = false);

void drawTypeIcon (OAMTable *oam, SpriteInfo * spriteInfo, int& oamIndex, int& palcnt, int & nextTile, Type t ,int x,int y, bool bottom);

int getCurrentDaytime();

extern std::string bagnames[8];
class scrnloader{
private:
    int pos;

public:
    scrnloader(int p):pos(p){}

    void draw(int m);
    void init();

    void run_bag();
    void run_pkmn();
    void run_dex(int num = 0);
};

#endif