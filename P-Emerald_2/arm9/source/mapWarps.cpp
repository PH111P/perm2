/*
Pok�mon Emerald 2 Version
------------------------------

file        : mapWarps.cpp
author      : Philip Wellnitz
description : Map warps.

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

#include "mapWarps.h"
#include "mapDrawer.h"

namespace MAP {
    bool operator==( position p_l, position p_r ) {
        return p_l.m_posX == p_r.m_posX && p_l.m_posY == p_r.m_posY && p_l.m_posZ == p_r.m_posZ;
    }
    bool operator<( position p_l, position p_r ) {
        return p_l.m_posX < p_r.m_posX || ( p_l.m_posX == p_r.m_posX && p_l.m_posY < p_r.m_posY )
            || ( p_l.m_posX == p_r.m_posX && p_l.m_posY == p_r.m_posY && p_l.m_posZ < p_r.m_posZ );
    }

#define MAP_POS( p_mapX, p_mapY, p_x, p_y, p_z ) { ( p_mapY ) * 32 + ( p_x ), ( p_mapX ) * 32 + ( p_y ), ( p_z ) }
#define POS( p_x, p_y, p_z ) { ( p_x ), ( p_y ), ( p_z ) }
#define WARP( p_map1, p_pos1, p_map2, p_pos2 ) { { p_map1, p_pos1 }, { p_map2, p_pos2 } },\
                                { { p_map2, p_pos2 }, { p_map1, p_pos1 } }

    std::map<std::pair<u8, position>, std::pair<u8, position>> warpList = {
        //Meteor Falls
        WARP( 10, POS( 104, 119, 5 ), 1, MAP_POS( 1, 3, 0x1c, 0x1d, 4 ) ),
        WARP( 10, POS( 83, 133, 3 ), 1, MAP_POS( 1, 3, 0x07, 0x32, 3 ) ),

        WARP( 1, MAP_POS( 3, 1, 0x05, 0x06, 4 ), 1, MAP_POS( 1, 1, 0x04, 0x0e, 3 ) ),
        WARP( 1, MAP_POS( 3, 1, 0x07, 0x0b, 5 ), 1, MAP_POS( 1, 1, 0x08, 0x13, 3 ) ),
        WARP( 1, MAP_POS( 3, 1, 0x12, 0x0e, 4 ), 1, MAP_POS( 1, 1, 0x15, 0x17, 3 ) ),

        WARP( 1, MAP_POS( 3, 1, 0x03, 0x17, 5 ), 1, MAP_POS( 1, 3, 0x06, 0x0f, 3 ) ),
        WARP( 1, MAP_POS( 3, 1, 0x14, 0x1e, 3 ), 1, MAP_POS( 1, 3, 0x1b, 0x27, 3 ) ),

        WARP( 1, MAP_POS( 3, 1, 0x11, 0x03, 3 ), 1, MAP_POS( 2, 2, 0x05, 0x0f, 3 ) ),
        WARP( 1, MAP_POS( 1, 1, 0x0a, 0x1d, 3 ), 1, MAP_POS( 1, 3, 0x0b, 0x0e, 3 ) )
    };

#undef MAP_POS
#undef POS
#undef WARP
}