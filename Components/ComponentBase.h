//
// Created by Antonin Fruchet on 15/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_COMPONENTBASE_H
#define COMPONENT_BASED_ARCH_COMPONENTBASE_H
#include "Component.h"


template <typename Derived>
class ComponentBase : public Component {
public:
    std::unique_ptr<Component> clone() const override
    {
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
};



#endif //COMPONENT_BASED_ARCH_COMPONENTBASE_H
