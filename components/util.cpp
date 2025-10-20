#include "../headers/util.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

namespace Util {

// helper for clamping values
static inline float clamp(float v, float min, float max) {
  return std::max(min, std::min(max, v));
}

void drawRoundedRect(sf::RenderTarget &target, const sf::FloatRect &rect,
                     const sf::Color &fillColor, const float radii[4]) {
  // Simplify if all radii are 0
  if (radii[0] <= 0 && radii[1] <= 0 && radii[2] <= 0 && radii[3] <= 0) {
    sf::RectangleShape box({rect.width, rect.height});
    box.setPosition(rect.left, rect.top);
    box.setFillColor(fillColor);
    target.draw(box);
    return;
  }

  const int segments = 12; // smoothness
  sf::VertexArray verts(sf::TriangleFan);
  sf::Vector2f pos(rect.left, rect.top);
  sf::Vector2f size(rect.width, rect.height);

  sf::RenderTexture tex;
  tex.create(static_cast<unsigned int>(rect.width),
             static_cast<unsigned int>(rect.height));
  tex.clear(sf::Color::Transparent);

  // Helper lambda for one corner
  auto drawCorner = [&](sf::Vector2f cornerPos, float radius,
                        float startAngleDeg) {
    sf::ConvexShape corner;
    int points = std::max(3, static_cast<int>(segments * (radius > 0 ? 1 : 0)));
    corner.setPointCount(points + 1);
    corner.setPoint(0, sf::Vector2f(radius, radius)); // center
    for (int i = 0; i < points; ++i) {
      float angle =
          (startAngleDeg + (90.0f / (points - 1)) * i) * 3.1415926f / 180.0f;
      corner.setPoint(i + 1, {radius + std::cos(angle) * radius,
                              radius + std::sin(angle) * radius});
    }
    corner.setFillColor(fillColor);
    corner.setPosition(cornerPos);
    tex.draw(corner);
  };

  // central rect (without rounded corners)
  sf::RectangleShape body({size.x, size.y});
  body.setFillColor(fillColor);
  tex.draw(body);

  // draw corners
  drawCorner({0, 0}, radii[0], 180.0f);                     // top-left
  drawCorner({size.x - 2 * radii[1], 0}, radii[1], 270.0f); // top-right
  drawCorner({size.x - 2 * radii[2], size.y - 2 * radii[2]}, radii[2],
             0.0f);                                        // bottom-right
  drawCorner({0, size.y - 2 * radii[3]}, radii[3], 90.0f); // bottom-left

  tex.display();

  sf::Sprite sprite(tex.getTexture());
  sprite.setPosition(rect.left, rect.top);
  target.draw(sprite);
}

void drawRoundedBorder(sf::RenderTarget &target, const sf::FloatRect &rect,
                       const sf::Color &borderColor, const float radii[4],
                       float borderWidth) {
  if (borderWidth <= 0.0f)
    return;

  // Outer
  drawRoundedRect(target, rect, borderColor, radii);

  // Inner — shrunk by border width
  sf::FloatRect inner = {rect.left + borderWidth, rect.top + borderWidth,
                         rect.width - borderWidth * 2,
                         rect.height - borderWidth * 2};

  float innerRadii[4];
  for (int i = 0; i < 4; ++i)
    innerRadii[i] = std::max(0.0f, radii[i] - borderWidth);

  // Cut out the inner shape using transparent fill
  sf::RenderTexture mask;
  mask.create((unsigned int)rect.width, (unsigned int)rect.height);
  mask.clear(sf::Color::Transparent);

  // Draw the inner rect to erase the middle area
  drawRoundedRect(mask, {borderWidth, borderWidth, inner.width, inner.height},
                  sf::Color::Transparent, innerRadii);

  mask.display();
  sf::Sprite borderSprite(mask.getTexture());
  borderSprite.setPosition(rect.left, rect.top);
  target.draw(borderSprite);
}

} // namespace Util
