#pragma once
#include "./renderer.hpp"
#include <SFML/Graphics.hpp>
#include <string>

using uint = unsigned int;

class Container;

struct Styles {
  std::string id = "";
  std::string width = "0";
  std::string height = "0";
  std::string className = "";
  std::string border[4] = {"0", "0", "0", "0"};
  std::string margin[4] = {"0", "0", "0", "0"};
  std::string padding[4] = {"0", "0", "0", "0"};

  bool visible = true;
  float borderWidth = 0.0f;
  sf::Color borderColor = sf::Color::Black;
  sf::Color backgroundColor = sf::Color::Transparent;

  int relZIndex = 0;  // local stacking
  int absZIndex = -1; // global stacking (negative = disabled)
};

struct BoxModel {
  // NOTE:             top right bottom left
  float border[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  float margin[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  float padding[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  sf::Vector2f computedSize = {0.0f, 0.0f}; // {width, height}
};

enum class Axis { Horizontal, Vertical };

class Element {
public:
  explicit Element(sf::RenderWindow &wind);
  virtual ~Element() = default;

  virtual void draw() = 0;

  // Update the function declaration
  BoxModel getBoxModel();
  float parseUnit(const std::string &unit, Axis axis) const;

  // Get computed positions including margins/padding
  sf::Vector2f getContentSize() const;
  sf::FloatRect getContentRect() const;
  sf::Vector2f getContentPosition() const;

  // Get full bounds including margins
  sf::FloatRect getMarginRect() const;
  sf::FloatRect getBorderRect() const;
  sf::FloatRect getPaddingRect() const;

  Container *getParent() const { return parent; }
  void setParent(Container *newParent) { parent = newParent; }

  // PASS 1: update/layout. This should submit only absolute elements
  virtual void update(Renderer &renderer) {
    // Default: if element is absolute, submit it for later drawing
    if (style.absZIndex >= 0) {
      renderer.addToGlobalDrawList(this);
      return; // don't recurse for absolute elements (they escape local
              // stacking)
    }
    // otherwise default does nothing; containers override to handle children
  }

  Styles style;
  BoxModel boxModel;
  sf::Vector2f computedPosition = {0.0f, 0.0f};

protected:
  sf::RenderWindow &window;
  Container *parent = nullptr;

  // Helper methods for drawing box model
  void drawBackground(const sf::FloatRect &rect, const sf::Color &color);
  void drawBorder(const sf::FloatRect &rect, const sf::Color &color,
                  float thickness);
};
