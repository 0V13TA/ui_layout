#include "../headers/element.hpp"
#include "../headers/container.hpp"
#include <cctype>
#include <iostream>
#include <stdexcept> // For std::invalid_argument, std::out_of_range

Element::Element(sf::RenderWindow &wind) : window(wind) {}

// Implementation of the parseUnit method
float Element::parseUnit(const std::string &unit, Axis axis) const {
  if (unit.empty()) {
    return 0.0f;
  }

  try {
    // --- Case 1: Percentage (%) ---
    if (unit.back() == '%') {
      float value = std::stof(unit.substr(0, unit.length() - 1));
      if (!parent) {
        // No parent, so percentage is meaningless. Default to 0.
        return 0.0f;
      }

      float parentWidth = parseUnit(parent->style.width, Axis::Vertical);
      float parentHeight = parseUnit(parent->style.height, Axis::Horizontal);

      if (axis == Axis::Horizontal) {
        return parentWidth * (value / 100.0f);
      } else {
        return parentHeight * (value / 100.0f);
      }
    }

    // --- Case 2: Viewport Width (vw) ---
    else if (unit.length() > 2 && unit.substr(unit.length() - 2) == "vw") {
      float value = std::stof(unit.substr(0, unit.length() - 2));
      float windowWidth = static_cast<float>(window.getSize().x);
      return windowWidth * (value / 100.0f);
    }

    // --- Case 3: Viewport Height (vh) ---
    else if (unit.length() > 2 && unit.substr(unit.length() - 2) == "vh") {
      float value = std::stof(unit.substr(0, unit.length() - 2));
      float windowHeight = static_cast<float>(window.getSize().y);
      return windowHeight * (value / 100.0f);
    }

    // --- Case 4: Pixels (px or no unit) ---
    else if (unit.length() > 2 && unit.substr(unit.length() - 2) == "px") {
      return std::stof(unit.substr(0, unit.length() - 2));
    } else {
      // If no unit is specified, assume it's pixels
      return std::stof(unit);
    }

  } catch (const std::invalid_argument &e) {
    // Could not perform conversion (e.g., unit was "abc%")
    std::cerr << "Warning: Invalid number format in unit string '" << unit
              << "'\n";
    return 0.0f;
  } catch (const std::out_of_range &e) {
    // Value was too large to fit in a float
    std::cerr << "Warning: Value out of range in unit string '" << unit
              << "'\n";
    return 0.0f;
  }
}

BoxModel Element::getBoxModel() {
  // Calculate all box model values
  for (int i = 0; i < 4; i++) {
    boxModel.border[i] = parseUnit(
        style.border[i], (i % 2 == 0) ? Axis::Vertical : Axis::Horizontal);
    boxModel.margin[i] = parseUnit(
        style.margin[i], (i % 2 == 0) ? Axis::Vertical : Axis::Horizontal);
    boxModel.padding[i] = parseUnit(
        style.padding[i], (i % 2 == 0) ? Axis::Vertical : Axis::Horizontal);
  }

  // Content size (width/height without padding/border/margin)
  float contentWidth = parseUnit(style.width, Axis::Horizontal);
  float contentHeight = parseUnit(style.height, Axis::Vertical);

  // Full computed size including padding + border
  boxModel.computedSize.x = contentWidth + boxModel.padding[1] +
                            boxModel.padding[3] + boxModel.border[1] +
                            boxModel.border[3];
  boxModel.computedSize.y = contentHeight + boxModel.padding[0] +
                            boxModel.padding[2] + boxModel.border[0] +
                            boxModel.border[2];

  return boxModel;
}

sf::Vector2f Element::getContentPosition() const {
  // Position of the actual content (inside padding + border + margin)
  return {computedPosition.x + boxModel.margin[3] + boxModel.border[3] +
              boxModel.padding[3],
          computedPosition.y + boxModel.margin[0] + boxModel.border[0] +
              boxModel.padding[0]};
}

sf::Vector2f Element::getContentSize() const {
  // Size of the actual content area
  return {parseUnit(style.width, Axis::Horizontal),
          parseUnit(style.height, Axis::Vertical)};
}

sf::FloatRect Element::getContentRect() const {
  auto pos = getContentPosition();
  auto size = getContentSize();
  return {pos.x, pos.y, size.x, size.y};
}

sf::FloatRect Element::getPaddingRect() const {
  // Area including content + padding
  auto contentPos = getContentPosition();
  auto contentSize = getContentSize();
  return {
      contentPos.x - boxModel.padding[3],                        // left
      contentPos.y - boxModel.padding[0],                        // top
      contentSize.x + boxModel.padding[1] + boxModel.padding[3], // width
      contentSize.y + boxModel.padding[0] + boxModel.padding[2]  // height
  };
}

sf::FloatRect Element::getBorderRect() const {
  // Area including content + padding + border
  auto paddingRect = getPaddingRect();
  return {
      paddingRect.left - boxModel.border[3],                       // left
      paddingRect.top - boxModel.border[0],                        // top
      paddingRect.width + boxModel.border[1] + boxModel.border[3], // width
      paddingRect.height + boxModel.border[0] + boxModel.border[2] // height
  };
}

sf::FloatRect Element::getMarginRect() const {
  // Full area including content + padding + border + margin
  auto borderRect = getBorderRect();
  return {
      borderRect.left - boxModel.margin[3],                       // left
      borderRect.top - boxModel.margin[0],                        // top
      borderRect.width + boxModel.margin[1] + boxModel.margin[3], // width
      borderRect.height + boxModel.margin[0] + boxModel.margin[2] // height
  };
}

void Element::drawBackground(const sf::FloatRect &rect,
                             const sf::Color &color) {
  if (color != sf::Color::Transparent) {
    sf::RectangleShape bg(sf::Vector2f(rect.width, rect.height));
    bg.setFillColor(color);
    bg.setPosition(rect.left, rect.top);
    window.draw(bg);
  }
}

void Element::drawBorder(const sf::FloatRect &rect, const sf::Color &color,
                         float thickness) {
  if (thickness > 0 && color != sf::Color::Transparent) {
    sf::RectangleShape border(sf::Vector2f(rect.width, rect.height));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(color);
    border.setOutlineThickness(thickness);
    border.setPosition(rect.left, rect.top);
    window.draw(border);
  }
}
