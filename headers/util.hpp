
#pragma once
#include <SFML/Graphics.hpp>

namespace Util {

/**
 * @brief Draw a rounded rectangle with individual corner radii.
 *
 * @param target The render target to draw on.
 * @param rect   The rectangle bounds.
 * @param fillColor The fill color.
 * @param radii  The corner radii: top-left, top-right, bottom-right,
 * bottom-left.
 */
void drawRoundedRect(sf::RenderTarget &target, const sf::FloatRect &rect,
                     const sf::Color &fillColor, const float radii[4]);

/**
 * @brief Draw only the border of a rounded rectangle.
 * This is useful for the box model border layer.
 *
 * @param target The render target.
 * @param rect   The full border rectangle bounds.
 * @param borderColor Color of the border.
 * @param radii  The corner radii for the outer edge.
 * @param borderWidth How thick the border is (defaults to 1px).
 */
void drawRoundedBorder(sf::RenderTarget &target, const sf::FloatRect &rect,
                       const sf::Color &borderColor, const float radii[4],
                       float borderWidth = 1.0f);

} // namespace Util
