// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <memouse/Mouse.h>
#include <limits>

using namespace memouse;
using namespace me;
using namespace input;

std::vector< std::string > g_ButtonNames =
{
	"LeftButton",
	"RightButton",
	"MiddleButton"
};

std::vector< std::string > g_TrackerNames =
{
	"Tracker",
};

Mouse::Mouse( game::IGame * gameInstance )
	: m_game( gameInstance )
{
	m_subSources.resize( gameInstance->GetOS()->RendererCount() );
	m_subSourcesUpdated.resize( gameInstance->GetOS()->RendererCount() );

	size_t index = 0;
	for ( auto && name : g_ButtonNames )
	{
		m_nameToIndex[ name ] = index++;
	}
	for ( auto && name : g_TrackerNames )
	{
		m_nameToIndex[ name ] = index++;
	}
}

Mouse::~Mouse()
{
}

std::string Mouse::GetName() const
{
	return "Mouse";
}

void Mouse::Update()
{
	for ( size_t i = 0; i < m_subSources.size(); ++i )
	{
		Source & updated = m_subSourcesUpdated[i];
		Source & source = m_subSources[i];

		source.LeftButtonPressed = false;
		source.RightButtonPressed = false;
		source.MiddleButtonPressed = false;

		if ( updated.LeftButton == false && source.LeftButton == true )
		{
			source.LeftButtonPressed = true;
		}
		if ( updated.RightButton == false && source.RightButton == true )
		{
			source.RightButtonPressed = true;
		}
		if ( updated.MiddleButton == false && source.MiddleButton == true )
		{
			source.MiddleButtonPressed = true;
		}

		source.LeftButton = updated.LeftButton;
		source.RightButton = updated.RightButton;
		source.MiddleButton = updated.MiddleButton;

		source.ChangeX = 0;
		if ( source.PositionX != -1.0f )
		{
			source.ChangeX = source.PositionX - updated.PositionX;
		}
		source.PositionX = updated.PositionX;

		source.ChangeY = 0;
		if ( source.PositionY != -1.0f )
		{
			source.ChangeY = source.PositionY - updated.PositionY;
		}
		source.PositionY = updated.PositionY;

		source.MouseWheel = updated.MouseWheel;
		updated.MouseWheel = 0;
	}
}

size_t Mouse::SubSourceCount() const
{
	return m_subSources.size();
}

size_t Mouse::GetInputCount( size_t subSource ) const
{
	return g_ButtonNames.size() + g_TrackerNames.size();
}

std::string Mouse::GetInputName( size_t subSource, size_t index ) const
{
	if ( index < g_ButtonNames.size() )
	{	
		return g_ButtonNames[ index ];
	}
	else if ( index < (g_ButtonNames.size() + g_TrackerNames.size()) )
	{
		return g_TrackerNames[ index - g_ButtonNames.size() ];
	}
	else
	{
		return std::string();
	}
}

size_t Mouse::GetInputIndex( size_t subSource, std::string name ) const
{
	auto && itr = m_nameToIndex.find( name );
	return itr == m_nameToIndex.end() ? std::numeric_limits< size_t >::max() : itr->second;
}

input::InputType Mouse::GetInputType( size_t subSource, size_t index ) const
{
	if ( index < g_ButtonNames.size() )
	{	
		return input::InputType::Button;
	}
	else if ( index < (g_ButtonNames.size() + g_TrackerNames.size()) )
	{
		return input::InputType::Tracker;
	}
	else
	{
		return input::InputType::Invalid;
	}
}																	 

input::IData::ptr Mouse::GetInputData( size_t subSource, size_t index ) const
{
	if ( subSource >= m_subSources.size() )
	{
		return input::IData::ptr();
	}

	if ( m_game->IsUpdateLocked( false ) || ! m_game->GetOS()->GetHasFocus() ) return nullptr;

	const Source & source = m_subSources[subSource];

	if ( index < g_ButtonNames.size() )
	{	
		input::ButtonData * data = new input::ButtonData();

		switch ( index )
		{
		case 0:
			data->down = source.LeftButton;
			data->pressed = source.LeftButtonPressed;
			break;
		case 1:
			data->down = source.RightButton;
			data->pressed = source.RightButtonPressed;
			break;
		case 2:
			data->down = source.MiddleButton;
			data->pressed = source.MiddleButtonPressed;
			break;
		default:
			throw std::exception( "Error in GetInputData, invalid button index!" );
		}
		return input::IData::ptr( data );
	}
	else if ( index < (g_ButtonNames.size() + g_TrackerNames.size()) )
	{
		input::TrackerData * data = new input::TrackerData();
		data->set = { true, true, true };
		data->position.x = source.PositionX;
		data->position.y = source.PositionY;
		data->position.z = 0;
		data->change.x = source.ChangeX;
		data->change.y = source.ChangeY;
		data->change.z = source.MouseWheel;
		return input::IData::ptr( data );
	}
	else
	{
		return input::IData::ptr();
	}
}

me::input::IData::ptr Mouse::GetInputData( size_t subSource, std::string name ) const
{
	size_t index = GetInputIndex( subSource, name );
	return GetInputData( subSource, index );
}

void Mouse::SetInputData( size_t subSource, size_t index, me::input::IData::ptr dataIn )
{
	if ( subSource >= m_subSources.size() )
	{
		throw std::exception( "Error! Attempted to SetInputData for subSource out of range!" );
	}

	Source & source = m_subSourcesUpdated[subSource];

	if ( index < g_ButtonNames.size() )
	{	
		if ( dataIn->type != input::InputType::Button )
		{
			throw std::exception( "Error! Attempted to SetInputData for button, with NON button data!" );
		}																								 
		input::ButtonData * data = reinterpret_cast<me::input::ButtonData*>(dataIn.get());

		switch ( index )
		{
		case 0:
			source.LeftButton = data->down;
			source.LeftButtonPressed = data->pressed;
			break;
		case 1:
			source.RightButton = data->down;
			source.RightButtonPressed = data->pressed;
			break;
		case 2:
			source.MiddleButton = data->down;
			source.MiddleButtonPressed = data->pressed;
			break;
		default:
			throw std::exception( "Error in SetInputData, invalid button index!" );
		}
	}
	else if ( index < (g_ButtonNames.size() + g_TrackerNames.size()) )
	{
		input::TrackerData * data = reinterpret_cast<me::input::TrackerData*>(dataIn.get());

		if ( data->set.x )
		{
			source.PositionX = data->position.x;
			source.ChangeX = data->change.x;
		}
		if ( data->set.y )
		{
			source.PositionY = data->position.y;
			source.ChangeY = data->change.y;
		}
		if ( data->set.z )
		{
			source.MouseWheel = data->change.z;
		}
	}
	else
	{
		throw std::exception( "Error! Attempted to SetInputData for input index out of range!" );
	}
}