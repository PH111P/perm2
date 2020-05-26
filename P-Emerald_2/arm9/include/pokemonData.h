/*
Pokémon neo
------------------------------

file        : pokemon.h
author      : Philip Wellnitz
description : Header file. Consult the corresponding source file for details.

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

#pragma once

#include <map>
#include <string>
#include <vector>
#include <nds.h>
#include "type.h"

#undef RAND_MAX
#define RAND_MAX 4294967295

enum pkmnLevelUpType {
    MEDIUM_FAST = 0,
    ERRATIC     = 1,
    FLUCTUATING = 2,
    MEDIUM_SLOW = 3,
    FAST        = 4,
    SLOW        = 5
};
enum pkmnColorType {
    RED_    = 0,
    BLUE_   = 1,
    YELLOW_ = 2,
    GREEN_  = 3,
    BLACK_  = 4,
    BROWN_  = 5,
    PURPLE_ = 6,
    GRAY_   = 7,
    WHITE_  = 8,
    PINK_   = 9
};
enum pkmnGenderType {
    GENDERLESS = 255,
    FEMALE     = 254,
    MALE_125   = 223,
    MALE_250   = 191,
    MALE_500   = 127,
    MALE_750   = 63,
    MALE_875   = 31,
    MALE       = 0
};
enum pkmnEggType {
    NONE_         = 0,
    MONSTER_      = 1,
    WASSER1_      = 2,
    WATER1_       = 2,
    KAEFER_       = 4,
    BUG_          = 4,
    FLUG_         = 5,
    FLYING_       = 5,
    FELD_         = 6,
    FIELD_        = 6,
    FEE_          = 7,
    FAIRY_        = 7,
    PFLANZE_      = 8,
    GRASS_        = 8,
    HUMANOTYP_    = 9,
    HUMANLIKE_    = 9,
    WASSER3_      = 10,
    WATER3_       = 10,
    MINERAL_      = 11,
    AMORPH_       = 12,
    WASSER2_      = 13,
    WATER2_       = 13,
    DITTO_        = 14,
    DRACHE_       = 15,
    DRAGON_       = 15,
    UNBEKANNT_    = 16,
    UNDISCOVERED_ = 16
};

enum pkmnNatures {
    ROBUST  = 0,
    HARDY   = 0,
    EINSAM  = 1,
    LONELY  = 1,
    MUTIG   = 2,
    BRAVE   = 2,
    HART    = 3,
    ADAMANT = 3,
    FRECH   = 4,
    NAUGHY  = 4,
    KUEHN   = 5,
    BOLD    = 5,
    SANFT   = 6,
    DOCILE  = 6,
    LOCKER  = 7,
    RELAXED = 7,
    PFIFFIG = 8,
    IMPISH  = 8,
    LASCH   = 9,
    LAX     = 9,
    SCHEU   = 10,
    TIMID   = 10,
    HASTIG  = 11,
    NASTY   = 11,
    ERNST   = 12,
    SERIOUS = 12,
    FROH    = 13,
    JOLLY   = 13,
    NAIV    = 14,
    NAIVE   = 14,
    MAESSIG = 15,
    MODEST  = 15,
    MILD    = 16,
    RUHIG   = 17,
    QUIET   = 17,
    ZAGHAFT = 18,
    BASHFUL = 18,
    HITZIG  = 19,
    RASH    = 19,
    STILL   = 20,
    CALM    = 20,
    ZART    = 21,
    GENTLE  = 21,
    FORSCH  = 22,
    SASSY   = 22,
    SACHT   = 23,
    CAREFUL = 23,
    KAUZIG  = 24,
    QUIRKY  = 24
};

struct pkmnFormeData {
    type           m_types[ 2 ];
    u16            m_abilities[ 4 ]; // abilities
    u8             m_bases[ 6 ];     // base values (hp, atk, def, satk, sdef, spd)
    u16            m_expYield;
    pkmnGenderType m_genderRatio;  // from pkmnGenderType
    u8             m_size;         // in dm
    u16            m_weight;       // in 100g
    u8             m_colorShape;   // (color << 4) | shape
    u8             m_evYield[ 6 ]; // (hp, atk, def, satk, sdef, spd)
    u16            m_items[ 4 ];   // possible held items: 1%, 5%, 50%, 100%
    u8             m_eggGroups;    // (eg1 << 4) | eg2;
};

struct pkmnData {
    pkmnFormeData m_baseForme;
    u8            m_expTypeFormeCnt; // (ExpType << 5) | FormeCnt
    u8            m_eggCycles;
    u8            m_catchrate;
    u8            m_baseFriend;

    inline u8 getExpType( ) const {
        return m_expTypeFormeCnt >> 5;
    }
    inline u8 getFormeCnt( ) const {
        return m_expTypeFormeCnt & 0xF;
    }
};

// deprecated
struct pokemonData {
    type           m_types[ 2 ];
    u16            m_bases[ 6 ];
    u16            m_catchrate;
    u16            m_items[ 4 ];
    pkmnGenderType m_gender;
    pkmnEggType    m_eggT[ 2 ];
    u16            m_eggcyc;
    u16            m_baseFriend;
    u16            m_EVYield[ 6 ];
    u16            m_EXPYield;
    u16            m_formecnt;
    u16            m_size;
    u16            m_weight;
    u16            m_expType;
    u16            m_abilities[ 4 ];

    union {
        struct evolvestruct {
            s16 m_evolvesInto;
            s16 m_evolutionTrigger;
            s16 m_evolveItem;
            s16 m_evolveLevel;
            s16 m_evolveGender;
            s16 m_evolveLocation;
            s16 m_evolveHeldItem;
            s16 m_evolveDayTime;
            s16 m_evolveKnownMove;
            s16 m_evolveKnownMoveType;
            s16 m_evolveFriendship;
            s16 m_evolveMinimumBeauty;
            s16 m_evolveAtkDefRelation; // 1: >; 2: ==; 3 <
            s16 m_evolveAdditionalPartyMember;
            s16 m_evolveTradeSpecies;
        } m_e;
        s16 m_evolveData[ 15 ];
    } m_evolutions[ 7 ];
    char m_displayName[ 15 ];
    char m_species[ 50 ];
    char m_dexEntry[ 200 ];

    u16  m_formeIdx[ 30 ];
    char m_formeName[ 30 ][ 15 ];

    u16 m_preEvolution;
    u8  m_stage; // 0: Basic, 1: Stage 1, 2: Stage 2, 3 Restored (m_preEvolution: itemIdx)
};

std::string getDisplayName( u16 p_pkmnId, u8 p_language, u8 p_forme );
std::string getDisplayName( u16 p_pkmnId, u8 p_forme = 0 );
bool        getDisplayName( u16 p_pkmnId, char* p_name, u8 p_language, u8 p_forme = 0 );

pkmnData getPkmnData( const u16 p_pkmnId, const u8 p_forme = 0 );
bool     getPkmnData( const u16 p_pkmnId, pkmnData* p_out );
bool     getPkmnData( const u16 p_pkmnId, const u8 p_forme, pkmnData* p_out );

[[deprecated]] bool getAll( u16 p_pkmnId, pokemonData& out, u8 p_forme = 0 );

const u16 LEARN_TM = 200;
const u16 LEARN_TUTOR = 201;
const u16 LEARN_EGG = 202;

void getLearnMoves( u16 p_pkmnId, u16 p_fromLevel, u16 p_toLevel, u16 p_num, u16* p_res );
bool canLearn( u16 p_pkmnId, u16 p_moveId, u16 p_maxLevel );

constexpr u8 NatMod[ 25 ][ 5 ]
    = {{10, 10, 10, 10, 10}, {11, 9, 10, 10, 10}, {11, 10, 9, 10, 10},
       {11, 10, 10, 9, 10}, {11, 10, 10, 10, 9}, {9, 11, 10, 10, 10},
       {10, 10, 10, 10, 10}, {10, 11, 9, 10, 10}, {10, 11, 10, 9, 10},
       {10, 11, 10, 10, 9}, {9, 10, 11, 10, 10}, {10, 9, 11, 10, 10},
       {10, 10, 10, 10, 10}, {10, 10, 11, 9, 10}, {10, 10, 11, 10, 9},
       {9, 10, 10, 11, 10}, {10, 9, 10, 11, 10}, {10, 10, 9, 11, 10},
       {10, 10, 10, 10, 10}, {10, 10, 10, 11, 9}, {9, 10, 10, 10, 11},
       {10, 9, 10, 10, 11}, {10, 10, 9, 10, 11}, {10, 10, 10, 9, 11},
       {10, 10, 10, 10, 10}};

constexpr u32 EXP[ 100 ][ 6 ] = {{0, 0, 0, 0, 0, 0},
                       {15, 6, 8, 9, 10, 4},
                       {52, 21, 27, 57, 33, 13},
                       {122, 51, 64, 96, 80, 32},
                       {237, 100, 125, 135, 156, 65},
                       {406, 172, 216, 179, 270, 112},
                       {637, 274, 343, 236, 428, 178},
                       {942, 409, 512, 314, 640, 276},
                       {1326, 583, 729, 419, 911, 393},
                       {1800, 800, 1000, 560, 1250, 540},
                       {2369, 1064, 1331, 742, 1663, 745},
                       {3041, 1382, 1728, 973, 2160, 967},
                       {3822, 1757, 2197, 1261, 2746, 1230},
                       {4719, 2195, 2744, 1612, 3430, 1591},
                       {5737, 2700, 3375, 2035, 4218, 1957},
                       {6881, 3276, 4096, 2535, 5120, 2457},
                       {8155, 3930, 4913, 3120, 6141, 3046},
                       {9564, 4665, 5832, 3798, 7290, 3732},
                       {11111, 5487, 6859, 4575, 8573, 4526},
                       {12800, 6400, 8000, 5460, 10000, 5440},
                       {14632, 7408, 9261, 6458, 11576, 6482},
                       {16610, 8518, 10648, 7577, 13310, 7666},
                       {18737, 9733, 12167, 8825, 15208, 9003},
                       {21012, 11059, 13824, 10208, 17280, 10506},
                       {23437, 12500, 15625, 11735, 19531, 12187},
                       {26012, 14060, 17576, 13411, 21970, 14060},
                       {28737, 15746, 19683, 15244, 24603, 16140},
                       {31610, 17561, 21952, 17242, 27440, 18439},
                       {34632, 19511, 24389, 19411, 30486, 20974},
                       {37800, 21600, 27000, 21760, 33750, 23760},
                       {41111, 23832, 29791, 24294, 37238, 26811},
                       {44564, 26214, 32768, 27021, 40960, 30146},
                       {48155, 28749, 35937, 29949, 44921, 33780},
                       {51881, 31443, 39304, 33084, 49130, 37731},
                       {55737, 34300, 42875, 36435, 53593, 42017},
                       {59719, 37324, 46656, 40007, 58320, 46656},
                       {63822, 40522, 50653, 43808, 63316, 50653},
                       {68041, 43897, 54872, 47846, 68590, 55969},
                       {72369, 47455, 59319, 52127, 74148, 60505},
                       {76800, 51200, 64000, 56660, 80000, 66560},
                       {81326, 55136, 68921, 61450, 86151, 71677},
                       {85942, 59270, 74088, 66505, 92610, 78533},
                       {90637, 63605, 79507, 71833, 99383, 84277},
                       {95406, 68147, 85184, 77440, 106480, 91998},
                       {100237, 72900, 91125, 83335, 113906, 98415},
                       {105122, 77868, 97336, 89523, 121670, 107069},
                       {110052, 83058, 103823, 96012, 129778, 114205},
                       {115015, 88473, 110592, 102810, 138240, 123863},
                       {120001, 94119, 117649, 109923, 147061, 131766},
                       {125000, 100000, 125000, 117360, 156250, 142500},
                       {131324, 106120, 132651, 125126, 165813, 151222},
                       {137795, 112486, 140608, 133229, 175760, 163105},
                       {144410, 119101, 148877, 141677, 186096, 172697},
                       {151165, 125971, 157464, 150476, 196830, 185807},
                       {158056, 133100, 166375, 159635, 207968, 196322},
                       {165079, 140492, 175616, 169159, 219520, 210739},
                       {172229, 148154, 185193, 179056, 231491, 222231},
                       {179503, 156089, 195112, 189334, 243890, 238036},
                       {186894, 164303, 205379, 199999, 256723, 250562},
                       {194400, 172800, 216000, 211060, 270000, 267840},
                       {202013, 181584, 226981, 222522, 283726, 281456},
                       {209728, 190662, 238328, 234393, 297910, 300293},
                       {217540, 200037, 250047, 246681, 312558, 315059},
                       {225443, 209715, 262144, 259392, 327680, 335544},
                       {233431, 219700, 274625, 272535, 343281, 351520},
                       {241496, 229996, 287496, 286115, 359370, 373744},
                       {249633, 240610, 300763, 300140, 375953, 390991},
                       {257834, 251545, 314432, 314618, 393040, 415050},
                       {267406, 262807, 328509, 329555, 410636, 433631},
                       {276458, 274400, 343000, 344960, 428750, 459620},
                       {286328, 286328, 357911, 360838, 447388, 479600},
                       {296358, 298598, 373248, 377197, 466560, 507617},
                       {305767, 311213, 389017, 394045, 486271, 529063},
                       {316074, 324179, 405224, 411388, 506530, 559209},
                       {326531, 337500, 421875, 429235, 527343, 582187},
                       {336255, 351180, 438976, 447591, 548720, 614566},
                       {346965, 365226, 456533, 466464, 570666, 639146},
                       {357812, 379641, 474552, 485862, 593190, 673863},
                       {367807, 394431, 493039, 505791, 616298, 700115},
                       {378880, 409600, 512000, 526260, 640000, 737280},
                       {390077, 425152, 531441, 547274, 664301, 765275},
                       {400293, 441094, 551368, 568841, 689210, 804997},
                       {411686, 457429, 571787, 590969, 714733, 834809},
                       {423190, 474163, 592704, 613664, 740880, 877201},
                       {433572, 491300, 614125, 636935, 767656, 908905},
                       {445239, 508844, 636056, 660787, 795070, 954084},
                       {457001, 526802, 658503, 685228, 823128, 987754},
                       {467489, 545177, 681472, 710266, 851840, 1035837},
                       {479378, 563975, 704969, 735907, 881211, 1071552},
                       {491346, 583200, 729000, 762160, 911250, 1122660},
                       {501878, 602856, 753571, 789030, 941963, 1160499},
                       {513934, 622950, 778688, 816525, 973360, 1214753},
                       {526049, 643485, 804357, 844653, 1005446, 1254796},
                       {536557, 664467, 830584, 873420, 1038230, 1312322},
                       {548720, 685900, 857375, 902835, 1071718, 1354652},
                       {560922, 707788, 884736, 932903, 1105920, 1415577},
                       {571333, 730138, 912673, 963632, 1140841, 1460276},
                       {583539, 752953, 941192, 995030, 1176490, 1524731},
                       {591882, 776239, 970299, 1027103, 1212873, 1571884},
                       {600000, 800000, 1000000, 1059860, 1250000, 1640000}};
