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


#pragma once

#include "DMXStudio.h"
#include "IDefinitionVisitor.h"

class ChannelAngle
{
    friend class DefinitionWriter;
    friend class DefinitionReader;

    int			m_angle;			// Angle (degrees)
    BYTE		m_value;			// DMX channel value

public:
    ChannelAngle() {}
    ChannelAngle( int angle, BYTE channel_value );
    ~ChannelAngle(void);

    void accept( IDefinitionVisitor* visitor) {
        visitor->visit(this);
    }

    int getAngle() const {
        return m_angle;
    }

    BYTE getValue() const {
        return m_value;
    }
};

typedef std::vector<ChannelAngle> ChannelAngleArray;
typedef std::map<int,ChannelAngle> ChannelAngleMap;