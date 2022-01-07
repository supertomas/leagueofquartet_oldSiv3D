#pragma once
# include <Siv3D.hpp>
#include<HamFramework.hpp>
#include"Mobu.hpp"
#include"Indoor.hpp"
class In :public SceneManager<String, Gamedata>::Scene
{
	public:
	std::shared_ptr<Player> player;
	Array<std::shared_ptr<Object>> objects;
	
	Rect bed_body;


	void update() override
	{
		std::sort(objects.begin(), objects.end(), [](std::shared_ptr<Object> a, std::shared_ptr<Object> b)
		{
			return a->pos.y < b->pos.y;
		});

		for (auto& object : objects)
		{
			object->update(player);
		}

		if (player->getFace().intersects(bed_body))
		{
			player->pos -= player->speed;
			error_bed = true;
		}
		if (error_bed == true && Input::KeyEnter.clicked)
		{
			TextManager::text->start(4);
		}
		String flag = TextManager::text->getFlag();
		if (flag != L"")
		{
			changeScene(flag);
			m_data->pos = player->pos;
			m_data->Firstmessage = true;
		}
		TextManager::text->update();
	}
	void draw() const override
	{
		Graphics::SetBackground(Color(15, 15, 15));
		Graphics2D::PushTransform(Mat3x2::Translate(-player->pos.x + Window::Size().x / 2, -player->pos.y + Window::Size().y / 2));
		for (auto& object : objects)
		{
			object->draw();
		}

		Graphics2D::PopTransform();
		TextManager::text->draw();
	}
private:
	bool error_bed = false;

};
class In1 :public In
{
	void init()
	{
		player = std::make_shared<Player>(m_data->pos, Point{ 55, 40 }, Point{ 95, 152 }, std::array<String, 4> { L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, Point(m_data->movement));
		objects.push_back(std::make_shared<Indoor>(Point{ 0, 0 }, Point{ 800,760 }, Point{ 360, 480 }, Point{ 5, 5 }, String(L"indoor"), this, Point{ 310, -340 }));
		objects.push_back(player);
	}

};

class In2 :public In
{
	void init()
	{
		player = std::make_shared<Player>(m_data->pos, Point{ 55, 40 }, Point{ 95, 152 }, std::array<String, 4> { L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, Point(m_data->movement));
		objects.push_back(std::make_shared<Indoor>(Point({ 0, 0 }), Point{ 800,760 }, Point{ 360, 480 }, Point({ 5, 5 }), String(L"indoor"), this, Point({ -390, 40 })));
		objects.push_back(player);
		objects.push_back(std::make_shared<Mobu>(Point({ -290, 620 }), Point({ 40, 30 }), MobuDirection::Forward, Point({ 71, 155 }), std::array<String, 4>{ L"Mobu1R", L"Mobu1L", L"Mobu1B", L"Mobu1F" }, 2));

	}
};

class In3 :public In
{
	void init()
	{
		if (m_data->Firstmessage == false)
		{
			TextManager::text->start(12);
		}
		player = std::make_shared<Player>(m_data->pos, Point{ 55, 40 }, Point{ 95, 152 }, std::array<String, 4> { L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, Point(m_data->movement));
		objects.push_back(std::make_shared<mainIndoor>(Point({ 0, 0 }), Point{ 700,660 }, Point{ 180, 260 }, Point({ 5, 5 }), String(L"mainHouse"), this, Point({ 280, 490 })));
		objects.push_back(player);
		bed_body = Rect(-320, 180, 200, 100);

	}
};
