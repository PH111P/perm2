/*
Pok�mon Emerald 2 Version
------------------------------

file        : fs.cpp
author      : Philip Wellnitz
description :

Copyright (C) 2012 - 2015
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

#include <string>
#include <vector>
#include <cstdio>
#include <initializer_list>

#include "fs.h"
#include "buffer.h"
#include "ability.h"
#include "move.h"
#include "pokemon.h"
#include "item.h"
#include "berry.h"
#include "uio.h"
#include "defines.h"
#include "mapSlice.h"

const char ITEM_PATH[ ] = "nitro:/ITEMS/";
const char PKMNDATA_PATH[ ] = "nitro:/PKMNDATA/";
const char ABILITYDATA_PATH[ ] = "nitro:/PKMNDATA/ABILITIES/";

ability::ability( int p_abilityId ) {
    sprintf( buffer, "nitro:/PKMNDATA/ABILITIES/%i.data", p_abilityId );
    FILE* f = fopen( buffer, "r" );

    if( !f )
        return;

    m_abilityName = FS::readString( f, true );
    m_flavourText = FS::readString( f, true );
    fscanf( f, "%u", &( m_type ) );
    FS::close( f );
}

std::wstring getWAbilityName( int p_abilityId ) {
    sprintf( buffer, "nitro:/LOCATIONS/%i.data", p_abilityId );
    FILE* f = fopen( buffer, "r" );

    if( !f )
        return L"---";
    auto ret = FS::readWString( f, false );
    FS::close( f );
    return ret;
}

namespace FS {
    bool exists( const char* p_path, const char* p_name ) {
        char buffer[ 100 ];
        sprintf( buffer, "%s%s.raw", p_path, p_name );
        FILE* fd = fopen( buffer, "rb" );

        if( fd == 0 ) {
            fclose( fd );
            return false;
        }
        fclose( fd );
        return true;
    }
    bool exists( const char* p_path, u16 p_pkmnIdx, const char* p_name ) {
        char buffer[ 100 ];
        sprintf( buffer, "%s%d/%d%s.raw", p_path, p_pkmnIdx, p_pkmnIdx, p_name );
        FILE* fd = fopen( buffer, "rb" );

        if( fd == 0 ) {
            fclose( fd );
            return false;
        }
        fclose( fd );
        return true;
    }

    FILE* open( const char* p_path, const char* p_name, const char* p_ext, const char* p_mode ) {
        char buffer[ 100 ];
        sprintf( buffer, "%s%s%s", p_path, p_name, p_ext );
        return fopen( buffer, p_mode );
    }
    FILE* open( const char* p_path, u16 p_value, const char* p_ext, const char* p_mode ) {
        char buffer[ 100 ];
        sprintf( buffer, "%s%d%s", p_path, p_value, p_ext );
        return fopen( buffer, p_mode );
    }
    void close( FILE* p_file ) {
        fclose( p_file );
    }

    bool readData( const char* p_path, const char* p_name, const unsigned short p_dataCnt, unsigned short* p_data ) {
        FILE* fd = open( p_path, p_name );
        if( !fd )
            return false;
        fread( p_data, sizeof( unsigned short ), p_dataCnt, fd );
        fclose( fd );
        return true;
    }

    bool readData( const char* p_path, const char* p_name, const unsigned short p_dataCnt1,
                   unsigned short* p_data1, unsigned int p_dataCnt2, unsigned int* p_data2 ) {
        FILE* fd = open( p_path, p_name );
        if( !fd )
            return false;
        fread( p_data1, sizeof( unsigned short ), p_dataCnt1, fd );
        fread( p_data2, sizeof( unsigned int ), p_dataCnt2, fd );
        fclose( fd );
        return true;
    }
    bool readData( const char* p_path, const char* p_name, const unsigned int p_dataCnt1,
                   unsigned int* p_data1, unsigned short p_dataCnt2, unsigned short* p_data2 ) {
        FILE* fd = open( p_path, p_name );
        if( !fd )
            return false;
        fread( p_data1, sizeof( unsigned int ), p_dataCnt1, fd );
        fread( p_data2, sizeof( unsigned short ), p_dataCnt2, fd );
        fclose( fd );
        return true;
    }

    bool readSpriteData( IO::SpriteInfo* p_spriteInfo, const char* p_path, const char* p_name, const u32 p_tileCnt, const u16 p_palCnt, bool p_bottom ) {
        if( !readData( p_path, p_name, (unsigned int)p_tileCnt, TEMP, (unsigned short)p_palCnt, TEMP_PAL ) )
            return false;

        if( p_bottom ) {
            dmaCopyHalfWords( IO::SPRITE_DMA_CHANNEL, TEMP, &SPRITE_GFX_SUB[ p_spriteInfo->m_entry->gfxIndex * IO::OFFSET_MULTIPLIER ], 4 * p_tileCnt );
            dmaCopyHalfWords( IO::SPRITE_DMA_CHANNEL, TEMP_PAL, &SPRITE_PALETTE_SUB[ p_spriteInfo->m_entry->palette * IO::COLORS_PER_PALETTE ], 2 * p_palCnt );
        } else {
            dmaCopyHalfWords( IO::SPRITE_DMA_CHANNEL, TEMP, &SPRITE_GFX[ p_spriteInfo->m_entry->gfxIndex * IO::OFFSET_MULTIPLIER ], 4 * p_tileCnt );
            dmaCopyHalfWords( IO::SPRITE_DMA_CHANNEL, TEMP_PAL, &SPRITE_PALETTE[ p_spriteInfo->m_entry->palette * IO::COLORS_PER_PALETTE ], 2 * p_palCnt );
        }
        return true;
    }

    bool readPictureData( u16* p_layer, const char* p_path, const char* p_name, u16 p_palSize, u32 p_tileCnt, bool p_bottom ) {

        if( !readData( p_path, p_name, (unsigned int)( 12288 ), TEMP, (unsigned short)( 256 ), TEMP_PAL ) )
            return false;

        dmaCopy( TEMP, p_layer, p_tileCnt );
        if( p_bottom )
            dmaCopy( TEMP_PAL, BG_PALETTE_SUB, p_palSize );
        else
            dmaCopy( TEMP_PAL, BG_PALETTE, p_palSize );
        return true;
    }

    bool readNavScreenData( u16* p_layer, const char* p_name, u8 p_no ) {
        if( p_no == SAV->m_bgIdx && IO::NAV_DATA[ 0 ] ) {
            dmaCopy( IO::NAV_DATA, p_layer, 256 * 192 );
            dmaCopy( IO::NAV_DATA_PAL, BG_PALETTE_SUB, 256 * 2 );
            return true;
        }

        char buffer[ 100 ];
        sprintf( buffer, "%s", p_name );

        if( !readData( "nitro:/PICS/NAV/", buffer, (unsigned int)( 12288 ), IO::NAV_DATA, (unsigned short)( 256 ), IO::NAV_DATA_PAL ) )
            return false;

        dmaCopy( IO::NAV_DATA, p_layer, 256 * 192 );
        dmaCopy( IO::NAV_DATA_PAL, BG_PALETTE_SUB, 256 * 2 );

        return true;
    }

    bool readNop( FILE* p_file, u32 p_cnt ) {
        if( p_file == 0 )
            return false;
        fread( 0, 1, p_cnt, p_file );
        return true;
    }

    bool readPal( FILE* p_file, MAP::Palette* p_palette ) {
        if( p_file == 0 )
            return false;
        for( u8 i = 0; i < 6; ++i )
            fread( p_palette[ i ].m_pal, sizeof( u16 ) * 16, 1, p_file );
        return true;
    }

    bool readTileSet( FILE* p_file, MAP::TileSet& p_tileSet, u16 p_startIdx, u16 p_size ) {
        if( p_file == 0 )
            return false;
        fread( &p_tileSet.m_blocks[ p_startIdx ], sizeof( MAP::Tile )*p_size, 1, p_file );
        return true;
    }

    bool readBlockSet( FILE* p_file, MAP::BlockSet& p_tileSet, u16 p_startIdx, u16 p_size ) {
        if( p_file == 0 )
            return false;
        readNop( p_file, 4 );
        for( u16 i = 0; i < p_size; ++i ) {
            fread( p_tileSet.m_blocks[ p_startIdx + i ].m_bottom, 4 * sizeof( MAP::BlockAtom ), 1, p_file );
            fread( p_tileSet.m_blocks[ p_startIdx + i ].m_top, 4 * sizeof( MAP::BlockAtom ), 1, p_file );
        }
        for( u16 i = 0; i < p_size; ++i ) {
            fread( &p_tileSet.m_blocks[ p_startIdx + i ].m_bottombehave, sizeof( u8 ), 1, p_file );
            fread( &p_tileSet.m_blocks[ p_startIdx + i ].m_topbehave, sizeof( u8 ), 1, p_file );
        }
        return true;
    }

    //inline void readAnimations( FILE* p_file, std::vector<MAP::Animation>& p_animations ) {
    //    if( p_file == 0 )
    //        return;
    //    u8 N;
    //    fread( &N, sizeof( u8 ), 1, p_file );
    //    for( int i = 0; i < N; ++i ) {
    //        MAP::Animation a;
    //        fread( &a.m_tileIdx, sizeof( u16 ), 1, p_file );
    //        fread( &a.m_speed, sizeof( u8 ), 1, p_file );
    //        fread( &a.m_maxFrame, sizeof( u8 ), 1, p_file );
    //        a.m_acFrame = 0;
    //        a.m_animationTiles.assign( a.m_maxFrame, Tile( ) );
    //        for( int i = 0; i < a.m_maxFrame; ++i )
    //            fread( &a.m_animationTiles[ i ], sizeof( Tile ), 1, p_file );
    //        p_animations.push_back( a );
    //    }
    //    fclose( p_file );
    //}

    std::string readString( FILE* p_file, bool p_new ) {
        std::string ret = "";
        int ac;

        while( ( ac = fgetc( p_file ) ) == '\n' || ac == '\r' );

        if( ac == '*' || ac == EOF ) {
            return ret;
        } else ret += ac;

        while( ( ac = fgetc( p_file ) ) != '*' && ac != EOF ) {
            if( ac == '|' )
                ret += (char)136;
            else if( ac == '#' )
                ret += (char)137;
            else
                ret += ac;
        }
        if( !p_new )
            return convertToOld( ret );
        else
            return ret;
    }

    std::wstring readWString( FILE* p_file, bool p_new ) {
        std::wstring ret = L"";
        int ac;
        while( ( ac = fgetc( p_file ) ) == '\n' || ac == '\r' );
        if( ac == '*' || ac == EOF ) {
            return ret;
        } else ret += ac;
        while( ( ac = fgetc( p_file ) ) != '*' && ac != EOF ) {
            if( ac == '|' )
                ret += (wchar_t)136;
            else if( ac == '#' )
                ret += (wchar_t)137;
            else
                ret += ac;
        }
        if( !p_new )
            return convertToOld( ret );
        else
            return ret;
    }

    std::string breakString( const std::string& p_string, u8 p_lineLength ) {
        std::string result = "";

        u8 acLineLength = 0;
        std::string tmp = "";
        for( auto c : p_string ) {
            if( c == ' ' ) {
                if( acLineLength + tmp.length( ) > p_lineLength ) {
                    if( acLineLength ) {
                        result += "\n" + tmp + " ";
                        acLineLength = tmp.length( );
                        tmp = "";
                    } else {
                        result += tmp + "\n";
                        acLineLength = 0;
                        tmp = "";
                    }
                } else {
                    result += tmp + ' ';
                    tmp = "";
                    acLineLength += tmp.length( ) + 1;
                }
            } else
                tmp += c;
        }

        if( acLineLength + tmp.length( ) > p_lineLength  && acLineLength )
            result += "\n" + tmp + " ";
        else
            result += tmp;
        return result;
    }

    std::string breakString( const std::string& p_string, IO::font* p_font, u8 p_lineLength ) {
        std::string result = "";

        u8 acLineLength = 0;
        std::string tmp = "";
        for( auto c : p_string ) {
            if( c == ' ' ) {
                u8 tmpLen = p_font->stringWidth( tmp.c_str( ) );
                if( acLineLength + tmpLen > p_lineLength ) {
                    if( acLineLength ) {
                        result += "\n" + tmp + " ";
                        acLineLength = tmpLen;
                        tmp = "";
                    } else {
                        result += tmp + "\n";
                        acLineLength = 0;
                        tmp = "";
                    }
                } else {
                    result += tmp + ' ';
                    tmp = "";
                    acLineLength += tmpLen;
                }
            } else
                tmp += c;
        }

        if( acLineLength + p_font->stringWidth( tmp.c_str( ) ) > p_lineLength  && acLineLength )
            result += "\n" + tmp + " ";
        else
            result += tmp;
        return result;
    }

    std::string convertToOld( const std::string& p_string ) {
        std::string ret = "";
        for( auto ac = p_string.begin( ); ac != p_string.end( ); ++ac ) {
            if( *ac == '�' )
                ret += '\x84';
            else if( *ac == '�' )
                ret += '\x8E';
            else if( *ac == '�' )
                ret += '\x81';
            else if( *ac == '�' )
                ret += '\x9A';
            else if( *ac == '�' )
                ret += '\x94';
            else if( *ac == '�' )
                ret += '\x99';
            else if( *ac == '�' )
                ret += '\x9D';
            else if( *ac == '�' )
                ret += '\x82';
            else if( *ac == '%' )
                ret += ' ';
            else if( *ac == '|' )
                ret += (char)136;
            else if( *ac == '#' )
                ret += (char)137;
            else if( *ac == '\r' )
                ret += "";
            else
                ret += *ac;
        }
        return ret;
    }
    std::wstring convertToOld( const std::wstring& p_string ) {
        std::wstring ret = L"";
        for( auto ac = p_string.begin( ); ac != p_string.end( ); ++ac ) {
            if( *ac == '�' )
                ret += L'\x84';
            else if( *ac == '�' )
                ret += L'\x8E';
            else if( *ac == '�' )
                ret += L'\x81';
            else if( *ac == '�' )
                ret += L'\x9A';
            else if( *ac == '�' )
                ret += L'\x94';
            else if( *ac == '�' )
                ret += L'\x99';
            else if( *ac == '�' )
                ret += L'\x9D';
            else if( *ac == '�' )
                ret += L'\x82';
            else if( *ac == '%' )
                ret += L' ';
            else if( *ac == '|' )
                ret += (char)136;
            else if( *ac == '#' )
                ret += (char)137;
            else if( *ac == '\r' )
                ret += L"";
            else
                ret += *ac;
        }
        return ret;
    }

    const char* getLoc( u16 p_ind ) {
        if( p_ind > 5000 )
            return FARAWAY_PLACE;
        FILE* f = FS::open( "nitro:/LOCATIONS/", p_ind, ".data" );

        if( f == 0 ) {
            if( savMod == SavMod::_NDS && p_ind > 322 && p_ind < 1000 )
                return getLoc( 3002 );

            return FARAWAY_PLACE;
        }
        char buffer[ 50 ] = { 0 };
        fread( buffer, 1, 49, f );

        std::string ret = std::string( buffer );
        ret.pop_back( );
        FS::close( f );
        return ret.c_str( );
    }
}

std::string toString( u16 p_num ) {
    char buffer[ 32 ];
    sprintf( buffer, "%hu", p_num );
    return std::string( buffer );
}

[[ deprecated ]]
Type getType( u16 p_pkmnId, u16 p_type ) {
    pokemonData tmp;
    if( getAll( p_pkmnId, tmp ) ) {
        return tmp.m_types[ p_type ];
    } else {
        return UNBEKANNT;
    }
}
[[ deprecated ]]
u16 getBase( u16 p_pkmnId, u16 p_base ) {
    pokemonData tmp;
    if( getAll( p_pkmnId, tmp ) ) {
        return tmp.m_bases[ p_base ];
    } else {
        return 0;
    }
}
[[ deprecated ]]
u16 getCatchRate( u16 p_pkmnId ) {
    pokemonData tmp;
    if( getAll( p_pkmnId, tmp ) ) {
        return tmp.m_catchrate;
    } else {
        return 0;
    }
}
[[ deprecated ]]
const char* getDisplayName( u16 p_pkmnId ) {
    static pokemonData tmp;
    if( getAll( p_pkmnId, tmp ) ) {
        return reinterpret_cast<const char*>( tmp.m_displayName );
    } else {
        return "???";
    }
}
const wchar_t* getWDisplayName( u16 p_pkmnId ) {
    static pokemonData tmp;
    if( !getAll( p_pkmnId, tmp ) ) {
        return L"???";
    }

    std::wstring res = L"";
    for( u8 i = 0; tmp.m_displayName[ i ]; ++i )
        res += tmp.m_displayName[ i ];
    return res.c_str( );
}
void getWDisplayName( u16 p_pkmnId, wchar_t* p_name ) {
    pokemonData tmp;
    if( !getAll( p_pkmnId, tmp ) ) {
        wcscpy( p_name, L"???" );
        return;
    }

    std::wstring res = L"";
    for( u8 i = 0; tmp.m_displayName[ i ]; ++i )
        res += tmp.m_displayName[ i ];
    wcscpy( p_name, res.c_str( ) );
}

[[ deprecated ]]
void getHoldItems( u16 p_pkmnId, u16* p_items ) {
    pokemonData tmp;
    memset( p_items, 0, 4 * sizeof( u16 ) );
    if( getAll( p_pkmnId, tmp ) ) {
        memcpy( p_items, tmp.m_items, 4 * sizeof( u16 ) );
    }
}

[[ deprecated ]]
pkmnGenderType getGenderType( u16 p_pkmnId ) {
    pokemonData tmp;
    if( getAll( p_pkmnId, tmp ) ) {
        return tmp.m_gender;
    } else {
        return pkmnGenderType( 0 );
    }
}

[[ deprecated ]]
const char* getDexEntry( u16 p_pkmnId ) {
    static pokemonData tmp;
    if( getAll( p_pkmnId, tmp ) ) {
        return reinterpret_cast<const char*>( tmp.m_dexEntry );
    } else {
        return NO_DATA;
    }
}

[[ deprecated ]]
u16 getForme( u16 p_pkmnId, u16 p_formeId, std::string& p_retFormeName ) {
    pokemonData tmp;
    if( !getAll( p_pkmnId, tmp ) ) {
        return p_pkmnId;
    }
    p_retFormeName = std::string( reinterpret_cast<const char*>( tmp.m_formeName[ p_formeId ] ) );
    return tmp.m_formeIdx[ p_formeId ];
}

[[ deprecated ]]
std::vector<u16> getAllFormes( u16 p_pkmnId ) {
    pokemonData tmp;
    if( !getAll( p_pkmnId, tmp ) ) {
        return{ };
    }

    std::vector<u16> res;
    for( u8 i = 0; i < tmp.m_formecnt; ++i )
        res.push_back( tmp.m_formeIdx[ i ] );
    return res;
}

[[ deprecated ]]
const char* getSpecies( u16 p_pkmnId ) {
    static pokemonData tmp;
    if( getAll( p_pkmnId, tmp ) ) {
        return reinterpret_cast<const char*>( tmp.m_species );
    } else {
        return UNKNOWN_SPECIES;
    }
}

bool getAll( u16 p_pkmnId, pokemonData& p_out ) {
    FILE* f = FS::open( PKMNDATA_PATH,  p_pkmnId , ".data" );
    if( f == 0 ) 
        return false;

    fread( &p_out, sizeof( pokemonData ), 1, f );
    FS::close( f );
    return true;
}

void getLearnMoves( u16 p_pkmnId, u16 p_fromLevel, u16 p_toLevel, u16 p_mode, u16 p_amount, u16* p_result ) {
    FILE* f = FS::open( ( std::string( PKMNDATA_PATH ) + "/LEARNSETS/" ).c_str( ), p_pkmnId, ".learnset.data" );
    if( !f )
        return;

    u16 buffer[ 700 ];
    fread( buffer, sizeof( u16 ), 699, f );
    FS::close( f );
    u16 ptr = 0;

    u16 rescnt = 0;
    for( u8 i = 0; i < p_amount; ++i )
        p_result[ i ] = 0;

    if( p_fromLevel > p_toLevel ) {
        std::vector<u16> reses;
        for( u16 i = 0; i <= p_fromLevel; ++i ) {
            u16 z = buffer[ ptr++ ];
            for( int j = 0; j < z; ++j ) {
                u16 g = buffer[ ptr++ ], h = buffer[ ptr++ ];
                if( i >= p_toLevel && h == (u16)p_mode && g < MAXATTACK )
                    reses.push_back( g );
            }
        }
        auto I = reses.rbegin( );
        for( u16 i = 0; i < p_amount && I != reses.rend( ); ++i, ++I ) {
            for( u16 z = 0; z < i; ++z )
                if( *I == p_result[ z ] ) {
                    --i;
                    goto N;
                }
            p_result[ i ] = *I;
N:
            ;
        }
        FS::close( f );
        return;
    } else {
        for( u16 i = 0; i <= p_toLevel; ++i ) {
            u16 z = buffer[ ptr++ ];
            for( u16 j = 0; j < z; ++j ) {
                u16 g = buffer[ ptr++ ], h = buffer[ ptr++ ];
                if( i >= p_fromLevel && h == p_mode && g < MAXATTACK ) {
                    for( u16 z = 0; z < rescnt; ++z )
                        if( g == p_result[ z ] )
                            goto NEXT;
                    p_result[ rescnt ] = g;
                    if( ++rescnt == p_amount )
                        return;
NEXT:
                    ;
                }
            }
        }
    }
    FS::close( f );
}
bool canLearn( u16 p_pkmnId, u16 p_moveId, u16 p_mode ) {
    FILE* f = FS::open( (std::string(PKMNDATA_PATH) + "/LEARNSETS/").c_str(), p_pkmnId, ".learnset.data" );
    if( !f )
        return false;

    u16 buffer[ 700 ];
    fread( buffer, sizeof( u16 ), 699, f );
    FS::close( f );
    u16 ptr = 0;

    for( int i = 0; i <= 100; ++i ) {
        int z = buffer[ ptr++ ];
        for( int j = 0; j < z; ++j ) {
            u16 g = buffer[ ptr++ ], h = buffer[ ptr++ ];
            if( g == p_moveId && h == p_mode )
                return true;
        }
    }
    return false;
}

u16 item::getItemId( ) {
    for( int i = 0; i < 700; ++i )
        if( ItemList[ i ]->m_itemName == m_itemName )
            return i;
    return 0;
}

bool item::load( ) {
    if( m_loaded )
        return true;
    FILE* f = FS::open( ITEM_PATH, m_itemName.c_str( ), ".data" );
    if( !f )
        return false;

    memset( &m_itemData, 0, sizeof( itemData ) );
    fscanf( f, "%hhu %lu %lu\n", &m_itemData.m_itemEffectType,
            &m_itemData.m_price, &m_itemData.m_itemEffect );
    strcpy( m_itemData.m_itemDisplayName, FS::readString( f, true ).c_str( ) );
    strcpy( m_itemData.m_itemDescription, FS::readString( f, true ).c_str( ) );
    strcpy( m_itemData.m_itemShortDescr, FS::readString( f, true ).c_str( ) );
    FS::close( f );
    return m_loaded = true;
}

bool berry::load( ) {
    if( m_loaded )
        return true;
    FILE* f = FS::open( ITEM_PATH, m_itemName.c_str( ), ".data" );
    if( !f )
        return false;

    memset( &m_itemData, 0, sizeof( itemData ) );
    fscanf( f, "%hhu %lu %lu\n", &m_itemData.m_itemEffectType,
            &m_itemData.m_price, &m_itemData.m_itemEffect );
    strcpy( m_itemData.m_itemDisplayName, FS::readString( f, true ).c_str( ) );
    strcpy( m_itemData.m_itemDescription, FS::readString( f, true ).c_str( ) );
    strcpy( m_itemData.m_itemShortDescr, FS::readString( f, true ).c_str( ) );

    memset( &m_berryData, 0, sizeof( berryData ) );
    fscanf( f, "%hu %hhu %hhu %hhu", &m_berryData.m_berrySize,
            &m_berryData.m_berryGuete, &m_berryData.m_naturalGiftType,
            &m_berryData.m_naturalGiftStrength );
    for( u8 i = 0; i < 5; ++i )
        fscanf( f, "%hhu", &m_berryData.m_berryTaste[ i ] );
    fscanf( f, "%hhu %hhu %hhu", &m_berryData.m_hoursPerGrowthStage,
            &m_berryData.m_minBerries, &m_berryData.m_maxBerries );
    FS::close( f );
    return m_loaded = true;
}

std::string item::getDisplayName( bool p_new ) {
    if( !m_loaded && !load( ) )
        return m_itemName;
    if( p_new )
        return std::string( m_itemData.m_itemDisplayName );
    else
        return FS::convertToOld( std::string( m_itemData.m_itemDisplayName ) );
}

std::string item::getDescription( ) {
    if( !m_loaded && !load( ) )
        return NO_DATA;
    return std::string( m_itemData.m_itemDescription );
}

std::string item::getShortDescription( ) {
    if( !m_loaded && !load( ) )
        return NO_DATA;
    return std::string( m_itemData.m_itemShortDescr );
}


u32 item::getEffect( ) {
    if( !m_loaded && !load( ) )
        return 0;
    return m_itemData.m_itemEffect;
}

item::itemEffectType item::getEffectType( ) {
    if( !m_loaded && !load( ) )
        return itemEffectType::NONE;
    return m_itemData.m_itemEffectType;
}

u32 item::getPrice( ) {
    if( !m_loaded && !load( ) )
        return 0;
    return m_itemData.m_price;
}
