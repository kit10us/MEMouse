// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/input/InputDevice.h>
#include <me/game/IGame.h>
#include <map>

namespace memouse
{
	class Mouse : public me::input::InputDevice
	{
	public:
		Mouse( me::game::IGame * gameInstance );
		virtual ~Mouse();

		std::string GetName() const;

		void Update() override;

		size_t SubSourceCount() const override;

		size_t GetInputCount( size_t subSource ) const override;

		std::string GetInputName( size_t subSource, size_t index ) const override;

		size_t GetInputIndex( size_t subSource, std::string name ) const override;

		me::input::InputType GetInputType( size_t subSource, size_t index ) const override;

		me::input::IData::ptr GetInputData( size_t subSource, size_t index ) const override;

		me::input::IData::ptr GetInputData( size_t subSource, std::string name ) const override;

		void SetInputData( size_t subSource, size_t index, me::input::IData::ptr dataIn ) override;

	private:
		me::game::IGame * m_game;

		struct Source
		{
			Source()
				: LeftButton( false )
				, LeftButtonPressed( false )
				, RightButton( false )
				, RightButtonPressed( false )
				, MiddleButton( false )
				, MiddleButtonPressed( false )
				, PositionX( -1.0f )
				, PositionY( -1.0f )
				, ChangeX( 0.0f )
				, ChangeY( 0.0f )
				, MouseWheel( 0.0f )
			{
			}

			bool LeftButton;
			bool LeftButtonPressed;
			bool RightButton;
			bool RightButtonPressed;
			bool MiddleButton;
			bool MiddleButtonPressed;

			float PositionX;
			float ChangeX;
			float PositionY;
			float ChangeY;
			float MouseWheel;
		};

		std::map< std::string, size_t, unify::string::CaseInsensitiveLessThanTest > m_nameToIndex;
		std::vector< Source > m_subSources;
		std::vector< Source > m_subSourcesUpdated;
	};
}