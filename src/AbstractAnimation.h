/* 
Copyright (C) 2011-14 Robert DeSantis
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

#include "IVisitor.h"
#include "DMXStudio.h"
#include "DObject.h"
#include "RGBWA.h"
#include "AnimationSignal.h"

class AbstractAnimation : public DObject
{
protected:
    UIDArray			m_actors;
    AnimationSignal		m_signal;
    AnimationTask*		m_animation_task;

public:
    AbstractAnimation( ) :
        DObject(),
        m_animation_task(NULL)
    {}

    AbstractAnimation( UID animation_uid, AnimationSignal signal ) :
        DObject( animation_uid, 0, NULL, NULL ),
        m_signal( signal ),
        m_animation_task(NULL)
    {}

    virtual ~AbstractAnimation( void ) {}

    AnimationSignal& signal( ) { return m_signal; }	

    UIDArray getActors() const { 
        return UIDArray( m_actors );
    }
    void setActors( UIDArray actors ) {
        m_actors = actors;
    }

    virtual void accept( IVisitor* visitor) = 0;
    virtual const char* getName(void) = 0;
    virtual const char* getClassName(void) = 0;
    virtual CString getSynopsis(void);
    virtual AbstractAnimation* clone(void) = 0;

    // NOTE: The DMX packet contains _modified_ channel values (dimmer, backout, etc)
    virtual void initAnimation( AnimationTask* task, DWORD time_ms, BYTE* dmx_packet ) = 0;
    virtual bool sliceAnimation( DWORD time_ms, BYTE* dmx_packet ) = 0;
    virtual void stopAnimation( void ) = 0;
    virtual void removeActor( UID actor );

    UIDArray populateActors();
};

typedef std::vector<AbstractAnimation*> AnimationPtrArray;
