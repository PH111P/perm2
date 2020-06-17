/*
Pokémon neo
------------------------------

file        : battleUI.cpp
author      : Philip Wellnitz
description :

Copyright (C) 2012 - 2020
Philip Wellnitz

This file is part of Pokémon neo.

Pokémon neo is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Pokémon neo is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Pokémon neo.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdio>

#include "ability.h"
#include "bagViewer.h"
#include "battle.h"
#include "battleTrainer.h"
#include "battleUI.h"
#include "choiceBox.h"
#include "defines.h"
#include "fs.h"
#include "item.h"
#include "keyboard.h"
#include "move.h"
#include "moveChoiceBox.h"
#include "moveNames.h"
#include "pokemon.h"
#include "pokemonNames.h"
#include "saveGame.h"
#include "sprite.h"
#include "uio.h"
#include "yesNoBox.h"
#include "sound.h"

#include "screenFade.h"

// Sprites
#include "BattleBall1.h" //Normal
#include "BattleBall2.h" //Statused
#include "BattleBall3.h" //Fainted
#include "BattleBall4.h" //NA

#include "battle_namebg.h"
#include "hpbar_battle.h"
#include "hpbar_battle_opp.h"

#include "status_brn.h"
#include "status_fnt.h"
#include "status_frz.h"
#include "status_par.h"
#include "status_psn.h"
#include "status_shiny.h"
#include "status_slp.h"
#include "status_txc.h"

#include "battle_ability.h"
#include "battle_fite.h"
#include "battle_pkmn.h"
#include "battle_inactive.h"
#include "battle_bag.h"
#include "battle_run.h"
#include "battle_fite_2.h"
#include "battle_pkmn_2.h"
#include "battle_inactive_2.h"
#include "battle_bag_2.h"
#include "battle_run_2.h"

#include "stat_up.h"
#include "stat_down.h"

#include "shiny_big.h"
#include "shiny_big_alt.h"
#include "shiny_big_alt2.h"

#include "message_24.h"
#include "message_large.h"

#include "movebox2.h"
#include "movebox3.h"
#include "movebox4.h"

#include "NoPkmn.h"

namespace BATTLE {

// Top screen
// (opp 1, opp 2, own 1, own 2)
#define SPR_ABILITY_OAM( p_opponent ) ( 3 * ( p_opponent ) )
#define SPR_HPBAR_OAM 6
#define SPR_STATUSBG_OAM( p_idx ) ( 10 + 3 * ( p_idx ) )
#define SPR_PKMN_START_OAM( p_idx ) ( 22 + 4 * ( p_idx ) )
#define SPR_PKMN_SHADOW_START_OAM( p_idx ) ( 38 + 4 * ( p_idx ) )
#define SPR_PLATFORM_OAM 54
#define SPR_STATUS_ICON_OAM( p_idx ) ( 58 + ( p_idx ) )
#define SPR_SHINY_ICON_OAM( p_idx ) ( 62 + ( p_idx ) )
#define SPR_SHINY_PARTICLE_START_OAM 70
#define SPR_STAT_CHANGE_PARTICLE_START_OAM 86
#define SPR_BALL_START_OAM 101
#define SPR_STATUS_BALL_OAM( p_idx ) ( 106 + ( p_idx ) )

#define SPR_PKMN_PAL( p_idx ) ( p_idx )
#define SPR_PKMN_SHADOW_PAL 4
#define SPR_PLATFORM_PAL 5
#define SPR_HPBAR_PAL 7
#define SPR_STATUS_ICON_PAL 9
#define SPR_SHINY_ICON_PAL 10
#define SPR_STATUSBG_PAL 11
#define SPR_BOX_PAL 12
#define SPR_STAT_CHANGE_PAL 13
#define SPR_BALL_PAL 14
#define SPR_STATUS_BALL_PAL 15

#define SPR_PKMN_GFX( p_idx ) ( 144 * ( p_idx ) )

// Sub screen
#define SPR_BATTLE_FITE_OAM_SUB 0
#define SPR_BATTLE_BAG_OAM_SUB 2
#define SPR_BATTLE_PKMN_OAM_SUB 4
#define SPR_BATTLE_RUN_OAM_SUB 6
#define SPR_BATTLE_ICON_OAM_SUB 8
#define SPR_SMALL_MESSAGE_OAM_SUB 9
#define SPR_LARGE_MESSAGE_OAM_SUB 20
#define SPR_TYPE_OAM_SUB( p_idx ) ( 40 + ( p_idx ) )
#define SPR_MOVE_OAM_SUB( p_move ) ( 44 + 6 * ( p_move ) )
#define SPR_DMG_CAT_OAM_SUB( p_idx ) ( 80 + ( p_idx ) )


#define SPR_BATTLE_FITE_PAL_SUB 0
#define SPR_BATTLE_BAG_PAL_SUB 1
#define SPR_BATTLE_PKMN_PAL_SUB 2
#define SPR_BATTLE_RUN_PAL_SUB 3
#define SPR_BATTLE_ICON_PAL_SUB 4
#define SPR_BOX_PAL_SUB 5
#define SPR_TYPE_PAL_SUB( p_idx ) ( 6 + ( p_idx ) )
#define SPR_DMG_CAT_PAL_SUB( p_idx ) ( 10 + ( p_idx ) )


// other macros
#define WILD_BATTLE_SPRITE_X_START 144
#define WILD_BATTLE_SPRITE_X 160

#define PKMN_OPP_1_X 192
#define PKMN_OPP_1_Y 20
#define PKMN_OPP_1_X_SINGLE 160
#define PKMN_OPP_1_Y_SINGLE 23
#define PKMN_OPP_2_X 128
#define PKMN_OPP_2_Y 14

#define PKMN_OWN_1_X -40
#define PKMN_OWN_1_Y 84
#define PKMN_OWN_1_X_SINGLE 4
#define PKMN_OWN_1_Y_SINGLE 92
#define PKMN_OWN_2_X 40
#define PKMN_OWN_2_Y 100


#define OPP_PLAT_Y 72
#define PLY_PLAT_Y 120
#define OPP_2_HP_X 94
#define OPP_2_HP_Y 44
#define OPP_1_HP_X 100
#define OPP_1_HP_Y 12
#define OWN_2_HP_X 130
#define OWN_2_HP_Y 152
#define OWN_1_HP_X 124
#define OWN_1_HP_Y 120

#define HP_OUTLINE_COL 240
#define OWN_HP_COL 241
#define OPP_HP_COL 245
#define HP_COL( a, b ) ( ( a ) ? ( OPP_HP_COL + (b) * 2 ) : ( OWN_HP_COL + (b) * 2 ) )

    void battleUI::initTop( ) {
        videoSetMode( MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE
                      | DISPLAY_SPR_1D );
        IO::bg2 = bgInit( 2, BgType_Bmp8, BgSize_B8_256x256, 1, 0 );
        IO::bg3 = bgInit( 3, BgType_Bmp8, BgSize_B8_256x256, 5, 0 );
        bgSetPriority( IO::bg3, 3 );
        bgSetPriority( IO::bg2, 2 );
        IO::initOAMTable( false );
        dmaFillWords( 0, bgGetGfxPtr( IO::bg2 ), 256 * 192 );


        u16 tileCnt = SPR_PKMN_GFX( 4 );

        // platforms
        tileCnt = IO::loadPlatform( _platform2, 144, OPP_PLAT_Y, SPR_PLATFORM_OAM,
                                    SPR_PLATFORM_PAL, tileCnt, false );
        tileCnt = IO::loadPlatform( _platform, -52, PLY_PLAT_Y, SPR_PLATFORM_OAM + 2,
                                    SPR_PLATFORM_PAL + 1, tileCnt, false );

        IO::OamTop->oamBuffer[ SPR_PLATFORM_OAM + 3 ].x = 80 - 16;
        IO::OamTop->oamBuffer[ SPR_PLATFORM_OAM + 2 ].isRotateScale = true;
        IO::OamTop->oamBuffer[ SPR_PLATFORM_OAM + 2 ].isSizeDouble  = true;
        IO::OamTop->oamBuffer[ SPR_PLATFORM_OAM + 2 ].rotationIndex = 0;
        IO::OamTop->oamBuffer[ SPR_PLATFORM_OAM + 3 ].isRotateScale = true;
        IO::OamTop->oamBuffer[ SPR_PLATFORM_OAM + 3 ].isSizeDouble  = true;
        IO::OamTop->oamBuffer[ SPR_PLATFORM_OAM + 3 ].rotationIndex = 0;
        IO::OamTop->matrixBuffer[ 0 ].hdx                           = 140;
        IO::OamTop->matrixBuffer[ 0 ].vdy                           = 130;
        IO::OamTop->matrixBuffer[ 1 ].hdx                           = 154;
        IO::OamTop->matrixBuffer[ 1 ].vdy                           = 154;

        // Preload other sprites
        // HP "bars"
        // opponent
        tileCnt = IO::loadSprite( SPR_STATUSBG_OAM( 0 ), SPR_STATUSBG_PAL, tileCnt, OPP_1_HP_X - 96,
                                  OPP_1_HP_Y, 64, 32, battle_namebgPal, battle_namebgTiles,
                                  battle_namebgTilesLen, false, false, true, OBJPRIORITY_3, false,
                                  OBJMODE_BLENDED );
        IO::loadSprite( SPR_STATUSBG_OAM( 0 ) + 1, SPR_STATUSBG_PAL,
                        IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex, OPP_1_HP_X - 32,
                        OPP_1_HP_Y, 64, 32, 0, 0, 0, false, true, true, OBJPRIORITY_3, false,
                        OBJMODE_BLENDED );
        IO::loadSprite( SPR_STATUSBG_OAM( 1 ), SPR_STATUSBG_PAL,
                        IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex, OPP_2_HP_X - 96,
                        OPP_2_HP_Y, 64, 32, 0, 0, 0, false, false, true, OBJPRIORITY_3, false,
                        OBJMODE_BLENDED );
        IO::loadSprite( SPR_STATUSBG_OAM( 1 ) + 1, SPR_STATUSBG_PAL,
                        IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex, OPP_2_HP_X - 32,
                        OPP_2_HP_Y, 64, 32, 0, 0, 0, false, true, true, OBJPRIORITY_3, false,
                        OBJMODE_BLENDED );

        IO::loadSprite( SPR_HPBAR_OAM, SPR_HPBAR_PAL, tileCnt, OPP_1_HP_X, OPP_1_HP_Y, 32, 32, 0, 0,
                        0, false, false, true, OBJPRIORITY_3, false, OBJMODE_BLENDED );
        tileCnt
            = IO::loadSprite( SPR_HPBAR_OAM + 1, SPR_HPBAR_PAL, tileCnt, OPP_2_HP_X, OPP_2_HP_Y, 32,
                              32, hpbar_battlePal, hpbar_battle_oppTiles, hpbar_battle_oppTilesLen,
                              false, false, true, OBJPRIORITY_3, false, OBJMODE_BLENDED );

        // player
        if( _mode == DOUBLE ) {
            IO::loadSprite( SPR_HPBAR_OAM + 2, SPR_HPBAR_PAL + 1, tileCnt, OWN_1_HP_X, OWN_1_HP_Y,
                            32, 32, 0, 0, 0, false, false, true, OBJPRIORITY_3, false,
                            OBJMODE_BLENDED );
            tileCnt = IO::loadSprite( SPR_HPBAR_OAM + 3, SPR_HPBAR_PAL + 1, tileCnt, OWN_2_HP_X,
                                      OWN_2_HP_Y, 32, 32, hpbar_battlePal, hpbar_battleTiles,
                                      hpbar_battleTilesLen, false, false, true, OBJPRIORITY_3,
                                      false, OBJMODE_BLENDED );

            IO::loadSprite( SPR_STATUSBG_OAM( 2 ), SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex, OWN_1_HP_X,
                            OWN_1_HP_Y, 64, 32, 0, 0, 0, false, false, true, OBJPRIORITY_3, false,
                            OBJMODE_BLENDED );
            IO::loadSprite( SPR_STATUSBG_OAM( 2 ) + 2, SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex,
                            OWN_1_HP_X + 64, OWN_1_HP_Y, 64, 32, 0, 0, 0, false, true, true,
                            OBJPRIORITY_3, false, OBJMODE_BLENDED );
            IO::loadSprite( SPR_STATUSBG_OAM( 2 ) + 1, SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex,
                            OWN_1_HP_X + 76, OWN_1_HP_Y, 64, 32, 0, 0, 0, false, true, true,
                            OBJPRIORITY_3, false, OBJMODE_BLENDED );

            IO::loadSprite( SPR_STATUSBG_OAM( 3 ), SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex, OWN_2_HP_X,
                            OWN_2_HP_Y, 64, 32, 0, 0, 0, false, false, true, OBJPRIORITY_3, false,
                            OBJMODE_BLENDED );
            IO::loadSprite( SPR_STATUSBG_OAM( 3 ) + 2, SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex,
                            OWN_2_HP_X + 64, OWN_2_HP_Y, 64, 32, 0, 0, 0, false, true, true,
                            OBJPRIORITY_3, false, OBJMODE_BLENDED );
            IO::loadSprite( SPR_STATUSBG_OAM( 3 ) + 1, SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex,
                            OWN_2_HP_X + 76, OWN_2_HP_Y, 64, 32, 0, 0, 0, false, true, true,
                            OBJPRIORITY_3, false, OBJMODE_BLENDED );

        } else {
            IO::loadSprite( SPR_HPBAR_OAM + 3, SPR_HPBAR_PAL + 1, tileCnt, OWN_1_HP_X, OWN_1_HP_Y,
                            32, 32, 0, 0, 0, false, false, true, OBJPRIORITY_3, false,
                            OBJMODE_BLENDED );
            tileCnt = IO::loadSprite( SPR_HPBAR_OAM + 2, SPR_HPBAR_PAL + 1, tileCnt, OWN_2_HP_X,
                                      OWN_2_HP_Y, 32, 32, hpbar_battlePal, hpbar_battleTiles,
                                      hpbar_battleTilesLen, false, false, true, OBJPRIORITY_3,
                                      false, OBJMODE_BLENDED );

            IO::loadSprite( SPR_STATUSBG_OAM( 2 ), SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex, OWN_2_HP_X,
                            OWN_2_HP_Y, 64, 32, 0, 0, 0, false, false, true, OBJPRIORITY_3, false,
                            OBJMODE_BLENDED );
            IO::loadSprite( SPR_STATUSBG_OAM( 2 ) + 2, SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex,
                            OWN_2_HP_X + 64, OWN_2_HP_Y, 64, 32, 0, 0, 0, false, true, true,
                            OBJPRIORITY_3, false, OBJMODE_BLENDED );
            IO::loadSprite( SPR_STATUSBG_OAM( 2 ) + 1, SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex,
                            OWN_2_HP_X + 76, OWN_2_HP_Y, 64, 32, 0, 0, 0, false, true, true,
                            OBJPRIORITY_3, false, OBJMODE_BLENDED );

            IO::loadSprite( SPR_STATUSBG_OAM( 3 ), SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex, OWN_1_HP_X,
                            OWN_1_HP_Y, 64, 32, 0, 0, 0, false, false, true, OBJPRIORITY_3, false,
                            OBJMODE_BLENDED );
            IO::loadSprite( SPR_STATUSBG_OAM( 3 ) + 1, SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex,
                            OWN_1_HP_X + 76, OWN_1_HP_Y, 64, 32, 0, 0, 0, false, true, true,
                            OBJPRIORITY_3, false, OBJMODE_BLENDED );
            IO::loadSprite( SPR_STATUSBG_OAM( 3 ) + 2, SPR_STATUSBG_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUSBG_OAM( 0 ) ].gfxIndex,
                            OWN_1_HP_X + 64, OWN_1_HP_Y, 64, 32, 0, 0, 0, false, true, true,
                            OBJPRIORITY_3, false, OBJMODE_BLENDED );
        }

        // Shiny / Status icon
        tileCnt = IO::loadSprite( SPR_STATUS_ICON_OAM( 0 ), SPR_STATUS_ICON_PAL, tileCnt, 0, 0, 8,
                                  8, status_parPal, status_parTiles, status_parTilesLen / 2, false,
                                  false, true, OBJPRIORITY_0, false, OBJMODE_NORMAL );
        // Shiny icon
        tileCnt = IO::loadSprite( SPR_SHINY_ICON_OAM( 0 ), SPR_SHINY_ICON_PAL, tileCnt, 0, 0, 8, 8,
                                  status_shinyPal, status_shinyTiles, status_shinyTilesLen, false,
                                  false, true, OBJPRIORITY_0, false, OBJMODE_NORMAL );

        for( u8 i = 0; i < 4; ++i ) {
            u16 anchorx
                = IO::OamTop->oamBuffer[ SPR_HPBAR_OAM + i ].x + ( ( i < 2 ) ? -8 : 34 + 82 );
            u16 anchory = IO::OamTop->oamBuffer[ SPR_HPBAR_OAM + i ].y + 4;
            IO::loadSprite( SPR_STATUS_ICON_OAM( i ), SPR_STATUS_ICON_PAL,
                            IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 0 ) ].gfxIndex,
                            anchorx, anchory, 8, 8, status_parPal, status_parTiles,
                            status_parTilesLen / 2, false, false, true, OBJPRIORITY_0, false,
                            OBJMODE_NORMAL );
            IO::loadSprite( SPR_SHINY_ICON_OAM( i ), SPR_SHINY_ICON_PAL,
                            IO::OamTop->oamBuffer[ SPR_SHINY_ICON_OAM( 0 ) ].gfxIndex, anchorx,
                            anchory + 8, 8, 8, status_shinyPal, status_shinyTiles,
                            status_shinyTilesLen, false, false, true, OBJPRIORITY_0, false,
                            OBJMODE_NORMAL );
        }

        // Ability windows
        IO::loadSprite( SPR_ABILITY_OAM( 1 ), SPR_BOX_PAL, tileCnt, 64, 96, 64, 32, 0, 0,
                        0, false, false, true, OBJPRIORITY_3, false, OBJMODE_NORMAL );
        IO::loadSprite( SPR_ABILITY_OAM( 1 ) + 1, SPR_BOX_PAL, tileCnt, 32, 96, 64, 32, 0, 0,
                        0, false, false, true, OBJPRIORITY_3, false, OBJMODE_NORMAL );
        IO::loadSprite( SPR_ABILITY_OAM( 1 ) + 2, SPR_BOX_PAL, tileCnt, 0, 96, 64, 32, 0, 0,
                        0, false, false, true, OBJPRIORITY_3, false, OBJMODE_NORMAL );

        IO::loadSprite( SPR_ABILITY_OAM( 0 ) + 1, SPR_BOX_PAL, tileCnt, 128 + 32, 64, 64, 32, 0, 0,
                        0, true, true, true, OBJPRIORITY_3, false, OBJMODE_NORMAL );
        IO::loadSprite( SPR_ABILITY_OAM( 0 ) + 2, SPR_BOX_PAL, tileCnt, 128 + 64, 64, 64, 32, 0, 0,
                        0, true, true, true, OBJPRIORITY_3, false, OBJMODE_NORMAL );
        tileCnt = IO::loadSprite( SPR_ABILITY_OAM( 0 ), SPR_BOX_PAL, tileCnt, 128, 64, 64,
                                  32, battle_abilityPal, battle_abilityTiles,
                                  battle_abilityTilesLen, true,
                                  true, true, OBJPRIORITY_3, false, OBJMODE_NORMAL );

        // Shiny animation
        for( u8 i = 0; i < 8; ++i ) {
            IO::loadSprite( SPR_SHINY_PARTICLE_START_OAM + i, SPR_SHINY_ICON_PAL,
                    IO::OamTop->oamBuffer[ SPR_SHINY_ICON_OAM( 0 ) ].gfxIndex, 0, 0, 8, 8, 0, 0, 0,
                    false, false, true, OBJPRIORITY_0, false, OBJMODE_BLENDED );
        }
        for( u8 i = 8; i < 15; ++i ) {
            IO::loadSprite( SPR_SHINY_PARTICLE_START_OAM + i, SPR_SHINY_ICON_PAL,
                    tileCnt, 0, 0, 16, 16, 0, 0, 0,
                    false, false, true, OBJPRIORITY_0, false, OBJMODE_BLENDED );
        }
        tileCnt =
            IO::loadSprite( SPR_SHINY_PARTICLE_START_OAM + 15, SPR_SHINY_ICON_PAL,
                    tileCnt, 0, 0, 16, 16, 0, shiny_bigTiles, shiny_bigTilesLen,
                    false, false, true, OBJPRIORITY_0, false, OBJMODE_BLENDED );

        // stat increase / decrease
        for( u8 i = 1; i < 15; ++i ) {
            IO::loadSprite( SPR_STAT_CHANGE_PARTICLE_START_OAM + i, SPR_STAT_CHANGE_PAL,
                    tileCnt, 0, 0, 8, 8, 0, 0, 0, false,
                    false, true, OBJPRIORITY_0, false, OBJMODE_BLENDED );
        }
        tileCnt = IO::loadSprite( SPR_STAT_CHANGE_PARTICLE_START_OAM, SPR_STAT_CHANGE_PAL,
                                  tileCnt, 0, 0, 8,
                                  8, stat_upPal, stat_upTiles, stat_upTilesLen / 2, false,
                                  false, true, OBJPRIORITY_0, false, OBJMODE_BLENDED );

        IO::updateOAM( false );
    }

    void battleUI::initSub( ) {
        IO::initOAMTable( true );
        SpriteEntry* oam     = IO::Oam->oamBuffer;

        u16          tileCnt = 0;

        IO::bg2sub = bgInitSub( 2, BgType_Bmp8, BgSize_B8_256x256, 1, 0 );
        bgSetPriority( IO::bg2sub, 2 );
        IO::bg3sub = bgInitSub( 3, BgType_Bmp8, BgSize_B8_256x256, 5, 0 );
        bgSetPriority( IO::bg3sub, 3 );

        dmaFillWords( 0, bgGetGfxPtr( IO::bg3sub ), 256 * 192 );

        dmaFillWords( 0, bgGetGfxPtr( IO::bg2sub ), 256 * 256 );
        bgUpdate( );

        // Load sprites
        // move selection

        IO::loadSprite( SPR_BATTLE_FITE_OAM_SUB, SPR_BATTLE_FITE_PAL_SUB, tileCnt, 128 - 64, 68, 64,
                        64, 0, 0, 0, false, false, true, OBJPRIORITY_3, true, OBJMODE_BLENDED );
        tileCnt = IO::loadSprite( SPR_BATTLE_FITE_OAM_SUB + 1, SPR_BATTLE_FITE_PAL_SUB, tileCnt,
                        128, 64, 64, 64, battle_fitePal, battle_fiteTiles, battle_fiteTilesLen,
                        true, true, true, OBJPRIORITY_3, true, OBJMODE_BLENDED );

        IO::loadSprite( SPR_BATTLE_PKMN_OAM_SUB, SPR_BATTLE_PKMN_PAL_SUB, tileCnt,
                        128 - 32 - 16 - 64 - 6, 68 + 64 + 18, 32,
                        32, 0, 0, 0, false, false, true, OBJPRIORITY_3, true, OBJMODE_BLENDED );
        tileCnt = IO::loadSprite( SPR_BATTLE_PKMN_OAM_SUB + 1, SPR_BATTLE_PKMN_PAL_SUB, tileCnt,
                        128 - 32 - 16 - 32 - 6, 68 + 64 + 18, 32, 32, battle_pkmnPal, battle_pkmnTiles,
                        battle_pkmnTilesLen, true, true, true,
                        OBJPRIORITY_3, true, OBJMODE_BLENDED );

        IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB, SPR_BATTLE_RUN_PAL_SUB, tileCnt,
                        128 - 32, 68 + 64 + 18, 32,
                        32, 0, 0, 0, false, false, true, OBJPRIORITY_3, true, OBJMODE_BLENDED );
        tileCnt = IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB + 1, SPR_BATTLE_RUN_PAL_SUB, tileCnt,
                        128, 68 + 64 + 18, 32, 32, battle_runPal, battle_runTiles,
                        battle_runTilesLen, true, true, true,
                        OBJPRIORITY_3, true, OBJMODE_BLENDED );

        IO::loadSprite( SPR_BATTLE_BAG_OAM_SUB, SPR_BATTLE_BAG_PAL_SUB, tileCnt,
                        128 + 32 + 16 + 6, 68 + 64 + 18, 32,
                        32, 0, 0, 0, false, false, true, OBJPRIORITY_3, true, OBJMODE_BLENDED );
        tileCnt = IO::loadSprite( SPR_BATTLE_BAG_OAM_SUB + 1, SPR_BATTLE_BAG_PAL_SUB, tileCnt,
                        128 + 32 + 16 + 32 + 6, 68 + 64 + 18, 32, 32, battle_bagPal,
                        battle_bagTiles, battle_bagTilesLen, true, true, true,
                        OBJPRIORITY_3, true, OBJMODE_BLENDED );


        // message boxes

        // small (move / attack selection)
        for( u8 i = 0; i < 7; ++i ) {
            IO::loadSprite( SPR_SMALL_MESSAGE_OAM_SUB + 7 - i, SPR_BOX_PAL_SUB, tileCnt,
                        128 - 32 - 64 + 28 * i, 68 - 18 - 26, 32, 32, 0, 0, 0,
                        false, true, true, OBJPRIORITY_3, true, OBJMODE_BLENDED );
        }
        tileCnt = IO::loadSprite( SPR_SMALL_MESSAGE_OAM_SUB, SPR_BOX_PAL_SUB, tileCnt,
                        128 - 24 - 16 - 64, 68 - 18 - 26, 32,
                        32, message_24Pal, message_24Tiles, message_24TilesLen,
                        false, false, true, OBJPRIORITY_3, true, OBJMODE_BLENDED );

        // large (battle log)


        for( u8 i = 0; i < 4; ++i ) {
            for( u8 j = 0; j < 3; ++j ) {
                IO::loadSprite( SPR_LARGE_MESSAGE_OAM_SUB + 3 + 4 * j - i,
                        SPR_BOX_PAL_SUB, tileCnt,
                        8 + 58 * i, 20 + 49 * j, 64, 64, 0, 0, 0,
                        j == 2, i, true, OBJPRIORITY_3, true, OBJMODE_BLENDED );
            }
        }
        tileCnt = IO::loadSprite( SPR_LARGE_MESSAGE_OAM_SUB + 3, SPR_BOX_PAL_SUB, tileCnt,
                        8, 20, 64, 64, message_largePal, message_largeTiles, message_largeTilesLen,
                        false, false, true, OBJPRIORITY_3, true, OBJMODE_BLENDED );


        // Pkmn Icon
        tileCnt = IO::loadSprite( SPR_BATTLE_ICON_OAM_SUB, SPR_BATTLE_ICON_PAL_SUB, tileCnt,
                                  112, 68 + 4, 32, 32, NoPkmnPal, NoPkmnTiles, NoPkmnTilesLen,
                                  false, false, true, OBJPRIORITY_0, true );

        // move windows
        s16 sy = -22;
        for( u8 i = 1; i < 3; i++ ) {
            u8 pos = 2 * i - 2;
            if( i == 1 ) {
                tileCnt = IO::loadSprite(
                    SPR_MOVE_OAM_SUB( pos ), SPR_TYPE_PAL_SUB( pos ), tileCnt, 31, 44 + i * 42 +
                    sy, 16, 32, 0, movebox4Tiles, movebox4TilesLen, false, false,
                    true, OBJPRIORITY_3, true, OBJMODE_NORMAL );
                tileCnt = IO::loadSprite(
                    SPR_MOVE_OAM_SUB( pos ) + 1, SPR_TYPE_PAL_SUB( pos ), tileCnt, 31 + 16,
                    44 + i * 42 + sy, 16, 32, 0, movebox2Tiles, movebox2TilesLen, false, false,
                    true, OBJPRIORITY_3, true, OBJMODE_NORMAL );
            } else {
                IO::loadSprite( SPR_MOVE_OAM_SUB( pos ), SPR_TYPE_PAL_SUB( pos ),
                                oam[ SPR_MOVE_OAM_SUB( 0 ) ].gfxIndex, 31, 44 + i * 42 + sy, 16,
                                32, 0, 0, 0, false, false, true, OBJPRIORITY_3,
                                true, OBJMODE_NORMAL );
                IO::loadSprite( SPR_MOVE_OAM_SUB( pos ) + 1, SPR_TYPE_PAL_SUB( pos ),
                                oam[ SPR_MOVE_OAM_SUB( 0 ) + 1 ].gfxIndex, 31 + 16,
                                44 + i * 42 + sy, 16, 32, 0, 0, 0, false, false, true,
                                OBJPRIORITY_3, true, OBJMODE_NORMAL );
            }
            for( u8 j = 2; j < 5; j++ ) {
                IO::loadSprite( SPR_MOVE_OAM_SUB( pos ) + j, SPR_TYPE_PAL_SUB( pos ),
                                oam[ SPR_MOVE_OAM_SUB( 0 ) + 1 ].gfxIndex, 31 + j * 16,
                                44 + i * 42 + sy, 16, 32, 0, 0, 0, false, false, true,
                                OBJPRIORITY_3, true, OBJMODE_NORMAL );
            }
            IO::loadSprite( SPR_MOVE_OAM_SUB( pos ) + 5, SPR_TYPE_PAL_SUB( pos ),
                            oam[ SPR_MOVE_OAM_SUB( 0 ) ].gfxIndex, 31 + 5 * 16,
                            44 + i * 42 + sy, 16, 32,
                            0, 0, 0, true, true, true, OBJPRIORITY_3,
                            true, OBJMODE_NORMAL );
        }
        for( u8 i = 1; i < 3; i++ ) {
            u8 pos = 2 * i - 1;
            IO::loadSprite( SPR_MOVE_OAM_SUB( pos ), SPR_TYPE_PAL_SUB( pos ),
                            oam[ SPR_MOVE_OAM_SUB( 0 ) ].gfxIndex, 129, 44 + i * 42 + sy, 16, 32,
                            0, 0, 0, false, false, true, OBJPRIORITY_3, true,
                            OBJMODE_NORMAL );
            for( u8 j = 1; j < 5; j++ ) {
                IO::loadSprite( SPR_MOVE_OAM_SUB( pos ) + j, SPR_TYPE_PAL_SUB( pos ),
                                oam[ SPR_MOVE_OAM_SUB( 0 ) + 1 ].gfxIndex, 129 + j * 16,
                                44 + i * 42 + sy, 16, 32, 0, 0, 0, false, false,
                                true, OBJPRIORITY_3, true,
                                OBJMODE_NORMAL );
            }
            IO::loadSprite( SPR_MOVE_OAM_SUB( pos ) + 5, SPR_TYPE_PAL_SUB( pos ),
                            oam[ SPR_MOVE_OAM_SUB( 0 ) ].gfxIndex, 129 + 5 * 16, 44 + i * 42 + sy,
                            16, 32, 0, 0, 0, true, true, true, OBJPRIORITY_3,
                            true, OBJMODE_NORMAL );
        }

        // type icons
        for( u8 i = 0; i < 4; ++i ) {
            tileCnt = IO::loadTypeIcon( UNKNOWN, oam[ SPR_MOVE_OAM_SUB( i ) + 1 ].x,
                    oam[ SPR_MOVE_OAM_SUB( i ) ].y - 8, SPR_TYPE_OAM_SUB( i ),
                    SPR_TYPE_PAL_SUB( i ), tileCnt, true, CURRENT_LANGUAGE );
            oam[ SPR_TYPE_OAM_SUB( i ) ].isHidden = true;
        }

        // damage cat icons
        for( u8 i = 0; i < 4; ++i ) {
            tileCnt = IO::loadTypeIcon( UNKNOWN, oam[ SPR_MOVE_OAM_SUB( i ) + 1 ].x + 32,
                    oam[ SPR_MOVE_OAM_SUB( i ) ].y - 8, SPR_DMG_CAT_OAM_SUB( i ),
                    SPR_DMG_CAT_PAL_SUB( i ), tileCnt, true, CURRENT_LANGUAGE );
            oam[ SPR_DMG_CAT_OAM_SUB( i ) ].isHidden = true;
        }

        IO::updateOAM( true );
    }

    void battleUI::animateBallRelease( bool p_opponent, u8 p_slot, u8 p_ballId ) const {
        for( u8 i = 0; i < 30; ++i ) { swiWaitForVBlank( ); }
        SOUND::playSoundEffect( SFX_BATTLE_RECALL );

        // TODO
    }

    void battleUI::animateBallThrow( u8 p_slot, u8 p_ballId ) const {
        SOUND::playSoundEffect( SFX_BATTLE_THROW );
        for( u8 i = 0; i < 30; ++i ) { swiWaitForVBlank( ); }

        // TODO
    }

    void battleUI::animateShiny( bool p_opponent, u8 p_slot, u8 p_shinyType ) const {
        SOUND::playSoundEffect( SFX_SHINY );
        SpriteEntry* oam     = IO::OamTop->oamBuffer;

        if( p_shinyType == 2 ) { // Forced shinies
            IO::loadSprite( SPR_SHINY_PARTICLE_START_OAM + 15, SPR_SHINY_ICON_PAL,
                    oam[ SPR_SHINY_PARTICLE_START_OAM + 15 ].gfxIndex,
                    0, 0, 16, 16, 0, shiny_big_alt2Tiles, shiny_big_alt2TilesLen,
                    false, false, true, OBJPRIORITY_0, false, OBJMODE_BLENDED );
        } else if( p_shinyType == 1 ) { // Circle for shinies due to extra rolls
            IO::loadSprite( SPR_SHINY_PARTICLE_START_OAM + 15, SPR_SHINY_ICON_PAL,
                    oam[ SPR_SHINY_PARTICLE_START_OAM + 15 ].gfxIndex,
                    0, 0, 16, 16, 0, shiny_big_altTiles, shiny_big_altTilesLen,
                    false, false, true, OBJPRIORITY_0, false, OBJMODE_BLENDED );
        } else { // normal star
            IO::loadSprite( SPR_SHINY_PARTICLE_START_OAM + 15, SPR_SHINY_ICON_PAL,
                    oam[ SPR_SHINY_PARTICLE_START_OAM + 15 ].gfxIndex,
                    0, 0, 16, 16, 0, shiny_bigTiles, shiny_bigTilesLen,
                    false, false, true, OBJPRIORITY_0, false, OBJMODE_BLENDED );
        }


        u16 centerx = oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_slot ) ].x + 48;
        u16 centery = oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_slot ) ].y + 48;

        u16 radius = 25;
        u16 sm_sh = 4, bg_sh = 8;

        if( !p_opponent ) {
            sm_sh = 8, bg_sh = 16;
            radius *= 2;
            centerx += 48;
            centery += 48;
        }

        constexpr s8 dirs[ 32 ][ 2 ] = {
            { 10, 0 }, { 10, 2 }, { 9, 4 }, { 8, 6 },
            { 7, 7 },  { 6, 8 },  { 4, 9 }, { 2, 10 },

            { 0, 10 }, { -2, 10 }, { -4, 9 }, { -6, 8 },
            { -7, 7 }, { -8, 6 },  { -9, 4 }, { -10, 2 },

            { -10, 0 }, { -10, -2 }, { -9, -4 }, { -8, -6 },
            { -7, -7 }, { -6, -8 },  { -4, -9 }, { -2, -10 },

            { 0, -10 }, { 2, -10 }, { 4, -9 }, { 6, -8 },
            { 7, -7 },  { 8, -6 },  { 9, -4 }, { 10, -2 },
        };

        // init frame
        for( u8 i = 0; i < 8; ++i ) {
            oam[ SPR_SHINY_PARTICLE_START_OAM + i ].isHidden = false;
            oam[ SPR_SHINY_PARTICLE_START_OAM + 8 + i ].isHidden = false;

            oam[ SPR_SHINY_PARTICLE_START_OAM + i ].x = centerx + dirs[ 4 * i ][ 0 ] - sm_sh;
            oam[ SPR_SHINY_PARTICLE_START_OAM + i ].y = centery + dirs[ 4 * i ][ 1 ] - sm_sh;
            oam[ SPR_SHINY_PARTICLE_START_OAM + 8 + i ].x = centerx + dirs[ 4 * i + 2 ][ 0 ]
                - bg_sh;
            oam[ SPR_SHINY_PARTICLE_START_OAM + 8 + i ].y = centery + dirs[ 4 * i + 2 ][ 1 ]
                - bg_sh;
        }
        IO::updateOAM( false );
        swiWaitForVBlank( );


        for( u8 f = 0; f < 14; ++f ) {
            for( u8 i = 0; i < 8; ++i ) {
                u8 pos = ( 4 * i + ( f & 3 ) ) & 31;

                oam[ SPR_SHINY_PARTICLE_START_OAM + i ].x = centerx +
                    ( dirs[ pos ][ 0 ] * ( radius + 5 * f / 4 ) / 10 ) - sm_sh;
                oam[ SPR_SHINY_PARTICLE_START_OAM + i ].y = centery +
                    ( dirs[ pos ][ 1 ] * ( radius + 5 * f / 4 ) / 10 ) - sm_sh;
                oam[ SPR_SHINY_PARTICLE_START_OAM + 8 + i ].x = centerx +
                    ( dirs[ ( pos + 2 ) & 31 ][ 0 ] * ( radius + 5 * f / 4 ) / 10 ) - bg_sh;
                oam[ SPR_SHINY_PARTICLE_START_OAM + 8 + i ].y = centery +
                    ( dirs[ ( pos + 2 ) & 31 ][ 1 ] * ( radius + 5 * f / 4 ) / 10 ) - bg_sh;
            }
            IO::updateOAM( false );
            swiWaitForVBlank( );
            if( f & 1 ) {
                swiWaitForVBlank( );
            }
        }

        // hide everything
        for( u8 i = 0; i < 8; ++i ) {
            oam[ SPR_SHINY_PARTICLE_START_OAM + i ].isHidden = true;
            oam[ SPR_SHINY_PARTICLE_START_OAM + 8 + i ].isHidden = true;
        }

        // some random extra noise
        for( u8 f = 0; f < 8; ++f ) {
            for( u8 i = 0; i < 8; ++i ) {
                oam[ SPR_SHINY_PARTICLE_START_OAM + i ].isHidden = rand( ) & 2;
                oam[ SPR_SHINY_PARTICLE_START_OAM + 8 + i ].isHidden = rand( ) & 2;

                oam[ SPR_SHINY_PARTICLE_START_OAM + i ].x = centerx - 32 - sm_sh
                    + ( rand( ) & 63 );
                oam[ SPR_SHINY_PARTICLE_START_OAM + i ].y = centery - 32 - sm_sh
                    + ( rand( ) & 63 );
                oam[ SPR_SHINY_PARTICLE_START_OAM + 8 + i ].x = centerx - 32 - bg_sh
                    + ( rand( ) & 63 );
                oam[ SPR_SHINY_PARTICLE_START_OAM + 8 + i ].y = centery - 32 - bg_sh
                    + ( rand( ) & 63 );
            }
            IO::updateOAM( false );
            swiWaitForVBlank( );
        }

        // hide everything
        for( u8 i = 0; i < 8; ++i ) {
            oam[ SPR_SHINY_PARTICLE_START_OAM + i ].isHidden = true;
            oam[ SPR_SHINY_PARTICLE_START_OAM + 8 + i ].isHidden = true;
        }
        IO::updateOAM( false );


        swiWaitForVBlank( );
    }

    void battleUI::animateStatChange( bool p_opponent, u8 p_slot, bool p_down ) const {
        SpriteEntry* oam     = IO::OamTop->oamBuffer;

        s8 diff = 0, shift = 0;
        u16 centerx = oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_slot ) ].x + 48;
        u16 centery = oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_slot ) ].y + 48;

        if( p_down ) {
            SOUND::playSoundEffect( SFX_BATTLE_DECREASE );
            IO::loadSprite( SPR_STAT_CHANGE_PARTICLE_START_OAM, SPR_STAT_CHANGE_PAL,
                        oam[ SPR_STAT_CHANGE_PARTICLE_START_OAM ].gfxIndex, 0, 0, 8,
                                  8, stat_downPal, stat_downTiles, stat_downTilesLen / 2, false,
                                  false, true, OBJPRIORITY_0, false, OBJMODE_BLENDED );
            diff = 1;
            shift = -20;
        } else {
            SOUND::playSoundEffect( SFX_BATTLE_INCREASE );
            IO::loadSprite( SPR_STAT_CHANGE_PARTICLE_START_OAM, SPR_STAT_CHANGE_PAL,
                        oam[ SPR_STAT_CHANGE_PARTICLE_START_OAM ].gfxIndex, 0, 0, 8,
                                  8, stat_upPal, stat_upTiles, stat_upTilesLen / 2, false,
                                  false, true, OBJPRIORITY_0, false, OBJMODE_BLENDED );
            diff = -1;
            shift = 20;
        }

        constexpr s8 pos[ 15 ][ 2 ] = {
            { -19, 5 },
            { -19, -5 },
            { -10, 0 },
            { -10, 10 },
            { -10, -10 },
            { 0, 5 },
            { 0, -5 },
            { 0, 15 },
            { 0, -15 },
            { 10, 0 },
            { 10, 10 },
            { 10, -10 },
            { 19, -5 },
            { 19, 5 },
        };

        for( u8 i = 0; i < 15; ++i ) {
            oam[ SPR_STAT_CHANGE_PARTICLE_START_OAM + i ].isHidden = false;

            oam[ SPR_STAT_CHANGE_PARTICLE_START_OAM + i ].x = centerx + 3 * pos[ i ][ 0 ] / 2;
            oam[ SPR_STAT_CHANGE_PARTICLE_START_OAM + i ].y
                = centery + shift + 3 * pos[ i ][ 1 ] / 2;
        }
        IO::updateOAM( false );

        for( u8 j = 0; j < 10; ++j ) {
            for( u8 i = 0; i < 15; ++i ) {
                oam[ SPR_STAT_CHANGE_PARTICLE_START_OAM + i ].y += 2 * diff;
            }
            swiWaitForVBlank( );
            IO::updateOAM( false );
        }
        for( u8 i = 0; i < 15; ++i ) {
            oam[ SPR_STAT_CHANGE_PARTICLE_START_OAM + i ].isHidden = true;
        }
        IO::updateOAM( false );
    }


    void battleUI::init( ) {
        IO::fadeScreen( IO::CLEAR_DARK_IMMEDIATE, true, true );
        IO::vramSetup( true );
        initTop( );
        initSub( );

        FS::readPictureData( bgGetGfxPtr( IO::bg3 ), "nitro:/PICS/BATTLE_BACK/",
                             std::to_string( _background ).c_str( ), 512, 49152 );
        FS::readPictureData( bgGetGfxPtr( IO::bg3sub ), "nitro:/PICS/", "battlesub2", 512, 49152,
                true );
        for( u8 i = 0; i < 2; ++i ) {
            u16* pal             = IO::BG_PAL( i );
            pal[ 0 ] = 0;
            pal[ 250 ] = IO::WHITE;
            pal[ 251 ]  = IO::GRAY;
            pal[ 252 ]  = IO::RGB( 18, 22, 31 );
            pal[ 254 ]   = IO::RGB( 31, 18, 18 );
            pal[ 253 ] = IO::RGB( 0, 0, 25 );
            pal[ 255 ]  = IO::RGB( 23, 0, 0 );
        }
    }

    void battleUI::deinit( ) {
        IO::fadeScreen( IO::CLEAR_DARK_IMMEDIATE, false );
        IO::initOAMTable( true );
        IO::initOAMTable( false );
        for( u8 i = 0; i < 2; ++i ) {
            u16* pal             = IO::BG_PAL( i );
            std::memset( pal, 0, 256 * sizeof( u16 ) );
        }
        dmaFillWords( 0, bgGetGfxPtr( IO::bg2 ), 256 * 256 );
        dmaFillWords( 0, bgGetGfxPtr( IO::bg3 ), 256 * 256 );
        dmaFillWords( 0, bgGetGfxPtr( IO::bg2sub ), 256 * 256 );
        dmaFillWords( 0, bgGetGfxPtr( IO::bg3sub ), 256 * 256 );
    }

    void battleUI::resetLog( ) {
        SpriteEntry* oam     = IO::Oam->oamBuffer;
        for( u8 i = 0; i < 128; ++i ) {
            oam[ i ].isHidden = true;
        }
        for( u8 i = 0; i < 12; ++i ) {
            oam[ SPR_LARGE_MESSAGE_OAM_SUB + i ].isHidden = false;
        }
        dmaFillWords( 0, bgGetGfxPtr( IO::bg2sub ), 256 * 256 );
        IO::updateOAM( true );
        _currentLogLine = 1;
    }

    void battleUI::log( std::string p_message ) {
        SpriteEntry* oam     = IO::Oam->oamBuffer;

        if( oam[ SPR_LARGE_MESSAGE_OAM_SUB ].isHidden ) {
            resetLog( );
        }

        IO::regularFont->setColor( 250, 1 );
        IO::regularFont->setColor( 251, 2 );

        u8 height = IO::regularFont->printBreakingStringC( p_message.c_str( ), 16,
                24, 256 - 32, true, IO::font::LEFT, 14,
                ' ', 0, false, false, -1 );

        if( _currentLogLine + height > 12 ) {
            _currentLogLine = 1;
        }

        dmaFillWords( 0, bgGetGfxPtr( IO::bg2sub ) + ( 10 + 14 * _currentLogLine ) * 128,
                ( 14 * ( height + 2 ) ) * 256 );
        _currentLogLine
            += IO::regularFont->printBreakingStringC( p_message.c_str( ), 16,
                                                     10 + 14 * _currentLogLine,
                                                     256 - 32, true, IO::font::LEFT, 14 );
    }

    std::string battleUI::getPkmnName( pokemon* p_pokemon, bool p_opponent,
                                       bool p_sentenceStart ) const {
        char buffer[ 50 ];
        if( p_opponent && _isWildBattle ) {
            if( p_sentenceStart ) {
                snprintf( buffer, 49, GET_STRING( 311 ), p_pokemon->m_boxdata.m_name );
            } else {
                snprintf( buffer, 49, GET_STRING( 309 ), p_pokemon->m_boxdata.m_name );
            }
        } else if( p_opponent ) {
            if( p_sentenceStart ) {
                snprintf( buffer, 49, GET_STRING( 312 ), p_pokemon->m_boxdata.m_name );
            } else {
                snprintf( buffer, 49, GET_STRING( 310 ), p_pokemon->m_boxdata.m_name );
            }
        } else {
            snprintf( buffer, 49, p_pokemon->m_boxdata.m_name );
        }
        return std::string( buffer );
    }

    void battleUI::logBoosts( pokemon* p_pokemon, bool p_opponent, u8 p_slot,
                              boosts p_intended, boosts p_actual ) {
        char buffer[ 100 ];
        auto pkmnstr = getPkmnName( p_pokemon, p_opponent );
        bool up = false;
        for( u8 i = 0; i < 8; ++i ) {
            if( p_intended.getBoost( i ) > 0 ) {
                auto bt = p_actual.getBoost( i );
                if( bt == 0 ) {
                    snprintf( buffer, 99, GET_STRING( 256 ), pkmnstr.c_str( ),
                            GET_STRING( 248 + i ), pkmnstr.c_str( ) );
                } else if ( bt == 1 ) {
                    snprintf( buffer, 99, GET_STRING( 257 ), pkmnstr.c_str( ),
                            GET_STRING( 248 + i ), pkmnstr.c_str( ) );
                    up = true;
                } else if ( bt == 2 ) {
                    snprintf( buffer, 99, GET_STRING( 258 ), pkmnstr.c_str( ),
                            GET_STRING( 248 + i ), pkmnstr.c_str( ) );
                    up = true;
                } else if ( bt >= 3 ) {
                    snprintf( buffer, 99, GET_STRING( 259 ), pkmnstr.c_str( ),
                            GET_STRING( 248 + i ), pkmnstr.c_str( ) );
                    up = true;
                }
                log( std::string( buffer ) );
            }
        }
        if( up ) {
            animateStatChange( p_opponent, p_slot, false );
        }

        up = false;
        for( u8 i = 0; i < 8; ++i ) {
            if( p_intended.getBoost( i ) < 0 ) {
                auto bt = p_actual.getBoost( i );
                if( bt == 0 ) {
                    snprintf( buffer, 99, GET_STRING( 256 ), pkmnstr.c_str( ),
                            GET_STRING( 248 + i ), pkmnstr.c_str( ) );
                } else if ( bt == -1 ) {
                    snprintf( buffer, 99, GET_STRING( 260 ), pkmnstr.c_str( ),
                            GET_STRING( 248 + i ), pkmnstr.c_str( ) );
                    up = true;
                } else if ( bt == -2 ) {
                    snprintf( buffer, 99, GET_STRING( 261 ), pkmnstr.c_str( ),
                            GET_STRING( 248 + i ), pkmnstr.c_str( ) );
                    up = true;
                } else if ( bt <= -3 ) {
                    snprintf( buffer, 99, GET_STRING( 262 ), pkmnstr.c_str( ),
                            GET_STRING( 248 + i ), pkmnstr.c_str( ) );
                    up = true;
                }
                log( std::string( buffer ) );
            }
        }
        if( up ) {
            animateStatChange( p_opponent, p_slot, true );
        }
        for( u8 i = 0; i < 30; ++i ) { swiWaitForVBlank( ); }
    }

    void battleUI::logAbility( pokemon* p_pokemon, bool p_opponent ) {
        for( u8 i = 0; i < 5; ++i ) { swiWaitForVBlank( ); }

        SpriteEntry* oam = IO::OamTop->oamBuffer;

        for( u8 i = 0; i < 3; ++i ) {
            oam[ SPR_ABILITY_OAM( !p_opponent ) + i ].isHidden = false;
        }
        IO::updateOAM( false );

        char buffer[ 100 ];
        snprintf( buffer, 99, GET_STRING( 393 ), p_pokemon->m_boxdata.m_name,
                getAbilityName( p_pokemon->getAbility( ) ).c_str( ),
                p_pokemon->m_boxdata.m_name );

        IO::regularFont->printStringC( buffer, 128 + ( p_opponent ? 18 : -32 ),
                oam[ SPR_ABILITY_OAM( !p_opponent ) ].y + 1, false,
                p_opponent ? IO::font::LEFT : IO::font::RIGHT, 14, -14 );

        for( u8 i = 0; i < 75; ++i ) {
            swiWaitForVBlank( );
        }

        for( u8 i = 0; i < 3; ++i ) {
            oam[ SPR_ABILITY_OAM( !p_opponent ) + i ].isHidden = true;
        }
        dmaFillWords( 0, bgGetGfxPtr( IO::bg2 ) +
                 ( oam[ SPR_ABILITY_OAM( !p_opponent ) ].y + 1 ) * 128, 30 * 256 );
        IO::updateOAM( false );

        for( u8 i = 0; i < 5; ++i ) { swiWaitForVBlank( ); }
    }

    void battleUI::logForewarn( pokemon* p_pokemon, bool p_opponent, u16 p_move ) {
        char buffer[ 50 ];
        snprintf( buffer, 49, GET_STRING( 396 ), getPkmnName( p_pokemon, p_opponent ).c_str( ),
                MOVE::getMoveName( p_move ).c_str( ) );
        log( std::string( buffer ) );
    }

    void battleUI::logAnticipation( pokemon* p_pokemon, bool p_opponent ) {
        char buffer[ 50 ];
        snprintf( buffer, 49, GET_STRING( 397 ), getPkmnName( p_pokemon, p_opponent ).c_str( ) );
        log( std::string( buffer ) );
    }

    void battleUI::logFrisk( pokemon* p_pokemon, bool p_opponent, std::vector<u16> p_itms ) {
        char buffer[ 100 ];
        if( p_itms.size( ) == 1 ) {
            snprintf( buffer, 99, GET_STRING( 398 ), getPkmnName( p_pokemon, p_opponent ).c_str( ),
                    ITEM::getItemName( p_itms[ 0 ] ) );
        } else if( p_itms.size( ) == 2 ) {
            snprintf( buffer, 99, GET_STRING( 399 ), getPkmnName( p_pokemon, p_opponent ).c_str( ),
                    ITEM::getItemName( p_itms[ 0 ] ),
                    ITEM::getItemName( p_itms[ 1 ] ) );
        } else { return; }
        log( std::string( buffer ) );
    }

    void battleUI::updatePkmnStats( bool p_opponent, u8 p_pos, pokemon* p_pokemon ) {
        SpriteEntry* oam = IO::OamTop->oamBuffer;
        u16          anchorx
            = oam[ SPR_HPBAR_OAM + 2 * ( !p_opponent ) + p_pos ].x + ( p_opponent ? -88 : 34 );
        u16 anchory = oam[ SPR_HPBAR_OAM + 2 * ( !p_opponent ) + p_pos ].y + 4;
        u16 hpx     = oam[ SPR_HPBAR_OAM + 2 * ( !p_opponent ) + p_pos ].x;
        u16 hpy     = oam[ SPR_HPBAR_OAM + 2 * ( !p_opponent ) + p_pos ].y;

        IO::regularFont->setColor( 250, 1 );
        IO::regularFont->setColor( 251, 2 );
        IO::smallFont->setColor( 250, 1 );
        IO::smallFont->setColor( 251, 2 );
        // show/hide hp bar
        oam[ SPR_HPBAR_OAM + 2 * ( !p_opponent ) + p_pos ].isHidden
            = p_pokemon == nullptr || ( p_pos && _mode != DOUBLE ) || !p_pokemon->canBattle( );
        // clear relevant part of the screen
        dmaFillWords( 0,
                      bgGetGfxPtr( IO::bg2 )
                          + ( oam[ SPR_HPBAR_OAM + 2 * ( !p_opponent ) + p_pos ].y ) * 128,
                      32 * 256 );

        if( p_pokemon != nullptr && ( !p_pos || _mode == DOUBLE ) && p_pokemon->canBattle( ) ) {

            // pkmn name
            u16 namewd = IO::regularFont->stringWidth( p_pokemon->m_boxdata.m_name );
            if( namewd > 60 ) {
                IO::regularFont->printStringC( p_pokemon->m_boxdata.m_name, anchorx, anchory,
                                               false );
                namewd = IO::regularFont->stringWidthC( p_pokemon->m_boxdata.m_name );
            } else {
                IO::regularFont->printString( p_pokemon->m_boxdata.m_name, anchorx, anchory,
                                              false );
            }

            // Level
            IO::smallFont->setColor( 0, 0 );
            IO::smallFont->setColor( 250, 1 );
            IO::smallFont->setColor( 251, 2 );
            IO::smallFont->printString( ( "!" + std::to_string( p_pokemon->m_level ) ).c_str( ),
                                        anchorx - 2, anchory + 9, false );

            // Gender
            if( p_pokemon->getSpecies( ) != PKMN_NIDORAN_F
                && p_pokemon->getSpecies( ) != PKMN_NIDORAN_M ) {
                if( p_pokemon->m_boxdata.m_isFemale ) {
                    IO::regularFont->setColor( 254, 1 );
                    IO::regularFont->setColor( 255, 2 );
                    IO::regularFont->printString( "}", anchorx + 2 + namewd, anchory, false );
                    IO::regularFont->setColor( 250, 1 );
                    IO::regularFont->setColor( 251, 2 );
                } else if( !p_pokemon->m_boxdata.m_isGenderless ) {
                    IO::regularFont->setColor( 252, 1 );
                    IO::regularFont->setColor( 253, 2 );
                    IO::regularFont->printString( "{", anchorx + 2 + namewd, anchory, false );
                    IO::regularFont->setColor( 250, 1 );
                    IO::regularFont->setColor( 251, 2 );
                }
            }

            // hp
            char buffer[ 10 ];
            snprintf( buffer, 8, "%3d", p_pokemon->m_stats.m_curHP );
            IO::smallFont->printString( buffer, anchorx + 96 - 32 - 28, anchory + 9, false );
            snprintf( buffer, 8, "/%d", p_pokemon->m_stats.m_maxHP );
            IO::smallFont->printString( buffer, anchorx + 96 - 32 - 4, anchory + 9, false );

            // Hp bars

            if( _curHP[ !p_opponent ][ p_pos ] == 101 ) {
                IO::displayHP( 100, 101, hpx, hpy, HP_COL( p_opponent, p_pos ),
                               HP_COL( p_opponent, p_pos ) + 1, false );
                _curHP[ !p_opponent ][ p_pos ] = 100;
            }
            IO::displayHP( _curHP[ !p_opponent ][ p_pos ],
                           100 - p_pokemon->m_stats.m_curHP * 100 / p_pokemon->m_stats.m_maxHP, hpx,
                           hpy, HP_COL( p_opponent, p_pos ), HP_COL( p_opponent, p_pos ) + 1,
                           false );
            _curHP[ !p_opponent ][ p_pos ]
                = 100 - p_pokemon->m_stats.m_curHP * 100 / p_pokemon->m_stats.m_maxHP;

            IO::smallFont->setColor( HP_COL( p_opponent, p_pos ), 1 );
            IO::smallFont->setColor( HP_OUTLINE_COL, 2 );
            IO::smallFont->setColor( HP_COL( p_opponent, p_pos ) + 1, 3 );
            IO::smallFont->printString(
                GET_STRING( 186 ), oam[ SPR_HPBAR_OAM + 2 * ( !p_opponent ) + p_pos ].x + 10,
                oam[ SPR_HPBAR_OAM + 2 * ( !p_opponent ) + p_pos ].y - 6, false ); // HP "icon"
        }

        // Status / shiny
        if( !p_pokemon->m_stats.m_curHP ) {
            IO::loadSprite(
                SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ), SPR_STATUS_ICON_PAL,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 0 ) ].gfxIndex,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].x,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].y, 8, 8,
                0, status_fntTiles, status_fntTilesLen / 2, false, false, false, OBJPRIORITY_0,
                false, OBJMODE_NORMAL );
        } else if( p_pokemon->m_status.m_isParalyzed ) {
            IO::loadSprite(
                SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ), SPR_STATUS_ICON_PAL,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 0 ) ].gfxIndex,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].x,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].y, 8, 8,
                0, status_parTiles, status_parTilesLen / 2, false, false, false, OBJPRIORITY_0,
                false, OBJMODE_NORMAL );
        } else if( p_pokemon->m_status.m_isAsleep ) {
            IO::loadSprite(
                SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ), SPR_STATUS_ICON_PAL,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 0 ) ].gfxIndex,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].x,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].y, 8, 8,
                0, status_slpTiles, status_slpTilesLen / 2, false, false, false, OBJPRIORITY_0,
                false, OBJMODE_NORMAL );
        } else if( p_pokemon->m_status.m_isBadlyPoisoned ) {
            IO::loadSprite(
                SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ), SPR_STATUS_ICON_PAL,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 0 ) ].gfxIndex,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].x,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].y, 8, 8,
                0, status_txcTiles, status_txcTilesLen / 2, false, false, false, OBJPRIORITY_0,
                false, OBJMODE_NORMAL );
        } else if( p_pokemon->m_status.m_isBurned ) {
            IO::loadSprite(
                SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ), SPR_STATUS_ICON_PAL,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 0 ) ].gfxIndex,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].x,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].y, 8, 8,
                0, status_brnTiles, status_brnTilesLen / 2, false, false, false, OBJPRIORITY_0,
                false, OBJMODE_NORMAL );
        } else if( p_pokemon->m_status.m_isFrozen ) {
            IO::loadSprite(
                SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ), SPR_STATUS_ICON_PAL,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 0 ) ].gfxIndex,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].x,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].y, 8, 8,
                0, status_frzTiles, status_frzTilesLen / 2, false, false, false, OBJPRIORITY_0,
                false, OBJMODE_NORMAL );
        } else if( p_pokemon->m_status.m_isPoisoned ) {
            IO::loadSprite(
                SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ), SPR_STATUS_ICON_PAL,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 0 ) ].gfxIndex,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].x,
                IO::OamTop->oamBuffer[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].y, 8, 8,
                0, status_psnTiles, status_psnTilesLen / 2, false, false, false, OBJPRIORITY_0,
                false, OBJMODE_NORMAL );
        } else {
            oam[ SPR_STATUS_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].isHidden = true;
        }

        oam[ SPR_SHINY_ICON_OAM( 2 * ( !p_opponent ) + p_pos ) ].isHidden = !p_pokemon->isShiny( );
        oam[ SPR_STATUSBG_OAM( 2 * ( !p_opponent ) + p_pos ) ].isHidden = false;
        oam[ SPR_STATUSBG_OAM( 2 * ( !p_opponent ) + p_pos ) + 1 ].isHidden = false;
        oam[ SPR_STATUSBG_OAM( 2 * ( !p_opponent ) + p_pos ) + 2 ].isHidden = false;

        IO::updateOAM( false );
    }

    void battleUI::updatePkmn( bool p_opponent, u8 p_pos, pokemon* p_pokemon ) {
        loadPkmnSprite( p_opponent, p_pos, p_pokemon );
        _curHP[ !p_opponent ][ p_pos ] = 101;
        updatePkmnStats( p_opponent, p_pos, p_pokemon );
    }

    void battleUI::loadPkmnSprite( bool p_opponent, u8 p_pos, pokemon* p_pokemon ) {
        SpriteEntry* oam = IO::OamTop->oamBuffer;

        u16 x = ( _mode == SINGLE ) ?
            ( p_opponent ? PKMN_OPP_1_X_SINGLE : PKMN_OWN_1_X_SINGLE )
            : ( ( p_opponent ?
                        ( p_pos ? PKMN_OPP_2_X : PKMN_OPP_1_X )
                       : ( p_pos ? PKMN_OWN_2_X : PKMN_OWN_1_X ) ) );
        u16 y = ( _mode == SINGLE ) ?
            ( p_opponent ? PKMN_OPP_1_Y_SINGLE : PKMN_OWN_1_Y_SINGLE )
            : ( ( p_opponent ?
                        ( p_pos ? PKMN_OPP_2_Y : PKMN_OPP_1_Y )
                       : ( p_pos ? PKMN_OWN_2_Y : PKMN_OWN_1_Y ) ) );

        y += IO::pkmnSpriteHeight( p_pokemon->getSpecies( ) );

        if( p_opponent ) {
            IO::loadPKMNSprite( p_pokemon->getSpecies( ), x, y,
                    SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ),
                    SPR_PKMN_PAL( 2 * ( !p_opponent ) + p_pos ),
                    SPR_PKMN_GFX( 2 * ( !p_opponent ) + p_pos ), false,
                    p_pokemon->isShiny( ), p_pokemon->isFemale( ), false, false,
                    p_pokemon->getForme( ) );
        } else {
            IO::loadPKMNSpriteBack( p_pokemon->getSpecies( ), x, y,
                    SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ),
                    SPR_PKMN_PAL( 2 * ( !p_opponent ) + p_pos ),
                    SPR_PKMN_GFX( 2 * ( !p_opponent ) + p_pos ), false,
                    p_pokemon->isShiny( ), p_pokemon->isFemale( ), false, false,
                    p_pokemon->getForme( ) );

            for( u8 i = 0; i < 4; ++i ) {
                oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + i ].isRotateScale = true;
                oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + i ].isSizeDouble = true;
                oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + i ].rotationIndex = 1;
            }
        }
        for( u8 i = 0; i < 4; ++i ) {
            oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + i ].priority =
                OBJPRIORITY_3;
        }

        u16 emptyPal[ 32 ]                = {0xffff, 0xffff, 0};
        IO::OamTop->matrixBuffer[ 2 ].hdx = ( 1 << 8 );
        IO::OamTop->matrixBuffer[ 2 ].hdy = ( 0 << 8 );
        IO::OamTop->matrixBuffer[ 2 ].vdx = ( 1 << 9 );
        IO::OamTop->matrixBuffer[ 2 ].vdy = ( 1 << 10 );

        IO::OamTop->matrixBuffer[ 3 ].hdx = 187;
        IO::OamTop->matrixBuffer[ 3 ].hdy = ( 0 << 8 );
        IO::OamTop->matrixBuffer[ 3 ].vdx = 187 << 1;
        IO::OamTop->matrixBuffer[ 3 ].vdy = 187 << 2;

        u16 sy = oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 0 ].y + 8
                 - ( IO::pkmnSpriteHeight( p_pokemon->getSpecies( ) ) / 2 ),
            sx = oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 0 ].x + 2
                 - 3 * IO::pkmnSpriteHeight( p_pokemon->getSpecies( ) ) / 4;
        s8 diffx = 80, diffy = 44;
        s8 ske = -23;
        if( !p_opponent ) {
            sy += 4;
            diffx = 97;
            diffy = 48;
            ske = -36;
        }
        IO::loadSprite( SPR_PKMN_SHADOW_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 0,
                        SPR_PKMN_SHADOW_PAL,
                        oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 0 ].gfxIndex,
                        sx, sy, 64, 64, emptyPal, 0, 0,
                        false, false, false, OBJPRIORITY_3, false, OBJMODE_BLENDED );
        IO::loadSprite( SPR_PKMN_SHADOW_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 1,
                        SPR_PKMN_SHADOW_PAL,
                        oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 1 ].gfxIndex,
                        diffx + sx, sy, 32, 64, 0, 0,
                        0, false, false, false, OBJPRIORITY_3, false, OBJMODE_BLENDED );
        IO::loadSprite( SPR_PKMN_SHADOW_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 2,
                        SPR_PKMN_SHADOW_PAL,
                        oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 2 ].gfxIndex,
                        ske + sx, diffy + sy, 64, 32,
                        0, 0, 0, false, false, false, OBJPRIORITY_3, false, OBJMODE_BLENDED );
        IO::loadSprite( SPR_PKMN_SHADOW_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 3,
                        SPR_PKMN_SHADOW_PAL,
                        oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 3 ].gfxIndex,
                        ske + diffx + sx, diffy + sy,
                        32, 32, 0, 0, 0, false, false, false, OBJPRIORITY_3, false,
                        OBJMODE_BLENDED );

        if( !p_opponent ) {
            oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) ].x -= 48;
            oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) ].y -= 48;
            oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 1 ].y -= 48;
            oam[ SPR_PKMN_START_OAM( 2 * ( !p_opponent ) + p_pos ) + 2 ].x -= 48;
        }

        for( u8 i = 0; i < 4; ++i ) {
            oam[ SPR_PKMN_SHADOW_START_OAM( 2 * ( !p_opponent ) + p_pos ) + i ].isRotateScale = true;
            oam[ SPR_PKMN_SHADOW_START_OAM( 2 * ( !p_opponent ) + p_pos ) + i ].isSizeDouble  = true;
            oam[ SPR_PKMN_SHADOW_START_OAM( 2 * ( !p_opponent ) + p_pos ) + i ].rotationIndex
                = 3 - p_opponent;
        }

        IO::updateOAM( false );
    }

    void battleUI::startWildBattle( pokemon* p_pokemon ) {
        SpriteEntry* oam = IO::OamTop->oamBuffer;

        IO::fadeScreen( IO::UNFADE, true, true );
        REG_BLDCNT   = BLEND_ALPHA | BLEND_DST_BG3;
        REG_BLDALPHA = 0xff | ( 0x06 << 8 );
        REG_BLDCNT_SUB   = BLEND_ALPHA | BLEND_DST_BG3;
        REG_BLDALPHA_SUB = 0xff | ( 0x02 << 8 );
        bgUpdate( );
        // Load pkmn sprite
        IO::loadPKMNSprite( p_pokemon->getSpecies( ), WILD_BATTLE_SPRITE_X_START,
                            OPP_PLAT_Y + 35 - 96 + IO::pkmnSpriteHeight( p_pokemon->getSpecies( ) ),
                            SPR_PKMN_START_OAM( 0 ), SPR_PKMN_PAL( 0 ), SPR_PKMN_GFX( 0 ), false,
                            p_pokemon->isShiny( ), p_pokemon->isFemale( ), false, false,
                            p_pokemon->getForme( ) );

        for( u8 i = 0; i < 4; ++i ) {
            oam[ SPR_PKMN_START_OAM( 0 ) + i ].priority = OBJPRIORITY_3;
        }
        u16 emptyPal[ 32 ]                = {0xffff, 0xffff, 0};
        IO::OamTop->matrixBuffer[ 2 ].hdx = ( 1 << 8 );
        IO::OamTop->matrixBuffer[ 2 ].hdy = ( 0 << 8 );
        IO::OamTop->matrixBuffer[ 2 ].vdx = ( 1 << 9 );
        IO::OamTop->matrixBuffer[ 2 ].vdy = ( 1 << 10 );

        u16 sy = oam[ SPR_PKMN_START_OAM( 0 ) + 0 ].y + 8
                 - ( IO::pkmnSpriteHeight( p_pokemon->getSpecies( ) ) / 2 ),
            sx = oam[ SPR_PKMN_START_OAM( 0 ) + 0 ].x + 2
                 - 3 * IO::pkmnSpriteHeight( p_pokemon->getSpecies( ) ) / 4;
        s8 diffx = 80, diffy = 44;
        IO::loadSprite( SPR_PKMN_SHADOW_START_OAM( 0 ) + 0, SPR_PKMN_SHADOW_PAL,
                        oam[ SPR_PKMN_START_OAM( 0 ) + 0 ].gfxIndex, sx, sy, 64, 64, emptyPal, 0, 0,
                        false, false, false, OBJPRIORITY_3, false, OBJMODE_BLENDED );
        IO::loadSprite( SPR_PKMN_SHADOW_START_OAM( 0 ) + 1, SPR_PKMN_SHADOW_PAL,
                        oam[ SPR_PKMN_START_OAM( 0 ) + 1 ].gfxIndex, diffx + sx, sy, 32, 64, 0, 0,
                        0, false, false, false, OBJPRIORITY_3, false, OBJMODE_BLENDED );
        IO::loadSprite( SPR_PKMN_SHADOW_START_OAM( 0 ) + 2, SPR_PKMN_SHADOW_PAL,
                        oam[ SPR_PKMN_START_OAM( 0 ) + 2 ].gfxIndex, -23 + sx, diffy + sy, 64, 32,
                        0, 0, 0, false, false, false, OBJPRIORITY_3, false, OBJMODE_BLENDED );
        IO::loadSprite( SPR_PKMN_SHADOW_START_OAM( 0 ) + 3, SPR_PKMN_SHADOW_PAL,
                        oam[ SPR_PKMN_START_OAM( 0 ) + 3 ].gfxIndex, -23 + diffx + sx, diffy + sy,
                        32, 32, 0, 0, 0, false, false, false, OBJPRIORITY_3, false,
                        OBJMODE_BLENDED );

        for( u8 i = 0; i < 4; ++i ) {
            oam[ SPR_PKMN_SHADOW_START_OAM( 0 ) + i ].isRotateScale = true;
            oam[ SPR_PKMN_SHADOW_START_OAM( 0 ) + i ].isSizeDouble  = true;
            oam[ SPR_PKMN_SHADOW_START_OAM( 0 ) + i ].rotationIndex = 2;
        }

        IO::updateOAM( false );

        char buffer[ 50 ];
        snprintf( buffer, 49, GET_STRING( 394 ), p_pokemon->m_boxdata.m_name );
        log( std::string( buffer ) );

        for( u16 i = WILD_BATTLE_SPRITE_X_START; i < WILD_BATTLE_SPRITE_X; ++i ) {
            for( u8 j = 0; j < 4; ++j ) {
                oam[ SPR_PKMN_START_OAM( 0 ) + j ].x++;
                oam[ SPR_PKMN_SHADOW_START_OAM( 0 ) + j ].x++;
            }
            IO::updateOAM( false );
            swiWaitForVBlank( );
        }

        SOUND::playCry( p_pokemon->getSpecies( ), p_pokemon->getForme( ),
                        p_pokemon->isFemale( ) );

        for( u8 i = 0; i < 30; ++i ) { swiWaitForVBlank( ); }

        if( p_pokemon->isShiny( ) ) {
            animateShiny( true, 0, p_pokemon->m_boxdata.m_shinyType );
        }
        _curHP[ 0 ][ 0 ] = 101;
        updatePkmnStats( true, 0, p_pokemon );
    }

    void battleUI::startTrainerBattle( battleTrainer* p_trainer ) {
        (void) p_trainer;

        // TODO
    }

    void battleUI::sendOutPkmn( bool p_opponent, u8 p_pos, pokemon* p_pokemon,
                                battleTrainer* p_trainer ) {
        char buffer[ 100 ];
        if( p_opponent ) {
            (void) p_trainer;
//            snprintf( buffer, 99, GET_STRING( 264 ),
//                    getTrainerClassName( p_trainer->getClass( ) ).c_str( ),
//                    p_trainer->m_strings.m_name, p_pokemon->m_boxdata.m_name );
//            log( std::string( buffer ) );
        } else {
            snprintf( buffer, 99, GET_STRING( 395 ), p_pokemon->m_boxdata.m_name );
            log( std::string( buffer ) );
        }

        // play pokeball animation
        animateBallRelease( p_opponent, p_pos, p_pokemon->getBall( ) );
        loadPkmnSprite( p_opponent, p_pos, p_pokemon );
        for( u8 i = 0; i < 30; ++i ) { swiWaitForVBlank( ); }

        SOUND::playCry( p_pokemon->getSpecies( ), p_pokemon->getForme( ),
                        p_pokemon->isFemale( ) );

        for( u8 i = 0; i < 30; ++i ) { swiWaitForVBlank( ); }

        if( p_pokemon->isShiny( ) ) {
            animateShiny( p_opponent, p_pos, p_pokemon->m_boxdata.m_shinyType );
        }

        _curHP[ !p_opponent ][ p_pos ] = 101;
        updatePkmnStats( p_opponent, p_pos, p_pokemon );
    }

    void battleUI::showMoveSelection( pokemon* p_pokemon, u8 p_slot, u8 p_highlightedButton ) {
        SpriteEntry* oam = IO::Oam->oamBuffer;
        char buffer[ 100 ];

        if( p_highlightedButton == u8( -1 ) ) {
            // initialize stuff
            FS::readPictureData( bgGetGfxPtr( IO::bg3sub ), "nitro:/PICS/", "battlesub", 512,
                    49152, true );
            for( u8 i = 0; i < 2; ++i ) {
                u16* pal             = IO::BG_PAL( i );
                pal[ 0 ] = 0;
                pal[ 250 ] = IO::WHITE;
                pal[ 251 ]  = IO::GRAY;
                pal[ 252 ]  = IO::RGB( 18, 22, 31 );
                pal[ 254 ]   = IO::RGB( 31, 18, 18 );
                pal[ 253 ] = IO::RGB( 0, 0, 25 );
                pal[ 255 ]  = IO::RGB( 23, 0, 0 );
            }

            // Clear log window
            dmaFillWords( 0, bgGetGfxPtr( IO::bg2sub ), 256 * 192 );
            for( u8 i = 0; i < 12; ++i ) {
                oam[ SPR_LARGE_MESSAGE_OAM_SUB + i ].isHidden = true;
            }


            for( u8 i = 0; i < 8; ++i ) {
                oam[ SPR_SMALL_MESSAGE_OAM_SUB + i ].isHidden = false;
            }

            for( u8 i = 0; i < 2; ++i ) {
                oam[ SPR_BATTLE_FITE_OAM_SUB + i].isHidden = false;
                oam[ SPR_BATTLE_PKMN_OAM_SUB + i].isHidden = false;
                oam[ SPR_BATTLE_RUN_OAM_SUB + i].isHidden = false;
                oam[ SPR_BATTLE_BAG_OAM_SUB + i].isHidden = false;
            }

            if( !_isWildBattle && !p_slot ) {
                IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB + 1, SPR_BATTLE_RUN_PAL_SUB,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].gfxIndex,
                        128, 68 + 64 + 18, 32, 32, battle_inactivePal, 0, 0, true, true, false,
                        OBJPRIORITY_3, true, OBJMODE_BLENDED );
            } else {
                IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB + 1, SPR_BATTLE_RUN_PAL_SUB,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].gfxIndex,
                        128, 68 + 64 + 18, 32, 32, battle_runPal, 0, 0, true, true, false,
                        OBJPRIORITY_3, true, OBJMODE_BLENDED );

            }

            IO::loadPKMNIcon(
                    p_pokemon->getSpecies( ), oam[ SPR_BATTLE_ICON_OAM_SUB ].x,
                    oam[ SPR_BATTLE_ICON_OAM_SUB ].y, SPR_BATTLE_ICON_OAM_SUB,
                    SPR_BATTLE_ICON_PAL_SUB, oam[ SPR_BATTLE_ICON_OAM_SUB ].gfxIndex,
                    true, p_pokemon->getForme( ), p_pokemon->isShiny( ),
                    p_pokemon->isFemale( ) );

            for( u8 i = 0; i < 4; ++i ) {
                oam[ SPR_TYPE_OAM_SUB( i ) ].isHidden = true;
                oam[ SPR_DMG_CAT_OAM_SUB( i ) ].isHidden = true;
                for( u8 j = 0; j < 6; j++ ) {
                    oam[ SPR_MOVE_OAM_SUB( i ) + j ].isHidden = true;
                }
            }
            IO::updateOAM( true );

            IO::regularFont->printString( GET_STRING( 264 ), 128,
                    oam[ SPR_BATTLE_FITE_OAM_SUB].y + 34, true, IO::font::CENTER );
            IO::regularFont->printStringC( GET_STRING( 265 ),
                    oam[ SPR_BATTLE_PKMN_OAM_SUB ].x + 32,
                    oam[ SPR_BATTLE_PKMN_OAM_SUB].y + 9, true, IO::font::CENTER );
            if( _isWildBattle || !p_slot ) {
                IO::regularFont->printStringC( GET_STRING( 266 ),
                        oam[ SPR_BATTLE_RUN_OAM_SUB ].x + 32,
                        oam[ SPR_BATTLE_RUN_OAM_SUB].y + 9, true, IO::font::CENTER );
            } else {
                IO::regularFont->printStringC( GET_STRING( 268 ),
                        oam[ SPR_BATTLE_RUN_OAM_SUB ].x + 32,
                        oam[ SPR_BATTLE_RUN_OAM_SUB].y + 9, true, IO::font::CENTER );
            }
            IO::regularFont->printStringC( GET_STRING( 267 ),
                    oam[ SPR_BATTLE_BAG_OAM_SUB ].x + 32,
                    oam[ SPR_BATTLE_BAG_OAM_SUB].y + 9, true, IO::font::CENTER );

            snprintf( buffer, 99, GET_STRING( 162 ), p_pokemon->m_boxdata.m_name );
            IO::regularFont->printStringC( buffer,
                    128, oam[ SPR_SMALL_MESSAGE_OAM_SUB ].y + 4, true, IO::font::CENTER );
        }
        if( p_highlightedButton == 0 ) {
            IO::loadSprite( SPR_BATTLE_FITE_OAM_SUB + 1, SPR_BATTLE_FITE_PAL_SUB,
                    oam[ SPR_BATTLE_FITE_OAM_SUB + 1 ].gfxIndex,
                    oam[ SPR_BATTLE_FITE_OAM_SUB + 1 ].x,
                    oam[ SPR_BATTLE_FITE_OAM_SUB + 1 ].y,
                    64, 64, battle_fite_2Pal, 0, 0, true, true, false,
                    OBJPRIORITY_3, true, OBJMODE_BLENDED );
        } else {
            IO::loadSprite( SPR_BATTLE_FITE_OAM_SUB + 1, SPR_BATTLE_FITE_PAL_SUB,
                    oam[ SPR_BATTLE_FITE_OAM_SUB + 1 ].gfxIndex,
                    oam[ SPR_BATTLE_FITE_OAM_SUB + 1 ].x,
                    oam[ SPR_BATTLE_FITE_OAM_SUB + 1 ].y,
                    64, 64, battle_fitePal, 0, 0, true, true, false,
                    OBJPRIORITY_3, true, OBJMODE_BLENDED );
        }
        if( p_highlightedButton == 1 ) {
            IO::loadSprite( SPR_BATTLE_PKMN_OAM_SUB + 1, SPR_BATTLE_PKMN_PAL_SUB,
                    oam[ SPR_BATTLE_PKMN_OAM_SUB + 1 ].gfxIndex,
                    oam[ SPR_BATTLE_PKMN_OAM_SUB + 1 ].x,
                    oam[ SPR_BATTLE_PKMN_OAM_SUB + 1 ].y,
                    32, 32, battle_pkmn_2Pal, 0, 0, true, true, false,
                    OBJPRIORITY_3, true, OBJMODE_BLENDED );
        } else {
            IO::loadSprite( SPR_BATTLE_PKMN_OAM_SUB + 1, SPR_BATTLE_PKMN_PAL_SUB,
                    oam[ SPR_BATTLE_PKMN_OAM_SUB + 1 ].gfxIndex,
                    oam[ SPR_BATTLE_PKMN_OAM_SUB + 1 ].x,
                    oam[ SPR_BATTLE_PKMN_OAM_SUB + 1 ].y,
                    32, 32, battle_pkmnPal, 0, 0, true, true, false,
                    OBJPRIORITY_3, true, OBJMODE_BLENDED );
        }
        if( p_highlightedButton == 2 ) {
            if ( !p_slot && !_isWildBattle ) {
                IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB + 1, SPR_BATTLE_RUN_PAL_SUB,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].gfxIndex,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].x,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].y,
                        32, 32, battle_inactive_2Pal, 0, 0, true, true, false,
                        OBJPRIORITY_3, true, OBJMODE_BLENDED );
            } else {
                IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB + 1, SPR_BATTLE_RUN_PAL_SUB,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].gfxIndex,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].x,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].y,
                        32, 32, battle_run_2Pal, 0, 0, true, true, false,
                        OBJPRIORITY_3, true, OBJMODE_BLENDED );
            }
        } else {
            if ( !p_slot && !_isWildBattle ) {
                IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB + 1, SPR_BATTLE_RUN_PAL_SUB,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].gfxIndex,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].x,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].y,
                        32, 32, battle_inactivePal, 0, 0, true, true, false,
                        OBJPRIORITY_3, true, OBJMODE_BLENDED );
            } else {
                IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB + 1, SPR_BATTLE_RUN_PAL_SUB,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].gfxIndex,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].x,
                        oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].y,
                        32, 32, battle_runPal, 0, 0, true, true, false,
                        OBJPRIORITY_3, true, OBJMODE_BLENDED );
            }
        }
        if( p_highlightedButton == 3 ) {
            IO::loadSprite( SPR_BATTLE_BAG_OAM_SUB + 1, SPR_BATTLE_BAG_PAL_SUB,
                    oam[ SPR_BATTLE_BAG_OAM_SUB + 1 ].gfxIndex,
                    oam[ SPR_BATTLE_BAG_OAM_SUB + 1 ].x,
                    oam[ SPR_BATTLE_BAG_OAM_SUB + 1 ].y,
                    32, 32, battle_bag_2Pal, 0, 0, true, true, false,
                    OBJPRIORITY_3, true, OBJMODE_BLENDED );
        } else {
            IO::loadSprite( SPR_BATTLE_BAG_OAM_SUB + 1, SPR_BATTLE_BAG_PAL_SUB,
                    oam[ SPR_BATTLE_BAG_OAM_SUB + 1 ].gfxIndex,
                    oam[ SPR_BATTLE_BAG_OAM_SUB + 1 ].x,
                    oam[ SPR_BATTLE_BAG_OAM_SUB + 1 ].y,
                    32, 32, battle_bagPal, 0, 0, true, true, false,
                    OBJPRIORITY_3, true, OBJMODE_BLENDED );
        }
    }

    void battleUI::showAttackSelection( pokemon* p_pokemon, bool p_canUseMove[ 4 ],
            bool p_showMegaEvolution, u8 p_highlightedButton,
            bool p_megaButtonActive ) {

        SpriteEntry* oam = IO::Oam->oamBuffer;
        char buffer[ 100 ];

        if( p_highlightedButton == u8( -1 ) ) {
            // initialize stuff
            FS::readPictureData( bgGetGfxPtr( IO::bg3sub ), "nitro:/PICS/", "battlesub3", 512,
                    49152, true );
            for( u8 i = 0; i < 2; ++i ) {
                u16* pal             = IO::BG_PAL( i );
                pal[ 0 ] = 0;
                pal[ 250 ] = IO::WHITE;
                pal[ 251 ]  = IO::GRAY;
                pal[ 252 ]  = IO::RGB( 18, 22, 31 );
                pal[ 254 ]   = IO::RGB( 31, 18, 18 );
                pal[ 253 ] = IO::RGB( 0, 0, 25 );
                pal[ 255 ]  = IO::RGB( 23, 0, 0 );
            }

            // Clear log window
            dmaFillWords( 0, bgGetGfxPtr( IO::bg2sub ), 256 * 192 );
            for( u8 i = 0; i < 12; ++i ) {
                oam[ SPR_LARGE_MESSAGE_OAM_SUB + i ].isHidden = true;
            }


            for( u8 i = 0; i < 8; ++i ) {
                oam[ SPR_SMALL_MESSAGE_OAM_SUB + i ].isHidden = false;
            }

            for( u8 i = 0; i < 2; ++i ) {
                oam[ SPR_BATTLE_FITE_OAM_SUB + i].isHidden = true;
                oam[ SPR_BATTLE_PKMN_OAM_SUB + i].isHidden = true;
                oam[ SPR_BATTLE_RUN_OAM_SUB + i].isHidden = false;
                oam[ SPR_BATTLE_BAG_OAM_SUB + i].isHidden = true;
            }

            oam[ SPR_BATTLE_ICON_OAM_SUB ].isHidden = true;

            IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB + 1, SPR_BATTLE_RUN_PAL_SUB,
                    oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].gfxIndex,
                    128, 68 + 64 + 18, 32, 32, battle_runPal, 0, 0, true, true, false,
                    OBJPRIORITY_3, true, OBJMODE_BLENDED );
            // Moves
            IO::regularFont->setColor( IO::WHITE_IDX, 1 );
            IO::regularFont->setColor( 0, 2 );
            IO::smallFont->setColor( IO::WHITE_IDX, 1 );
            IO::smallFont->setColor( 0, 2 );
            auto mdatas = std::vector<MOVE::moveData>( );
            for( int i = 0; i < 4; i++ ) {
                oam[ SPR_TYPE_OAM_SUB( i ) ].isHidden = !p_pokemon->getMove( i );
                oam[ SPR_DMG_CAT_OAM_SUB( i ) ].isHidden = !p_pokemon->getMove( i );
                for( u8 j = 0; j < 6; j++ ) {
                    oam[ SPR_MOVE_OAM_SUB( i ) + j ].isHidden = !p_pokemon->getMove( i );
                }

                if( !p_pokemon->getMove( i ) ) continue;
                auto mdata = MOVE::getMoveData( p_pokemon->getMove( i ) );
                mdatas.push_back( mdata );

                type t;
                if( p_pokemon->getMove( i ) != M_HIDDEN_POWER ) {
                    t          = mdata.m_type;
                } else {
                    t = p_pokemon->getHPType( );
                }
                IO::loadTypeIcon( t, oam[ SPR_TYPE_OAM_SUB( i ) ].x,
                        oam[ SPR_TYPE_OAM_SUB( i ) ].y, SPR_TYPE_OAM_SUB( i ),
                        SPR_TYPE_PAL_SUB( i ), oam[ SPR_TYPE_OAM_SUB( i ) ].gfxIndex,
                        true, CURRENT_LANGUAGE );
                IO::loadDamageCategoryIcon( mdata.m_category, oam[ SPR_DMG_CAT_OAM_SUB( i ) ].x,
                        oam[ SPR_DMG_CAT_OAM_SUB( i ) ].y, SPR_DMG_CAT_OAM_SUB( i ),
                        SPR_DMG_CAT_PAL_SUB( i ), oam[ SPR_DMG_CAT_OAM_SUB( i ) ].gfxIndex,
                        true );
                IO::copySpritePal( movebox4Pal + 4, SPR_TYPE_PAL_SUB( i ), 4, 2 * 4, true );
            }

            IO::updateOAM( true );

            IO::regularFont->printStringC( GET_STRING( 268 ),
                    oam[ SPR_BATTLE_RUN_OAM_SUB ].x + 32,
                    oam[ SPR_BATTLE_RUN_OAM_SUB].y + 9, true, IO::font::CENTER );

            IO::regularFont->printStringC( GET_STRING( 49 ),
                    128, oam[ SPR_SMALL_MESSAGE_OAM_SUB ].y + 4, true, IO::font::CENTER );

            for( int i = 0; i < 4; i++ ) {
                if( !p_pokemon->getMove( i ) ) continue;
                auto mname = MOVE::getMoveName( p_pokemon->getMove( i ) );
                if( mname.length( ) > 18 ) {
                    snprintf( buffer, 20, "%s.", mname.c_str( ) );
                } else {
                    snprintf( buffer, 20, "%s", mname.c_str( ) );
                }

                if( !p_canUseMove[ i ] ) {
                    IO::regularFont->setColor( IO::RED_IDX, 1 );
                } else {
                    IO::regularFont->setColor( IO::WHITE_IDX, 1 );
                }
                IO::regularFont->printStringC( buffer, oam[ SPR_MOVE_OAM_SUB( i ) ].x + 48,
                                               oam[ SPR_MOVE_OAM_SUB( i ) ].y + 7, true,
                                               IO::font::CENTER );

                snprintf(
                    buffer, 49, GET_STRING( 377 ), p_pokemon->m_boxdata.m_curPP[ i ],
                    s8( mdatas[ i ].m_pp * ( ( 5 + p_pokemon->m_boxdata.PPupget( i ) ) / 5.0 ) ) );
                IO::smallFont->printStringC( buffer, oam[ SPR_MOVE_OAM_SUB( i ) ].x + 84,
                                             oam[ SPR_MOVE_OAM_SUB( i ) ].y + 13, true,
                                             IO::font::RIGHT );
            }
            IO::regularFont->setColor( IO::WHITE_IDX, 1 );


        }

        for( u8 i = 0; i < 4; ++i ) {
            oam[ SPR_TYPE_OAM_SUB( i ) ].isHidden = !p_pokemon->getMove( i );
            oam[ SPR_DMG_CAT_OAM_SUB( i ) ].isHidden = !p_pokemon->getMove( i );
            IO::copySpritePal( movebox4Pal + 4, SPR_TYPE_PAL_SUB( i ), 4, 2 * 4, true );
        }

        if( p_highlightedButton < 4 ) {
            IO::copySpritePal( movebox3Pal + 4, SPR_TYPE_PAL_SUB( p_highlightedButton ),
                    4, 2 * 4, true );
        }

        if( p_highlightedButton == 4 ) {
            IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB + 1, SPR_BATTLE_RUN_PAL_SUB,
                    oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].gfxIndex,
                    oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].x,
                    oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].y,
                    32, 32, battle_run_2Pal, 0, 0, true, true, false,
                    OBJPRIORITY_3, true, OBJMODE_BLENDED );
        } else {
            IO::loadSprite( SPR_BATTLE_RUN_OAM_SUB + 1, SPR_BATTLE_RUN_PAL_SUB,
                    oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].gfxIndex,
                    oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].x,
                    oam[ SPR_BATTLE_RUN_OAM_SUB + 1 ].y,
                    32, 32, battle_runPal, 0, 0, true, true, false,
                    OBJPRIORITY_3, true, OBJMODE_BLENDED );
        }

        IO::updateOAM( true );
    }

} // namespace BATTLE
