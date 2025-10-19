#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

using uint = unsigned int;

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
  float border[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  float margin[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  float padding[4] = {0.0f, 0.0f, 0.0f, 0.0f};
};

enum class Axis { Horizontal, Vertical };

class Element {
public:
  explicit Element(sf::RenderWindow &wind);
  virtual ~Element() = default;

  virtual void draw() = 0;
  virtual bool isContainer() const { return false; }

  virtual void addChild(std::shared_ptr<Element> child);

  // Update the function declaration
  float parseUnit(const std::string &unit, Axis axis);
  BoxModel getBoxModel();

  const std::vector<std::shared_ptr<Element>> &getChildren() const {
    return children;
  }

  Styles style;

protected:
  sf::RenderWindow &window;
  std::vector<std::shared_ptr<Element>> children;
  Element *parent = nullptr;
};
