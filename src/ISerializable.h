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

#include "tinyxml.h"

class ISerializable
{
public:
	// Helper methods
	static void add_attribute( TiXmlElement& element, const char *name, DWORD value ) {
		char buffer[128];
		sprintf_s( buffer, "%lu", value );
		element.SetAttribute( name, buffer );
	}

	static void add_attribute( TiXmlElement& element, const char *name, WORD value ) {
		char buffer[128];
		sprintf_s( buffer, "%u", value );
		element.SetAttribute( name, buffer );
	}

	static void add_attribute( TiXmlElement& element, const char *name, int value ) {
		char buffer[128];
		sprintf_s( buffer, "%d", value );
		element.SetAttribute( name, buffer );
	}

	static void add_attribute( TiXmlElement& element, const char *name, unsigned value ) {
		char buffer[128];
		sprintf_s( buffer, "%u", value );
		element.SetAttribute( name, buffer );
	}

	static void add_attribute( TiXmlElement& element, const char *name, LONG value ) {
		char buffer[128];
		sprintf_s( buffer, "%ld", value );
		element.SetAttribute( name, buffer );
	}

	static void add_attribute( TiXmlElement& element, const char *name, bool value ) {
		element.SetAttribute( name, value ? "true" : "false" );
	}

	static void add_attribute( TiXmlElement& element, const char *name, double value ) {
		char buffer[128];
		sprintf_s( buffer, "%.2lf", value );
		element.SetAttribute( name, buffer );
	}

	static void add_attribute( TiXmlElement& element, const char *name, const char * value ) {
		element.SetAttribute( name, value );
	}

	static void add_text_element( TiXmlElement& element, const char *name, const char * value ) {
		if ( !value || strlen( value ) == 0 )
			return;

		TiXmlText text( value );
		TiXmlElement node( name );
		node.InsertEndChild( text );
		element.InsertEndChild( node );
	}

	template <class T>
	static void add_pfuids_element( TiXmlElement& element, T &uids  ) {
		TiXmlElement pfuids( "pfuids" );

		for ( T::iterator it=uids.begin(); it != uids.end(); it++ ) {
			TiXmlElement uidElement( "uid" );
			add_attribute( uidElement, "value", (*it) );
			pfuids.InsertEndChild( uidElement );
		}

		element.InsertEndChild( pfuids );
	}

	static const char *read_text_element( TiXmlElement *self, const char * element_name ) {
		TiXmlElement * element = self->FirstChildElement( element_name );
		if ( element == NULL || element->NoChildren() )
			return "";
		return element->FirstChild()->Value();
	}

	static const DWORD read_dword_attribute( TiXmlElement *self, const char * attribute_name, DWORD default_value=0UL ) {
		const char *value = self->Attribute( attribute_name );
		DWORD result = default_value;

		if ( value != NULL )
			sscanf_s( value, "%lu", &result );
		return result;
	}

	static const long read_long_attribute( TiXmlElement *self, const char * attribute_name, LONG default_value=0L ) {
		const char *value = self->Attribute( attribute_name );
		long result = default_value;

		if ( value != NULL )
			sscanf_s( value, "%ld", &result );
		return result;
	}

	static const WORD read_word_attribute( TiXmlElement *self, const char * attribute_name, WORD default_value=0 ) {
		const char *value = self->Attribute( attribute_name );
		unsigned result = default_value;

		if ( value != NULL )
			sscanf_s( value, "%u", &result );
		return (WORD)result;
	}

	static const double read_double_attribute( TiXmlElement *self, const char * attribute_name, double default_value=0.0 ) {
		const char *value = self->Attribute( attribute_name );
		double result = default_value;

		if ( value != NULL )
			sscanf_s( value, "%lf", &result );
		return result;
	}

	static const float read_float_attribute( TiXmlElement *self, const char * attribute_name, float default_value=0.0 ) {
		const char *value = self->Attribute( attribute_name );
		float result = default_value;

		if ( value != NULL )
			sscanf_s( value, "%f", &result );
		return result;
	}

	static const int read_int_attribute( TiXmlElement *self, const char * attribute_name, int default_value=0 ) {
		const char *value = self->Attribute( attribute_name );
		int result = default_value;

		if ( value != NULL )
			result = atoi( value );
		return result;
	}

	static const unsigned read_unsigned_attribute( TiXmlElement *self, const char * attribute_name, unsigned default_value=0 ) {
		const char *value = self->Attribute( attribute_name );
		unsigned result = default_value;

		if ( value != NULL )
			sscanf_s( value, "%u", &result );
		return result;
	}

	static const bool read_bool_attribute( TiXmlElement *self, const char * attribute_name, bool default_value=false ) {
		const char *value = self->Attribute( attribute_name );
        if ( !value )
            return default_value;
        return ( !_stricmp( value, "true" ) || !_stricmp( value, "yes" ) || !_stricmp( value, "1" ) );
	}

	static const char* read_text_attribute( TiXmlElement *self, const char * attribute_name ) {
		const char* value = self->Attribute( attribute_name );
		const char* result = "";

		if ( value != NULL )
			result = value;
		return result;
	}

#if 0
	template <class T>
	static std::vector<T *> read_list( TiXmlElement *container, const char *element_name ) {
		std::vector<T *> list;

		if ( container ) {
			TiXmlElement* element = container->FirstChildElement( element_name );
			while ( element ) {
				T * instance = T::readXML( element );
				if ( instance ) {
					list.push_back( instance );
				}
				element = element->NextSiblingElement();
			}
		}

		return list;
	}

	template <class T>
	static void write_map( TiXmlElement &container, T& list ) {
		for ( T::iterator it=list.begin();
			  it != list.end(); it++ )
			it->second.writeXML( container );
	}

	template <class T>
	static void write_ptr_array( TiXmlElement &container, T& list ) {
		for ( T::iterator it=list.begin();
			  it != list.end(); it++ )
			(*it)->writeXML( container );
	}

	template <class T>
	static void write_array( TiXmlElement &container, T& list ) {
		for ( T::iterator it=list.begin();
			  it != list.end(); it++ )
			(*it).writeXML( container );
	}
#endif

protected:
	virtual ~ISerializable() {}
};