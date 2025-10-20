#include "../headers/renderer.hpp"
#include "../headers/container.hpp"
#include <algorithm> // for std::sort

Renderer::Renderer() : root(nullptr) {}

void Renderer::addToGlobalDrawList(Element *element) {
  globalDrawList.push_back(element);
}

void Renderer::setRoot(Container *rootComponent) {
  root = rootComponent;
  globalDrawList.push_back(root); // now compiler knows Container -> Element
}

void Renderer::flush() {
  // No elements to draw → exit early
  if (globalDrawList.empty())
    return;

  // Sort elements by their absolute z-index (ascending)
  std::sort(globalDrawList.begin(), globalDrawList.end(),
            [](Element *a, Element *b) {
              return a->style.absZIndex < b->style.absZIndex;
            });

  // Draw each element in sorted order
  for (Element *el : globalDrawList) {
    if (el && el->style.visible)
      el->draw();
  }

  // Clear the draw list for the next frame
  globalDrawList.clear();
}
