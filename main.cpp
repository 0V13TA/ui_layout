#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode({512, 512}), "UI Layout System");

  while (window.isOpen()) {

    sf::Event evnt;
    while (window.pollEvent(evnt)) {

      switch (evnt.type) {

      case sf::Event::Closed:
          printf("It is closed \n");
        window.close();
        break;
      }
    }

    window.clear(sf::Color::White);
    window.display();
  }

  return 0;
}
