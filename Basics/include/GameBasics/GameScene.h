#pragma once
#ifndef GAME_BASICS_GAMESCENE_H
#define GAME_BASICS_GAMESCENE_H

#include "AppBasics/Scene.h"
#include "Map.h"

begin_basics_namespace(game)

class BASICS_API GameScene : public Scene, public virtual ObjectWorld
{
public:
	GameScene()
		: Scene(),
		ObjectWorld([&](AppInfo& app)
			{
				if (!this->isRunning()) return;

				update_objects(app);
			})
	{
	}
};

end_basics_namespace(game)

#endif