#include <gtc/matrix_transform.hpp>
#include "engine/game_core.h"
#include "rank_table/rank_table.h"
#include "scenes/rank_table_scene.h"
#include "scenes/game_scene.h"
#include <iostream>
#include "scenes/main_title_scene.h"
#include "rank_table/rank_table_text_file.h"

const int map_size = 3;
const float dist_between_tubes = 0.9f;
const float tube_width = 0.4f;

GameCore_t game_core(map_size, 1.f, -4.f, 1.3f, dist_between_tubes, tube_width, 0.5f, -2.f, 0.08f, 0.08f);
#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(500, 500), "Flappy Cube");

	sf::Font font;
	if (!font.loadFromFile("fonts\\machine_c.otf")) {
		std::cout << "Can't open \"fonts\\machine_c.otf\" file" << std::endl;
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
	rank_table_scene.move(50, 150);

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
