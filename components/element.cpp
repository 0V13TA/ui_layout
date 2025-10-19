#include "../headers/element.hpp"
#include <stdexcept> // For std::invalid_argument, std::out_of_range

// Implementation of the parseUnit method
float Element::parseUnit(const std::string &unit, Axis axis) {
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

      // Get parent's computed size (assuming you have a way to get this)
      // For this example, let's pretend the parent's size is stored in a
      // BoxModel member. You would replace this with your actual layout
      // calculation logic. BoxModel parentLayout = parent->getBoxModel(); float
      // parentWidth = parentLayout.width;  // You'll need to add width/height
      // to BoxModel float parentHeight = parentLayout.height;

      // FAKE DATA for demonstration:
      float parentWidth = 500.0f;
      float parentHeight = 300.0f;

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
    // std::cerr << "Warning: Invalid number format in unit string '" << unit <<
    // "'\n";
    return 0.0f;
  } catch (const std::out_of_range &e) {
    // Value was too large to fit in a float
    // std::cerr << "Warning: Value out of range in unit string '" << unit <<
    // "'\n";
    return 0.0f;
  }
}
