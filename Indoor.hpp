#pragma once
#include <Siv3D.hpp>
#include"Player.hpp"
#include"Object.hpp"
#include"TextManager.hpp"
#include"house.hpp"
class Indoor : public Object
{
public:
	Indoor(Point _pos, Point _region, Point _adjust, Point _wall, String _texture, SceneManager<String, Gamedata>::Scene* _Scene,Point _exit)
	{
		pos = _pos;
		texture = _texture;
		region = _region;
		adjust = _adjust;
		wall = _wall;
		Scene = _Scene;
		exit = _exit;
		init();
	}
	
	SceneManager<String, Gamedata>::Scene* Scene;
	Rect getRegion() const
	{
		return Rect(pos.x - region.x / 2, pos.y, region.x, region.y);
	}
	void init()
	{
		rect.push_back(Rect(pos.x + region.x / 2- adjust.x , pos.y + adjust.y, wall.x, region.y - adjust.y));
		rect.push_back(Rect(pos.x - region.x / 2, pos.y + adjust.y, wall.x, region.y - adjust.y));
		rect.push_back(Rect(pos.x - region.x / 2, pos.y + adjust.y, region.x - adjust.x, wall.y));
		rect.push_back(Rect(pos.x - region.x / 2, pos.y + region.y, region.x - adjust.x, wall.y));
		rect.push_back(Rect(-270, 480 ,80, 65));
		rect.push_back(Rect(-110, 560, 80, 95));
	}
	
	

	void update(std::shared_ptr<Player>& player)
	{
		for (uint32 i = 0; i < rect.size(); i++)
		{
			if (player->getBody().intersects(rect[i]))
			{
				player->pos -= player->speed;
			}
		}
		if (intersect_WithDoor(player) && player->movement.y > 0)
		{
			Scene->changeScene(L"FirstCity");
			Scene->m_data->pos = exit;
			Scene->m_data->movement = player->movement;
		}
	}

	void draw()
	{
		getRegion()(TextureAsset(texture)).draw();
		/*for (auto rect : rect)
		{
			rect.draw();
		}*/
		//Door().draw(Palette::Red);
	}
private:
	Point region, wall, adjust, exit;
	String texture;
	std::vector <Rect> rect;
	Rect Door() const
	{
		return Rect(-320, 745, 80, 5);
	}

	bool intersect_WithDoor(std::shared_ptr<Player>& player)
	{
		return player->getBody().intersects(Door());
	}
};

class mainIndoor : public Object
{
public:
	mainIndoor(Point _pos, Point _region, Point _adjust, Point _wall, String _texture, SceneManager<String, Gamedata>::Scene* _Scene, Point _exit)
	{
		pos = _pos;
		texture = _texture;
		region = _region;
		adjust = _adjust;
		wall = _wall;
		Scene = _Scene;
		exit = _exit;
		init();
	}
	
	String texture;
	std::vector <Rect> rect;
	SceneManager<String, Gamedata>::Scene* Scene;
	Rect getRegion() const
	{
		return Rect(pos.x - region.x / 2, pos.y, region.x, region.y);
	}
	void init()
	{
		rect.push_back(Rect(pos.x + region.x / 2 - adjust.x, pos.y + adjust.y, wall.x, region.y - adjust.y));
		rect.push_back(Rect(pos.x - region.x / 2, pos.y + adjust.y, wall.x, region.y - adjust.y));
		rect.push_back(Rect(pos.x - region.x / 2, pos.y + adjust.y, region.x - adjust.x, wall.y));
		rect.push_back(Rect(pos.x - region.x / 2, pos.y + region.y, region.x - adjust.x, wall.y));
		rect.push_back(Rect(25, 450, 110, 145));
		rect.push_back(Rect(80, 260, 80, 80));
	}

	void update(std::shared_ptr<Player>& player)
	{
		for (uint32 i = 0; i < rect.size(); i++)
		{
			if (player->getBody().intersects(rect[i]))
			{
				player->pos -= player->speed;
			}
		}
		if (intersect_WithDoor(player) && player->movement.y > 0)
		{
			Scene->m_data->Firstmessage = true;
			Scene->changeScene(L"FirstCity");
			Scene->m_data->pos = exit;
			Scene->m_data->movement = player->movement;
		}
	}

	void draw()
	{
		getRegion()(TextureAsset(texture)).draw();
		/*for (auto rect : rect)
		{
			rect.draw();
		}*/
		//Door().draw(Palette::Red);
	}
private:
	Rect Door() const
	{
		return Rect(-270, 635, 110, 5);
	}

	bool intersect_WithDoor(std::shared_ptr<Player>& player) const
	{
		return player->getBody().intersects(Door());
	}
	int32 phrase;
	Point region, wall, adjust, exit;
};