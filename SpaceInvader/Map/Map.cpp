#include "Map.h"
#include "../WinAPI/WinAPI.h"
#include "../Projectile/Projectile.h"

Map::Map(size_t frame_x, size_t frame_y, size_t width, size_t height) : 
	_frame_x(frame_x),
	_frame_y(frame_y),
	_width(width),
	_height(height),
	_stars_update_timer(_stars_update_delay)
{
	_frame_width = width + 2;
	_frame_height = height + 2;

	std::uniform_int_distribution<size_t> stars_distribution(1, _frame_y + _height);
	for (size_t i = 1; i <= _width; i = i + 3) {
		_stars.emplace_back(*this, _frame_x + i, stars_distribution(_stars_random_device));
	}
}

void Map::update()
{
	if (!_stars_update_timer.tick()) {
		static std::uniform_int_distribution<size_t> stars_distribution(1, _frame_y + _height);
		for (auto&& star : _stars) {
			if (!star.move()) {
				star.move_to(_frame_x + _width, stars_distribution(_stars_random_device));
				star.show();
			}
		}
	}
}

void Map::show()
{
	this->show_frame();
	for (auto&& star : _stars) { star.show(); }
}

void Map::show_frame()
{
	WinAPI::put_symbol(218, _frame_x, _frame_y);
	WinAPI::put_symbol(191, _frame_x + _frame_width - 1, _frame_y);
	WinAPI::put_symbol(192, _frame_x, _frame_y + _frame_height - 1);
	WinAPI::put_symbol(217, _frame_x + _frame_width -1, _frame_y + _frame_height - 1);

	WinAPI::put_symbol(196, _frame_x + 1, _frame_y, _frame_width - 2);
	WinAPI::put_symbol(196, _frame_x + 1, _frame_y + _frame_height - 1, _frame_width - 2);
	for (size_t i = 1; i < _frame_height - 1; ++i)
	{
		WinAPI::put_symbol(179, _frame_x, _frame_y + i);
		WinAPI::put_symbol(' ', _frame_x + 1, _frame_y + i, _frame_width - 2);
		WinAPI::put_symbol(179, _frame_x + _frame_width - 1, _frame_y + i);
	}
}

Rect Map::frame_position()
{
	return {static_cast<int>(_frame_x), static_cast<int>(_frame_y), static_cast<int>(_frame_width), static_cast<int>(_frame_height)};
}

Rect Map::position()
{
	return { static_cast<int>(_frame_x + 1), static_cast<int>(_frame_y + 1), static_cast<int>(_width), static_cast<int>(_height) };
}

Map::Star::Star(Map& map, size_t x, size_t y) : MoveComponent(x, y), VisibleComponent(x, y, '*'), Coordinates(x, y)
{}

bool Map::Star::move()
{
	VisibleComponent::hide();
	--_x;
	if (!_x) { return false; }
	VisibleComponent::show();
	return true;
}
