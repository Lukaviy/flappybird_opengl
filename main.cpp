#include <gtc/matrix_transform.hpp>
#include "engine/game_core.h"
#include "rank_table/rank_table.h"
#include "rank_table/rank_table_db_text_file.h"
#include "scenes/rank_table_scene.h"

const int map_size = 3;
const float dist_between_tubes = 0.9f;
const float tube_width = 0.4f;

GameCore_t<map_size> game_core(1.f, -4.f, 1.3f, dist_between_tubes, tube_width, 0.5f, -2.f, 0.08f, 0.08f);
#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(500, 500), "Flappy Bird");
	sf::RectangleShape bird(sf::Vector2<float>(0.1f, 0.1f));
	sf::RectangleShape up_tube(sf::Vector2<float>(tube_width, 2.f));
	sf::RectangleShape bottom_tube(sf::Vector2<float>(tube_width, 2.f));
	sf::View game_view(sf::FloatRect(-1.f, 1.f, 2.f, -2.f));
	sf::View gui_view(sf::FloatRect(0.f, 0.f, 500.f, 500.f));
	bird.setFillColor(sf::Color::Red);
	up_tube.setFillColor(sf::Color::Green);
	bottom_tube.setFillColor(sf::Color::Green);
	bird.move(-0.05f, -0.05f);
	up_tube.move(-tube_width / 2.f, 0.f);
	bottom_tube.move(-tube_width / 2.f, -2.f);

	sf::Font font;
	if (!font.loadFromFile("fonts\\machine_c.otf"))
		return EXIT_FAILURE;

	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	window.setView(game_view);

	sf::Clock deltaClock;
	deltaClock.restart();

	RankTableTextFileDb_t rank_table_db("scores.txt");

	RankTable_t rank_table(rank_table_db);

	RankTableScene_t rank_table_scene(rank_table);
	rank_table_scene.set_size(sf::Vector2f(400, 300));
	rank_table_scene.move(50, 150);
	rank_table_scene.set_font(font);

	while (window.isOpen()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			game_core.reset();
			rank_table_scene.reset();
		}

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			rank_table_scene.send_event(event);
		}

		sf::Time dt = deltaClock.restart();
		auto dtm = dt.asMilliseconds() / 1000.f;

		game_core.step(dtm, sf::Keyboard::isKeyPressed(sf::Keyboard::Space));

		auto bird_pos = game_core.get_bird_pos();

		auto bv = game_core.get_bird_velocity();

		float velocity = length(glm::vec2(bv.x, bv.y));
		float cos_b = bv.y / velocity;
		float sin_b = bv.x / velocity;

		sf::Transform t3 (
			cos_b, sin_b, 0,
			-sin_b, cos_b, bird_pos.y,
			0, 0, 1
		);

		window.clear();

		window.setView(game_view);

		auto map = game_core.get_map();
		int curr_ceil = game_core.get_current_ceil();
		
		for (int i = 0; i < map_size; i++) {
			auto tube = map[(curr_ceil + i) % map_size];

			float hole_size = tube.hole_size;
			float hole_pos = tube.y_pos;
			float x_pos = tube.x_pos - bird_pos.x;

			sf::Transform up_tube_transform;
			sf::Transform bottom_tube_transform;
			up_tube_transform.translate(x_pos, hole_pos + hole_size / 2.f);
			bottom_tube_transform.translate(x_pos, hole_pos - hole_size / 2.f);

			window.draw(up_tube, up_tube_transform);
			window.draw(bottom_tube, bottom_tube_transform);
		}
		
		window.draw(bird, t3);

		sf::Text text(std::to_string(game_core.get_current_ceil()), font, 50.f);
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
		text.move(250.f, 20.f);

		window.setView(gui_view);
		window.draw(text);

		if (game_core.get_state() == GameCore_t<10>::State_t::DEAD) {
			rank_table_scene.set_score(game_core.get_current_ceil());
			rank_table_scene.step(dtm);
			window.draw(rank_table_scene);
		}

		window.display();
	}

	return 0;
}
