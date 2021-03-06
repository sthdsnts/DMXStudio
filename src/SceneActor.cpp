/* 
Copyright (C) 2011,2012 Robert DeSantis
 
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

#include "DMXStudio.h"
#include "SceneActor.h"
#include "Venue.h"

// ----------------------------------------------------------------------------
//
SceneActor::SceneActor( Fixture *pf ) :
    m_uid( pf->getUID() ),
    m_group( false), 
    m_channels( pf->getNumChannels() )
{
    reset_channel_values();

    // Copy channel defaults
    for ( channel_t channel=0; channel < m_channels; channel++ ) {
        m_channel_values[ channel ] = pf->getChannel( channel )->getDefaultValue();
    }
}

// ----------------------------------------------------------------------------
//
SceneActor::SceneActor( Venue* venue, FixtureGroup *fg ) :
    m_uid( fg->getUID() ),
    m_group( true ),
    m_channels(0)
{
    reset_channel_values();

    if ( fg->getNumChannelValues() > 0 ) {
        m_channels = fg->getNumChannelValues();
        fg->getChannelValues( m_channel_values );
    }
    else {
        Fixture* pf = venue->getGroupRepresentative( fg->getUID() );

        if ( pf != NULL ) {         // Make sure this is not an empty group
            m_channels = pf->getNumChannels();

            // Copy channel defaults
            for ( channel_t channel=0; channel < m_channels; channel++ ) {
                m_channel_values[ channel ] = pf->getChannel( channel )->getDefaultValue();
            }
        }
    }
}

// ----------------------------------------------------------------------------
//
SceneActor::~SceneActor(void)
{
}
