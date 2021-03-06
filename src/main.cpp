#include "engine/game_core.h"
#include "rank_table/rank_table.h"
#include "scenes/rank_table_scene.h"
#include "scenes/game_scene.h"
#include "scenes/main_title_scene.h"
#include "rank_table/rank_table_text_file.h"
#include <iostream>
#include <SFML/Graphics.hpp>

const float x_velocity = 0.9f;
const float y_velocity = -6.f;
const float y_flap_velocity = 2.f;
const int map_size = 3;
const float dist_between_tubes = 0.9f;
const float tube_width = 0.4f;
const float hole_size = 0.5f;
const float start_pos = -2.f;
const float bird_height = 0.08f;
const float birg_width = 0.08f;

GameCore_t game_core(
	map_size, 
	x_velocity, 
	y_velocity, 
	y_flap_velocity, 
	dist_between_tubes, 
	tube_width, 
	hole_size, 
	start_pos, 
	bird_height, 
	birg_width
);

int main() {
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(500, 500), "Flappy Cube", sf::Style::Titlebar | sf::Style::Close);

	sf::Font font;
	if (!font.loadFromFile("resources\\fonts\\machine_c.otf")) {
		return EXIT_FAILURE;
	}

	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	sf::Clock deltaClock;
	deltaClock.restart();

	RankTableTextFile_t rank_table("scores.txt");
	try {
		rank_table.load_ranks();
	} catch(RankTableException_t) {
		std::cout << "Can't load ranks" << std::endl;
	}

	RankTableScene_t rank_table_scene(rank_table, font);
	rank_table_scene.set_size(sf::Vector2f(400, 300));

	GameScene_t game_scene(game_core);
	game_scene.set_font(font);

	MainTitleScene_t main_title_scene(font);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			rank_table_scene.send_event(event);
			game_scene.send_event(event);
			main_title_scene.send_event(event);
		}

		sf::Time dt = deltaClock.restart();
		auto dtm = dt.asMilliseconds() / 1000.f;

		window.clear();

		game_scene.step(dtm);
		window.draw(game_scene);

		if (main_title_scene.get_state() == MainTitleScene_t::STARTED) {
			if (game_scene.get_state() == GameScene_t::WAIT) {
				game_scene.start();
			}
		} else {
			main_title_scene.step(dtm);
			window.draw(main_title_scene);
		}

		if (game_scene.get_state() == GameScene_t::DEAD) {
			rank_table_scene.set_score(game_core.get_current_ceil());
			rank_table_scene.step(dtm);
			window.draw(rank_table_scene);

			if (rank_table_scene.get_state() == RankTableScene_t::RESTART_REQUESTED) {
				rank_table_scene.reset();
				game_scene.reset();
			}
		}

		window.display();
	}

	return 0;
}
