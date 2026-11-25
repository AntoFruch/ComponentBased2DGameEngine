//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_COMPONENT_H
#define COMPONENT_BASED_ARCH_COMPONENT_H
#include "SFML/System/Time.hpp"

class GameObject;

class Component {
public:
    explicit Component()=default;
    virtual ~Component()=default;

    GameObject* gameObject;
    /**
     * @brief Called after OnEnable() to start component behaviour
     */
    virtual void Start();

    /**
     * @brief Called each frame
     * @param elapsedTime
     */
    virtual void Update(const sf::Time& elapsedTime);

    /**
     * @brief Changes the parent GameObject to go
     * @param go
     */
    void setParent(GameObject* go);

    // PROTOTYPE
    virtual std::unique_ptr<Component> clone() const = 0;
};



#endif //COMPONENT_BASED_ARCH_COMPONENT_H
