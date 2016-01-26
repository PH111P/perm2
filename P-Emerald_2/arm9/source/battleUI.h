/*
Pok�mon Emerald 2 Version
------------------------------

file        : battleUI.h
author      : Philip Wellnitz
description : Consult corresponding source file.

Copyright (C) 2012 - 2016
Philip Wellnitz

This file is part of Pok�mon Emerald 2 Version.

Pok�mon Emerald 2 Version is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Pok�mon Emerald 2 Version is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Pok�mon Emerald 2 Version.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include "battle.h"

namespace BATTLE {

    class battleUI {
        battle* _battle;
        s8      _oldPKMNStats[ 6 ][ 2 ][ 10 ];

    public:

        static void initLogScreen( );
        static void clearLogScreen( );
        static void setLogTextColor( u16 p_color );
        static void setLogText2Color( u16 p_color );
        static void writeLogText( const std::wstring& p_message );

        void    init( );
        void    trainerIntro( );
        void    pokemonIntro( );

        bool    declareBattleMove( u8 p_pokemonPos, bool p_showBack );

        u16     chooseAttack( u8 p_pokemonPos );
        u8      chooseAttackTarget( u8 p_pokemonPos, u16 p_moveNo );
        u16     chooseItem( u8 p_pokemonPos );
        u8      choosePKMN( bool p_firstIsChosen, bool p_back = true );
        void    useNav( );

        void    showAttack( bool p_opponent, u8 p_pokemonPos );
        void    updateHP( bool p_opponent, u8 p_pokemonPos );
        void    showStatus( bool p_opponent, u8 p_pokemonPos );
        void    updateStatus( bool p_opponent, u8 p_pokemonPos );
        void    applyEXPChanges( bool p_opponent, u8 p_pokemonPos, u32 p_gainedExp );
        void    updateStats( bool p_opponent, u8 p_pokemonPos, bool p_move = true );

        void    hidePKMN( bool p_opponent, u8 p_pokemonPos );
        void    sendPKMN( bool p_opponent, u8 p_pokemonPos );
        void    evolvePKMN( bool p_opponent, u8 p_pokemonPos );

        void    learnMove( u8 p_pokemonPos, u16 p_move );

        void    showEndScreen( );

        battleUI( ) { }
        battleUI( battle* p_battle )
            :_battle( p_battle ) { }

        ~battleUI( ) {
            _battle = 0;
        }
    };
}