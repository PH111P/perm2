/*
    Pokémon neo
    ------------------------------

    file        : item.h
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
#include <functional>
#include <string>
#include "pokemon.h"
#include "itemNames.h"

namespace ITEM {
    const u8 ITEMTYPE_POKEBALL    = 1;
    const u8 ITEMTYPE_MEDICINE    = 2;
    const u8 ITEMTYPE_BATTLEITEM  = 3;
    const u8 ITEMTYPE_COLLECTIBLE = 4;
    const u8 ITEMTYPE_USABLE      = 5;
    const u8 ITEMTYPE_EVOLUTION   = 6;
    const u8 ITEMTYPE_FORMECHANGE = 8;
    const u8 ITEMTYPE_KEYITEM     = 9;
    const u8 ITEMTYPE_TM          = 12;
    const u8 ITEMTYPE_APRICORN    = 13;
    const u8 ITEMTYPE_BERRY       = 16;
    const u8 ITEMTYPE_HOLD        = 32;

    struct itemData {
        u8  m_itemType;
        u8  m_effect; // Effect index
        u16 m_param1; // Effect param 1
        u16 m_param2; // Effect param 2
        u16 m_param3; // Effect param 3
        u16 m_sellPrice;
        u16 m_buyPrice;
    };

    /*
     * @brief: Compute itemtype character.
     */
    constexpr u16 getItemChar( const u8 p_itemType ) {
        if( p_itemType & ITEMTYPE_BERRY ) { return 473; }

        auto itemType = ( p_itemType & 15 );

        switch( itemType ) {
        case ITEMTYPE_POKEBALL:
            return 474;
        case ITEMTYPE_BATTLEITEM:
            return 475;
        case ITEMTYPE_COLLECTIBLE:
        case ITEMTYPE_USABLE:
        case ITEMTYPE_EVOLUTION:
            return 468;
        case ITEMTYPE_MEDICINE:
            return 472;
        case ITEMTYPE_FORMECHANGE:
        case ITEMTYPE_KEYITEM:
            return 469;
        case ITEMTYPE_TM:
            return 470;
        default:
            return ' ';
        }
    }

    /*
     * @brief Converts an item idx to an idx for a pokemon
     */
    constexpr u8 itemToBall( u16 p_itemIdx ) {
        switch( p_itemIdx ) {
            case I_MASTER_BALL:
                return 1;
            case I_ULTRA_BALL:
                return 2;
            case I_GREAT_BALL:
                return 3;
            default:
            case I_POKE_BALL:
                return 0;
            case I_SAFARI_BALL:
                return 4;
            case I_NET_BALL:
                return 5;
            case I_DIVE_BALL:
                return 6;
            case I_NEST_BALL:
                return 7;
            case I_REPEAT_BALL:
                return 8;
            case I_TIMER_BALL:
                return 9;
            case I_LUXURY_BALL:
                return 10;
            case I_PREMIER_BALL:
                return 11;
            case I_DUSK_BALL:
                return 12;
            case I_HEAL_BALL:
                return 13;
            case I_QUICK_BALL:
                return 14;
            case I_CHERISH_BALL:
                return 15;
            case I_FAST_BALL:
                return 16;
            case I_LEVEL_BALL:
                return 17;
            case I_LURE_BALL:
                return 18;
            case I_HEAVY_BALL:
                return 19;
            case I_LOVE_BALL:
                return 20;
            case I_FRIEND_BALL:
                return 21;
            case I_MOON_BALL:
                return 22;
            case I_SPORT_BALL:
                return 23;
            case I_PARK_BALL:
                return 24;
            case I_DREAM_BALL:
                return 25;
            case I_BEAST_BALL:
                return 26;
        }
    }

    /*
     * @brief Converts an pokemon pokeball idx to an item idx
     */
    constexpr u16 ballToItem( u8 p_ballIdx ) {
        switch( p_ballIdx ) {
            case 1:
                return I_MASTER_BALL;
            case 2:
                return I_ULTRA_BALL;
            case 3:
                return I_GREAT_BALL;
            default:
            case 0:
                return I_POKE_BALL;
            case 4:
                return I_SAFARI_BALL;
            case 5:
                return I_NET_BALL;
            case 6:
                return I_DIVE_BALL;
            case 7:
                return I_NEST_BALL;
            case 8:
                return I_REPEAT_BALL;
            case 9:
                return I_TIMER_BALL;
            case 10:
                return I_LUXURY_BALL;
            case 11:
                return I_PREMIER_BALL;
            case 12:
                return I_DUSK_BALL;
            case 13:
                return I_HEAL_BALL;
            case 14:
                return I_QUICK_BALL;
            case 15:
                return I_CHERISH_BALL;
            case 16:
                return I_FAST_BALL;
            case 17:
                return I_LEVEL_BALL;
            case 18:
                return I_LURE_BALL;
            case 19:
                return I_HEAVY_BALL;
            case 20:
                return I_LOVE_BALL;
            case 21:
                return I_FRIEND_BALL;
            case 22:
                return I_MOON_BALL;
            case 23:
                return I_SPORT_BALL;
            case 24:
                return I_PARK_BALL;
            case 25:
                return I_DREAM_BALL;
            case 26:
                return I_BEAST_BALL;
        }
    }

    bool        getItemName( const u16 p_itemId, const u8 p_language, char* p_out );
    std::string getItemName( const u16 p_itemId, const u8 p_language );
    std::string getItemName( const u16 p_itemId );

    bool        getItemDescr( const u16 p_itemId, const u8 p_language, char* p_out );
    std::string getItemDescr( const u16 p_itemId, const u8 p_language );
    std::string getItemDescr( const u16 p_itemId );

    itemData    getItemData( const u16 p_itemId );
    bool        getItemData( const u16 p_itemId, itemData* p_out );

    bool isUsable( const u16 p_itemId );

    /*
     * @brief: Uses a usable item/ key item.
     */
    bool use( const u16 p_itemId, const bool p_dryRun = false );

    /*
     * @brief: Use specified item on given pokemon. (Item needs to be either medicine,
     * formeChange).
     */
    bool use( const u16 p_itemId, const itemData& p_data, pokemon& p_pokemon,
              std::function<u8( u8 )> p_callback, bool p_inbattle = false );
} // namespace ITEM
