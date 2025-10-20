#include "./headers/container.hpp"
#include "./headers/renderer.hpp"
#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode(512, 512), "UI Layout Test");

  Renderer renderer;

  // Root container
  auto root = std::make_shared<HorizontalLayout>(window);
  root->style.width = "256px";
  root->style.height = "256px";
  root->style.backgroundColor = sf::Color(230, 230, 230); // light gray
  root->justifyContent = JustifyContent::SpaceAround;
  root->alignItems = AlignItems::Center;
  // root->gap = 20.0f;

  // Add children (colored boxes)
  for (int i = 0; i < 3; ++i) {
    auto box = std::make_shared<HorizontalLayout>(window);
    box->style.width = "100px";
    box->style.height = "100px";
    box->style.backgroundColor = sf::Color(100 + i * 40, 0, 250 - i * 50);
    box->style.borderColor = sf::Color::Black;
    box->boxModel.border[0] = 2; // top border (we only use one value for all)
    root->addChild(box);
  }

  auto verticalBox = std::make_shared<VerticalLayout>(window);
  verticalBox->style.width = "200px";
  verticalBox->style.width = "400px";
  verticalBox->style.backgroundColor = sf::Color::Green;
  verticalBox->style.borderColor = sf::Color::Black;
  verticalBox->boxModel.border = {10.0f, 10.0f, 10.0f, 10.0f};
  verticalBox->style.absZIndex = 10;
  root->addChild(verticalBox);

  renderer.setRoot(root.get());

  while (window.isOpen()) {
    sf::Event evnt;
    while (window.pollEvent(evnt)) {
      if (evnt.type == sf::Event::Closed)
        window.close();
    }

    window.clear(sf::Color::White);

    root->update(renderer);
    root->draw();

    window.display();
  }

  return 0;
}
