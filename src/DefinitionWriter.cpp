/* 
Copyright (C) 2011,2012 Robert DeSantis
hopluvr at gmail dot com

This file is part of DMX Studio.
 
DMX Studio is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.
 
DMX Studio is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
License for more details.
 
You should have received a copy of the GNU General Public License
along with DMX Studio; see the file _COPYING.txt.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA.
*/


#include "DefinitionWriter.h"
#include "FixtureDefinition.h"

// ----------------------------------------------------------------------------
//
DefinitionWriter::DefinitionWriter(void)
{
}

// ----------------------------------------------------------------------------
//
DefinitionWriter::~DefinitionWriter(void)
{
}

// ----------------------------------------------------------------------------
//
void DefinitionWriter::writeFixtureDefinition( 
            LPCSTR file_name, 
            LPCSTR author, 
            LPCSTR version, 
            FixtureDefinitionPtrArray& definitions )
{
    TiXmlElement fixtures( "fixture_definitions" );
    add_text_element( fixtures, "author", author );
    add_text_element( fixtures, "version", version );

    visit_ptr_array<FixtureDefinitionPtrArray>( fixtures, definitions );

    TiXmlDocument doc;
    TiXmlDeclaration xml_decl( "1.0", "", "" );

    doc.InsertEndChild( xml_decl );
    doc.InsertEndChild( fixtures );
    //doc.Print();

    CString output_file = file_name;

    if ( doc.SaveFile( output_file ) )
        studio.log_status( "Wrote fixture definitions to '%s'\n", output_file );
    else
        studio.log_status( "Error writing to '%s'\n", output_file );
}

// ----------------------------------------------------------------------------
//
void DefinitionWriter::visit( FixtureDefinition* fixture_definition )
{
    TiXmlElement fixture( "fixture" );

    CString type( fixture_definition->convertFixtureTypeToText( fixture_definition->m_type ) );
    type.MakeLower();

    add_attribute( fixture, "type", (LPCSTR)type );
    add_attribute( fixture, "channels", (int)fixture_definition->m_channels.size() );

    add_text_element( fixture, "manufacturer", fixture_definition->m_manufacturer );
    add_text_element( fixture, "model", fixture_definition->m_model );

    TiXmlElement channels( "channels" );
    visit_array<ChannelArray>( channels, fixture_definition->m_channels );
    fixture.InsertEndChild( channels );

    getParent().InsertEndChild( fixture );
}

// ----------------------------------------------------------------------------
//
void DefinitionWriter::visit( Channel* channel )
{
    TiXmlElement channel_element( "channel" );

    add_attribute( channel_element, "index", (DWORD)channel->m_channel_offset );
    add_attribute( channel_element, "type", (LPCSTR)channel->convertChannelTypeToText( channel->m_type ) );
    add_attribute( channel_element, "color", channel->isColor() );
    add_attribute( channel_element, "blackout", channel->canBlackout() );

    if ( channel->m_default_value )
        add_attribute( channel_element, "value", channel->m_default_value );
    if ( channel->m_pixel_index )
        add_attribute( channel_element, "pixel", channel->m_pixel_index );
    if ( channel->m_home_value )
        add_attribute( channel_element, "home_value", channel->m_home_value );
    if ( channel->m_head_number )
        add_attribute( channel_element, "head", channel->m_head_number );

    // Add dimmer
    if ( channel->isDimmer() && 
         (channel->getType() != CHNLT_DIMMER ||
          channel->m_lowest_intensity != 0 || 
          channel->m_highest_intensity != 255) ) {
        TiXmlElement dimmer( "dimmer" );
        add_attribute( dimmer, "lowest_intensity", channel->m_lowest_intensity );
        add_attribute( dimmer, "highest_intensity", channel->m_highest_intensity );
        channel_element.InsertEndChild( dimmer );
    }

    add_text_element( channel_element, "name", channel->m_name );

    if ( channel->m_ranges.size() > 0 && channel->m_type != CHNLT_PAN && channel->m_type != CHNLT_TILT ) {
        TiXmlElement ranges( "ranges" );
        visit_array<ChannelValueRangeArray>( ranges, channel->m_ranges );
        channel_element.InsertEndChild( ranges );
    }

    if ( channel->m_angles.size() > 0 ) {
        TiXmlElement angles( "angles" );
        visit_map<ChannelAngleMap>( angles, channel->m_angles );
        channel_element.InsertEndChild( angles );
    }

    getParent().InsertEndChild( channel_element );
}

// ----------------------------------------------------------------------------
//
void DefinitionWriter::visit( ChannelAngle* channel_angle )
{
    TiXmlElement range( "angle" );

    add_attribute( range, "degrees", channel_angle->m_angle );
    add_attribute( range, "value",  channel_angle->m_value );

    getParent().InsertEndChild( range );
}

// ----------------------------------------------------------------------------
//
void DefinitionWriter::visit( ChannelValueRange* range )
{
    TiXmlElement range_element( "range" );

    add_attribute( range_element, "start", range->m_start );
    add_attribute( range_element, "end", range->m_end );
    add_text_element( range_element, "name", range->m_name );

    getParent().InsertEndChild( range_element );
}
