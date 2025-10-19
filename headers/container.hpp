#pragma once
#include "./element.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class Container : public Element {
public:
  explicit Container(sf::RenderWindow &wind);
  virtual ~Container() = default;
  bool isContainer() const override { return true; }
  const std::vector<std::shared_ptr<Element>> &getChildren() const {
    return children;
  }

  void addChild(std::shared_ptr<Element> child) {
    children.push_back(child);
    child->setParent(this); // Set this container as parent
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
  std::vector<std::shared_ptr<Element>> children;
  void arrangeChildren();
};
