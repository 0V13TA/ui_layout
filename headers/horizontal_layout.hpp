#pragma once
#include "./container.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>

enum class JustifyContent {
  Start,
  Center,
  End,
  SpaceBetween,
  SpaceAround,
  SpaceEvenly
};

enum class AlignItems { Start, Center, End };

enum class WrapMode { NoWrap, Wrap };

/**
 * @brief A layout container that arranges children horizontally.
 *
 * Supports:
 * - gap (spacing between items)
 * - justify-content (main-axis alignment)
 * - align-items (cross-axis alignment)
 * - wrap (multi-line layout)
 */
class HorizontalLayout : public Container {
public:
  using Container::Container; // inherit constructor

  // Layout properties
  JustifyContent justifyContent = JustifyContent::Start;
  AlignItems alignItems = AlignItems::Start;
  WrapMode wrap = WrapMode::NoWrap;
  float gap = 0.0f;

  // -------------------------------------------------------------
  // Draw background + border (container appearance)
  // -------------------------------------------------------------
  void drawSelf() override {
    drawBackground(getBorderRect(), style.backgroundColor);
    drawBorder(getBorderRect(), style.borderColor, boxModel.border[0]);
  }

  // -------------------------------------------------------------
  // Arrange children horizontally with spacing and alignment
  // -------------------------------------------------------------
  void arrangeChildren() override {
    if (children.empty())
      return;

    const float containerWidth = parseUnit(style.width, Axis::Horizontal);
    const float containerHeight = parseUnit(style.height, Axis::Vertical);
    const float paddingLeft = boxModel.padding[3];
    const float paddingTop = boxModel.padding[0];

    float x = computedPosition.x + paddingLeft;
    float y = computedPosition.y + paddingTop;

    // ---------- Compute total width of all children ----------
    float totalWidth = 0.0f;
    for (auto &ch : children) {
      totalWidth += ch->getBoxModel().computedSize.x;
    }
    totalWidth += gap * (children.size() - 1);

    // ---------- Calculate justification ----------
    float extraSpace = containerWidth - totalWidth;
    float startOffset = 0.0f;

    switch (justifyContent) {
    case JustifyContent::Center:
      startOffset = extraSpace / 2.0f;
      break;
    case JustifyContent::End:
      startOffset = extraSpace;
      break;
    case JustifyContent::SpaceBetween:
      if (children.size() > 1)
        gap = extraSpace / (children.size() - 1);
      break;
    case JustifyContent::SpaceAround:
      gap = extraSpace / children.size();
      startOffset = gap / 2.0f;
      break;
    case JustifyContent::SpaceEvenly:
      gap = extraSpace / (children.size() + 1);
      startOffset = gap;
      break;
    default:
      break;
    }

    x += std::max(0.0f, startOffset);

    // ---------- Position children ----------
    float lineHeight = 0.0f; // for wrapping
    for (auto &ch : children) {
      auto &childBox = ch->getBoxModel();
      float cw = childBox.computedSize.x;
      float chh = childBox.computedSize.y;

      // Wrap if enabled and child exceeds container width
      if (wrap == WrapMode::Wrap &&
          x + cw > computedPosition.x + containerWidth - paddingLeft) {
        x = computedPosition.x + paddingLeft;
        y += lineHeight + gap;
        lineHeight = 0.0f;
      }

      // Align items vertically
      float offsetY = 0.0f;
      switch (alignItems) {
      case AlignItems::Center:
        offsetY = (containerHeight - chh) / 2.0f;
        break;
      case AlignItems::End:
        offsetY = containerHeight - chh;
        break;
      default:
        break;
      }

      ch->computedPosition = {x, y + offsetY};
      x += cw + gap;
      lineHeight = std::max(lineHeight, chh);
    }
  }

  // -------------------------------------------------------------
  // Update pass (layout + recursive update)
  // -------------------------------------------------------------
  void update(Renderer &renderer) override {
    arrangeChildren();
    Container::update(renderer); // recurse and submit absZ children
  }
};
