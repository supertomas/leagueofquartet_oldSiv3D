#pragma once
# include <Siv3D.hpp>
#include<HamFramework.hpp>
#include"backGround.hpp"
#include"TextManager.hpp"
#include"Battle.hpp"
#include"Gamedata.hpp"
#include"Special_Com.hpp"
#include"encount_rader.hpp"
#include"Mobu.hpp"
struct changerect
{
	Rect rect;
	String Scene;
	Point nextpos;
};


class Field : public SceneManager<String, Gamedata>::Scene
{
public:
	std::shared_ptr<Player> player;
	Array<std::shared_ptr<Object>> objects;
	Array<std::shared_ptr<Object>> grounds;
	Array<std::shared_ptr<flower>> flowers;
	Sound townM;
	Vec2 changeBattle;
	std::vector<changerect> change_scene_rects;
	std::vector<String> battle_scenes;
	AllBattle battle;
	encount_radar rader;
	CellManager cellManager;

	void update() override
	{
		rader.update(cellManager,changeBattle,player->distance);
		townM.play();
		for (auto& object : objects)
		{
			object->update(player);
		}
		std::sort(objects.begin(), objects.end(), [](std::shared_ptr<Object> a, std::shared_ptr<Object> b)
			{
				return a->pos.y < b->pos.y;
			});

		//次のフィールドに移り変わる処理
		for (auto& changerect : change_scene_rects)
		{
			if (player->getBody().intersects(changerect.rect))
			{
				changeScene(changerect.Scene);
				m_data->pos = changerect.nextpos;
				m_data->movement = player->movement;
			}
		}
		//フィールドのモンスターと戦うシーンに移り変わる処理
		for (auto& battle_secne : battle_scenes)
		{
			if (rader.Battle_Scene)
			{
				changeScene(battle_secne);
				townM.pause();
				m_data->pos = player->pos;
				rader.Battle_Scene = false;
				player->distance = { 0,0 };
				m_data->movement = player->movement;
			}

		}
		String flag = TextManager::text->getFlag();
		//特定のセリフの話しが終わったらバトルに移り変わる処理
		if (flag != L"")
		{
			townM.pause();
			flag;
			changeScene(flag);
			m_data->pos = player->pos;
			m_data->movement = player->movement;
		}
		TextManager::text->update();

	}

	void draw() const override
	{
		Graphics::SetBackground(Color(15, 15, 15));
		if (Input::KeyX.clicked)
		{
			skill_com.draw();
		}
		Graphics2D::PushTransform(Mat3x2::Translate(-player->pos.x + Window::Size().x / 2, -player->pos.y + Window::Size().y / 2));
		for (auto& ground : grounds)
		{
			ground->draw();
		}
		for (auto& flower : flowers)
		{
			flower->draw();
		}
		for (auto& object : objects)
		{
			object->draw();
		}

		Graphics2D::PopTransform();
		cellManager.draw();
		rader.draw();
		TextManager::text->draw();
	}
private:

	std::shared_ptr<Mobu> mobu;
	Skill_Com skill_com;
	Font font = Font(30);
};


class firstCity : public Field
{
public:

	void init()
	{
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);

		grounds.push_back(std::make_shared<Ground>(Point{ -1000,-700 }, Point{ 3000, 2700 }, L"grass", 5, 7));
		grounds.push_back(std::make_shared<Ground>(Point({ -380,80 }), Point({ 400, 300 }), L"soil2", 2, 1));
		flowers.push_back(std::make_shared<flower>(Point({ 80,-600 }), Point({ 60, 60 }), L"flower", 3, 3));
		flowers.push_back(std::make_shared<flower>(Point({ 150,0 }), Point({ 60, 60 }), L"flower", 5, 3));
		flowers.push_back(std::make_shared<flower>(Point({ -450,200 }), Point({ 60, 60 }), L"flower", 3, 2));

		player = std::make_shared<Player>(Point(m_data->pos), Point({ 55, 40 }), Point({ 91, 154 }), std::array<String, 4>{ L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, Point(m_data->movement));
		objects.push_back(player);
		changeBattle = { INFINITY,INFINITY };


		objects.push_back(std::make_shared<tree>(Point{ -750, 1000 }, Point{ 370, 370 }, Point{ -100, -180 }, String(L"tree1"), 9, 3));
		objects.push_back(std::make_shared<tree>(Point{ -500, -1300 }, Point{ 370, 370 }, Point{ -100, -190 }, L"tree1", 3, 3));
		objects.push_back(std::make_shared<tree>(Point({ -1700,-1200 }), Point({ 370, 370 }), Point({ -50, -100 }), String(L"tree1"), 6, 12));
		objects.push_back(std::make_shared<tree>(Point({ 900,-1200 }), Point({ 370, 370 }), Point({ -50, -100 }), String(L"tree1"), 4, 15));


		objects.push_back(std::make_shared<House01>(Point{ 400, -400 }, Point{ 500, 500 }, Point{ 8, 50 }, Point{ 150,70 }, Point{ 300, 80 }, 108, String(L"house"), this));
		objects.push_back(std::make_shared<House02>(Point{ -300, 0 }, Point{ 500, 500 }, Point{ 8, 50 }, Point{ 150,70 }, Point{ 300, 80 }, 108, String(L"house"), this));
		objects.push_back(std::make_shared<House03>(Point{ 350, 450 }, Point{ 500, 500 }, Point{ 8, 50 }, Point{ 130, 80 }, Point{ 360, 110 }, 175, String(L"house2"), this));

		objects.push_back(std::make_shared<Mobu>(Point{ -100, 500 }, Point{ 40, 30 }, MobuDirection::Backward, Point{ 94 ,158 }, std::array < String, 4>{ L"Mobu2R", L"Mobu2L", L"Mobu2B", L"Mobu2F" }, 0));
		objects.push_back(std::make_shared<Mobu>(Point{ 500, -100 }, Point{ 40, 30 }, MobuDirection::Right, Point{ 100 ,173 }, std::array < String, 4>{ L"PieroR", L"PieroL", L"PieroB", L"PieroF" }, 1));
		objects.push_back(std::make_shared<Mobu>(Point{ 100, -900 }, Point{ 40, 30 }, MobuDirection::Forward, Point{ 94,158 }, std::array < String, 4>{ L"Lady1R", L"Lady1L", L"Lady1B", L"Lady1F" }, 5));
		objects.push_back(std::make_shared<Mobu>(Point{ -150, -500 }, Point({ 40, 30 }), MobuDirection::Forward, 2, Point{ 82, 155 }, std::array< String, 4>{ L"DoctorR", L"DoctorL", L"DoctorB", L"DoctorF" }, 3));
		change_scene_rects.push_back(changerect{ Rect(-100, -1300, 1000, 20), L"FirstField",{ 130,750 } });

	}
private:

};

class firstField :public Field
{
public:
	void init()
	{
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		player = std::make_shared<Player>(Point(m_data->pos), Point({ 55, 40 }), Point({ 91, 154 }), std::array<String, 4>{ L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, Point(m_data->movement));
		grounds.push_back(std::make_shared<Ground>(Point{ -1000,-700 }, Point{ 3000, 2700 }, String(L"grass"), 5, 7));
		grounds.push_back(std::make_shared<Ground>(Point{ -850,-2100 }, Point{ 3000, 2700 }, String(L"soil"), 3, 1));
		flowers.push_back(std::make_shared<flower>(Point{ 460, -320 }, Point{ 60, 60 }, String(L"flower"), 3, 3));
		flowers.push_back(std::make_shared<flower>(Point({ 80,-600 }), Point({ 60, 60 }), L"flower", 3, 3));
		flowers.push_back(std::make_shared<flower>(Point({ 150,0 }), Point({ 60,60 }), L"flower", 5, 3));
		flowers.push_back(std::make_shared<flower>(Point({ -450,200 }), Point({ 60, 60 }), L"flower", 3, 3));
		flowers.push_back(std::make_shared<flower>(Point({ 380,-750 }), Point({ 60, 60 }), L"flower", 5, 5));

		objects.push_back(std::make_shared<tree>(Point{ 800,-1200 }, Point{ 370, 370 }, Point{ -50, -100 }, L"tree1", 6, 15));
		objects.push_back(std::make_shared<tree>(Point{ -1200,-1200 }, Point{ 370, 370 }, Point{ -50, -100 }, L"tree1", 5, 17));
		objects.push_back(std::make_shared<tree>(Point{ -1400,-2300 }, Point{ 370, 370 }, Point{ -50, -180 }, L"tree1", 1, 20));
		objects.push_back(std::make_shared<tree>(Point{ -900,-2500 }, Point{ 370, 370 }, Point{ -50, -180 }, L"tree1", 20, 5));
		objects.push_back(std::make_shared<tree>(Point{ 50,200 }, Point{ 370, 370 }, Point{ -80, -230 }, L"tree1", 4, 2));
		objects.push_back(std::make_shared<tree>(Point{ -230,-850 }, Point{ 370, 370 }, Point{ -80, -230 }, L"tree1", 3, 2));
		objects.push_back(std::make_shared<rock>(Point{ 1790,-1350 }, Point{ 780, 780 }, Point{ 470, 350 }, L"stone"));
		changeBattle = { Random(80,400),Random(80,400) };
		objects.push_back(std::make_shared<Mobu>(Point({ 505, -370 }), Point({ 40, 30 }), MobuDirection::Left, 2, Point({ 94, 158 }), std::array<String, 4>{ L"Mobu2R", L"Mobu2L", L"Mobu2B", L"Mobu2F" }, 9));
		objects.push_back(std::make_shared<Mobu>(Point{ 1140, -1610 }, Point{ 40, 30 }, MobuDirection::Backward, Point{ 72 ,158 }, std::array<String, 4>{ L"Mobu1R", L"Mobu1L", L"Mobu1B", L"Mobu1F" }, 6));
		objects.push_back(player);
		if (!m_data->Arisu_finish)
		{
			objects.push_back(std::make_shared<Mobu>(Point{ -1155, -1900 }, Point{ 40, 30 }, MobuDirection::Right, Point{ 98 ,167 }, std::array<String, 4>{ L"MajyoR", L"MajyoL", L"MajyoB", L"MajyoF" }, 7));
		}
		else if(m_data->Arisu_finish)
		{
			objects.push_back(std::make_shared<Mobu>(Point{ -1155, -1900 }, Point{ 40, 30 }, MobuDirection::Right, Point{ 98 ,167 }, std::array<String, 4>{ L"MajyoR", L"MajyoL", L"MajyoB", L"MajyoF" }, 10));
		}
		change_scene_rects.push_back(changerect{ Rect(-300, 800, 1000, 20), L"FirstCity",{ 385,-1130 } });
		change_scene_rects.push_back(changerect{ Rect(-1300, -2335, 400, 10), L"SecondField",{ 0,720 } });
		battle_scenes.push_back(String(L"Battle"));
	}
};


class secondField :public Field
{
public:

	void init()
	{
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		player = std::make_shared<Player>(Point(m_data->pos), Point({ 55, 40 }), Point({ 91, 154 }), std::array<String, 4>{ L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, Point(m_data->movement));
		grounds.push_back(std::make_shared<Ground>(Point{ -1000,-700 }, Point{ 3000, 2700 }, String(L"soil"), 7, 7));

		objects.push_back(std::make_shared<Rock>(Point{ 270, 580 }, Point{ 370, 370 }, Point{ -70, -120 }, L"stone", 14, 3));
		objects.push_back(std::make_shared<Rock>(Point{ -1200, -2050 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 5, 20));
		objects.push_back(std::make_shared<Rock>(Point{ -205, -1000 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 13, 5));
		objects.push_back(std::make_shared<Rock>(Point{ 3045, -1480 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 5, 15));
		objects.push_back(std::make_shared<Rock>(Point{ 0, -2080 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 16, 3));
		changeBattle = { Random(80,400),Random(80,400) };
		objects.push_back(player);
		if (!m_data->Donald_finish)
		{
			objects.push_back(std::make_shared<Mobu>(Point{ 2635, -950 }, Point{ 40, 30 }, MobuDirection::Right, Point{ 100 ,173 }, std::array<String, 4>{ L"PieroR", L"PieroL", L"PieroB", L"PieroF" }, 11));
		}
		else if (m_data->Donald_finish)
		{
			objects.push_back(std::make_shared<Mobu>(Point{ 2635, -950 }, Point{ 40, 30 }, MobuDirection::Right, Point{ 100 ,173 }, std::array<String, 4>{ L"PieroR", L"PieroL", L"PieroB", L"PieroF" }, 13));
		}

		change_scene_rects.push_back(changerect{ Rect(-300, 800, 1000, 20), L"FirstField", {-1150,-2150} });
		change_scene_rects.push_back(changerect{ Rect(1320,-1680,50,480) , L"ThirdField", { 1500,-25 } });
		battle_scenes.push_back(String(L"Battle2"));

	}
};


class thirdField :public Field
{
	void init()
	{
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		grounds.push_back(std::make_shared<Ground>(Point{ -2000,-1500 }, Point{ 3000, 2700 }, String(L"soil"), 8, 8));
		objects.push_back(std::make_shared<Mobu>(Point{ -1990, -2150 }, Point{ 40, 30 }, MobuDirection::Forward, Point{ 94 ,158 }, std::array<String, 4>{ L"Mobu2R", L"Mobu2L", L"Mobu2B", L"Mobu2F" }, 14));
		objects.push_back(std::make_shared<Mobu>(Point{ -1885, -3000 }, Point{ 40, 30 }, MobuDirection::Forward, Point{ 95 ,152 }, std::array<String, 4>{ L"Chara2R", L"CharaL", L"CharaB", L"CharaF" }, 8));
		player = std::make_shared<Player>(Point(m_data->pos), Point({ 55, 40 }), Point({ 91, 154 }), std::array<String, 4>{ L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, Point(m_data->movement));
		changeBattle = { Random(50,320),Random(80,350) };
		objects.push_back(player);
		change_scene_rects.push_back(changerect{ Rect(1605,-260,50,480) , L"SecondField",{ 1450,-1500 } });
		objects.push_back(std::make_shared<Rock>(Point{ 200, -305 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 20, 1));
		objects.push_back(std::make_shared<Rock>(Point{ -350, 280 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 25, 1));
		objects.push_back(std::make_shared<Rock>(Point{ -400, -1000 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 1, 7));
		objects.push_back(std::make_shared<Rock>(Point{ 145, -1350 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 1, 6));
		objects.push_back(std::make_shared<Rock>(Point{ -1520, -1500 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 9, 1));
		objects.push_back(std::make_shared<Rock>(Point{ -2005, -1000 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 9, 1));
		objects.push_back(std::make_shared<Rock>(Point{ -2200, -2225 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 1, 7));
		objects.push_back(std::make_shared<Rock>(Point{ -1580, -2220 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 1, 4));
		objects.push_back(std::make_shared<Rock>(Point{ -2680, -2350 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 3, 1));
		objects.push_back(std::make_shared<Rock>(Point{ -1565, -2350 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 3, 1));
		objects.push_back(std::make_shared<Rock>(Point{ -2880, -3300 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 1, 6));
		objects.push_back(std::make_shared<Rock>(Point{ -980, -3300 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 1, 6));
		objects.push_back(std::make_shared<Rock>(Point{ -2880, -3400 }, Point{ 370, 370 }, Point{ -50, -100 }, L"stone", 10, 1));
		battle_scenes.push_back(String(L"Battle3"));
	}
};

class Clear :public SceneManager<String, Gamedata>::Scene
{
public:
	Font font = Font(60);
	const String text3 = L"Game     Clear";
	void init()
	{
		Window::Resize(1200, 750);
		TextureAsset::Register(L"sky", L"Images/sky.jpg");
	}
	void update() override
	{
		if (Input::KeyEnter.clicked)
		{
			m_data->pos = { 0,400 };
			changeScene(L"Title");
		}
	}
	void draw() const override
	{
		TextureAsset(L"sky").scale(0.8, 0.8).draw();
		font(text3).drawCenter(600, 300);
	}
};