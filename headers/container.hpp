#pragma once
#include "./element.hpp"
#include "./renderer.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class Container : public Element {
public:
  using Ptr = std::shared_ptr<Element>;

  explicit Container(sf::RenderWindow &wind);
  virtual ~Container() = default;
  const std::vector<std::shared_ptr<Element>> &getChildren() const {
    return children;
  }

  // PASS 1: layout + submitting absolute children
  void update(Renderer &renderer) override {
    if (style.absZIndex >= 0) {
      renderer.addToGlobalDrawList(this);
      return;
    }

    arrangeChildren();

    for (auto &ch : children) {
      ch->update(renderer);
    }
  }

  // PASS 2: draw this container and its non-abs children in relZ order
  void draw() override {
    if (!style.visible)
      return;

    drawSelf();

    // Sort children by relZIndex for local stacking
    std::sort(children.begin(), children.end(), [](const Ptr &a, const Ptr &b) {
      return a->style.relZIndex < b->style.relZIndex;
    });

    for (auto &ch : children) {
      if (ch->style.absZIndex >= 0)
        continue;
      ch->draw();
    }
  }
  void addChild(std::shared_ptr<Element> child) {
    children.push_back(child);
    child->setParent(this);
  }

  void removeChild(const std::string &id) {
    children.erase(std::remove_if(children.begin(), children.end(),
                                  [&id](const std::shared_ptr<Element> &elem) {
                                    return elem->style.id == id;
                                  }),
                   children.end());
  }

  void clearChildren() { children.clear(); }

protected:
  std::vector<Ptr> children;
  virtual void drawSelf() = 0;
  virtual void arrangeChildren() = 0;
};
