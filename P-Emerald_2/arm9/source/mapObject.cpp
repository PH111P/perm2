/*
Pokémon neo
------------------------------

file        : mapObject.cpp
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

#include "mapObject.h"
#include "mapSprite.h"

namespace MAP {
    void mapObject::interact( ) {
    }

    mapSprite mapObject::show( u16 p_currX, u16 p_currY, u8 p_oamIdx, u8 p_palIdx, u16 p_tileIdx ) {
        u8 frameStart = getFrame( m_direction );

        return mapSprite( p_currX, p_currY, m_picNum, frameStart, p_oamIdx, p_palIdx, p_tileIdx );
    }
} // namespace MAP
