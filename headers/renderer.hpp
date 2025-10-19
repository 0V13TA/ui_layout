#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Container;
class Element;

class Renderer {
public:
  explicit Renderer();
  virtual ~Renderer() = default;

  /**
   * @brief Flush all elements in the global draw list to the screen.
   *
   * This method sorts the elements by their absolute z-index (absZIndex)
   * and draws them to the SFML render window in the correct stacking order.
   *
   * @note Call this once per frame after updating all element states.
   */
  void flush();

  void addToGlobalDrawList(Element *element);

  void setRoot(Container *rootComponent);

private:
  Container *root;
  std::vector<Element *> globalDrawList;
};
