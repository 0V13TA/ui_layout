#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode({512, 512}), "UI Layout System");

  while (window.isOpen()) {

    sf::Event evnt;
    while (window.pollEvent(evnt)) {
      if (evnt.type == sf::Event::Closed) {
        printf("It is now closed");
        window.close();
      }
    }

    window.clear(sf::Color::White);
    window.display();
  }

  return 0;
}
