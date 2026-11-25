//
// Created by Antonin Fruchet on 25/11/2026.
//

#include "XMLParser.h"

#include "Managers/Scene/ComponentFactory.h"
#include "Managers/Scene/PrefabManager.h"
#include "../Managers/Input/StringToKeyMap.h"

// Scene
std::vector<std::unique_ptr<GameObject>> parseGameObject(const pugi::xml_node& rootNode)
{
    std::vector<std::unique_ptr<GameObject>> objects;

    // Structure pour stocker l'élément XML et son parent dans la pile
    struct StackElement {
        pugi::xml_node node;
        GameObject* parent; // nullptr si c'est un objet racine
    };

    std::stack<StackElement> stack;
    stack.push({rootNode, nullptr});

    while (!stack.empty())
    {
        auto [currentNode, parentPtr] = stack.top();
        stack.pop();

        auto ptr = std::make_unique<GameObject>(
            currentNode.attribute("label").as_string(),
            sf::Vector2f{
                currentNode.attribute("x").as_float(),
                currentNode.attribute("y").as_float()
            },
            sf::degrees(currentNode.attribute("angle").as_float()),
            sf::Vector2f{
                currentNode.attribute("sx") ? currentNode.attribute("sx").as_float() : 1.f,
                currentNode.attribute("sy") ? currentNode.attribute("sy").as_float() : 1.f,
            },
            currentNode.attribute("active") ? currentNode.attribute("active").as_bool() : true
        );

        auto components = currentNode.child("Components");
        for (const auto& component : components.children())
        {
            std::string componentNodeName = component.name();
            if (componentNodeName != "Component")
            {
                throw IllegalOperationException("Components node children must be named \"Component\"");
            }
            const std::string componentName = component.attribute("name").as_string();
            if (componentName.empty())
            {
                throw IllegalOperationException("Component node must have a non-empty \"name\" attribute");
            }
            ptr->addComponent(ComponentFactory::Create(componentName, component));
        }

        if (parentPtr != nullptr)
        {
            parentPtr->transform.addChild(&ptr->transform);
        }

        std::vector<pugi::xml_node> childList;
        for (auto child : currentNode.children("GameObject"))
        {
            stack.push({child, ptr.get()});
        }

        objects.push_back(std::move(ptr));
    }

    return objects;
}
std::unique_ptr<Scene> XMLParser::parseScene(const std::string& filePath, PrefabManager& manager)
{
    std::filesystem::path path = filePath;
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result)
    {
        throw IllegalOperationException(std::format("Could not open scene because {}", result.description()));
    }
    pugi::xml_node sceneObj = doc.child("Scene");
    if (!sceneObj)
    {
        throw IllegalOperationException("Root node of scene must be named \"Scene\"");
    }

    std::vector<std::unique_ptr<GameObject>> gameObjects;

    for (const auto& obj : sceneObj)
    {
        std::string name = obj.name();
        if (name == "GameObject")
        {
            for (auto& p : parseGameObject(obj))
            {
                gameObjects.push_back(std::move(p));
            }
        } else if (name == "Prefab")
        {
            auto source = obj.attribute("src").as_string();
            auto prefab = manager.getOrParse(source);

            for (auto& p : prefab)
            {
                gameObjects.push_back(std::move(p));
            }
        }
    }

    return std::make_unique<Scene>(gameObjects);
}
std::vector<std::unique_ptr<GameObject>> XMLParser::parsePrefab(std::string_view filePath)
{
    std::filesystem::path path = filePath;
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result)
    {
        throw IllegalOperationException(std::format("Could not parse prefab {} because {}",filePath, result.description()));
    }
    auto obj = doc.child("GameObject");
    auto objects = parseGameObject(obj);

    if (obj.attribute("label"))
    {
        objects[0]->setLabel(obj.attribute("label").as_string());
    }
    if (obj.attribute("x") || obj.attribute("y"))
    {
        objects[0]->transform.move({obj.attribute("x").as_float(), obj.attribute("y").as_float()});
    }
    if (obj.attribute("angle"))
    {
        objects[0]->transform.rotate(sf::degrees(obj.attribute("angle").as_float()));
    }
    if (obj.attribute("sx") || obj.attribute("sy"))
    {
        objects[0]->transform.rescale({obj.attribute("sx").as_float(1.f), obj.attribute("sy").as_float(1.f)});
    }
    if (obj.attribute("active"))
    {
        objects[0]->setActive(obj.attribute("active").as_bool());
    }

    return objects;
}

// Input
std::vector<DirectionalBindings> create_directional_bindings(pugi::xml_node node)
{
    std::vector<DirectionalBindings> vector;
    for (const auto& binding : node.children())
    {
        DirectionalBindings b{
            stringToKey.at(binding.child("up").attribute("bind").as_string()),
            stringToKey.at(binding.child("down").attribute("bind").as_string()),
            stringToKey.at(binding.child("left").attribute("bind").as_string()),
            stringToKey.at(binding.child("right").attribute("bind").as_string())
        };
        vector.push_back(b);
    }
    return vector;
}
std::vector<ButtonBinding> create_button_bindings(pugi::xml_node node)
{
    std::vector<ButtonBinding> vector;
    for (const auto& binding : node.children())
    {
        ButtonBinding b{
            stringToKey.at(binding.attribute("bind").as_string()),
        };
        vector.push_back(b);
    }
    return vector;
}
std::unordered_map<std::string, std::unique_ptr<InputAction>> XMLParser::parseInputAction(std::string_view filePath)
{
    pugi::xml_document doc;
    if (auto result = doc.load_file(filePath.data()); !result)
    {
        throw IllegalOperationException(std::format("could not open file {} because : {}\n", filePath, result.description()));
    }
    std::unordered_map<std::string, std::unique_ptr<InputAction>> actions{};
    for (const auto& node : doc.child("InputActions").children())
    {
        if (std::string name = node.name(); name != "Action") continue;

        std::string label{node.attribute("label").as_string()};
        std::string type{node.attribute("type").as_string()};
        if (type == "Vector2D") {
            actions.try_emplace(label, std::make_unique<InputAction>(label, create_directional_bindings(node)));
            continue;
        }
        if (type == "Button") {
            actions.try_emplace(label, std::make_unique<InputAction>(label, create_button_bindings(node)));
            continue;
        }
        throw IllegalOperationException("Other input types than Vector2D or Button are not supported");
    }
    return actions;
}

// Animation Graph
std::variant<bool, float> parseConditionValue(const pugi::xml_attribute& value)
{
    const std::string valueText = value.as_string();
    if (valueText == "true") return true;
    if (valueText == "false") return false;
    return value.as_float();
}
void loadConditions(const pugi::xml_node& cond, std::vector<Condition>& conditions)
{
    std::string name = cond.name();
    if (name != "Condition")
    {
        throw IllegalOperationException("Transition node children must be named \"Condition\"");
    }
    std::string cmp = cond.attribute("cmp").as_string();
    if (cmp == "Less")
    {
        conditions.emplace_back(cond.attribute("parameter").as_string(),
                                parseConditionValue(cond.attribute("value")), Less);
    }
    else if (cmp == "Greater")
    {
        conditions.emplace_back(cond.attribute("parameter").as_string(),
                                parseConditionValue(cond.attribute("value")), Greater);
    }
    else // default is Equals
    {
        conditions.emplace_back(cond.attribute("parameter").as_string(),
                                parseConditionValue(cond.attribute("value")), Equals);
    }
}
Transition createTransition(const pugi::xml_node& transi)
{
    std::vector<Condition> conditions;
    for (const auto& cond : transi.children())
    {
        loadConditions(cond, conditions);
    }
    return {
        transi.attribute("to").as_string(),
        conditions
    };
}
AnimationState createAnimationState(const pugi::xml_node& state)
{
    std::vector<Transition> transitions;
    for (const auto& transi : state.children())
    {
        std::string name = transi.name();
        if (name == "Transition")
        {
            transitions.push_back(createTransition(transi));
        }
    }
    return {
        state.attribute("label").as_string(),
        state.attribute("startx").as_uint(),
        state.attribute("starty").as_int(),
        state.attribute("length").as_uint(),
        state.attribute("frameDuration").as_uint(),
        state.attribute("loop").as_bool(),
        state.attribute("breakable").as_bool(),
        transitions
    };
}
std::unordered_map<std::string, Parameter> createParametersMap(const pugi::xml_node& params)
{
    std::unordered_map<std::string, Parameter> parameters;
    for (const auto& param : params.children())
    {
        std::string type = param.attribute("type").as_string();
        if (type == "Trigger")
        {
            parameters.try_emplace(param.attribute("label").as_string(),
                                   Parameter{Trigger, param.attribute("default").as_bool()});
        }
        else if (type == "Bool")
        {
            parameters.try_emplace(param.attribute("label").as_string(),
                                   Parameter{Bool, param.attribute("default").as_bool()});
        }
        else if (type == "Float")
        {
            parameters.try_emplace(param.attribute("label").as_string(),
                                   Parameter{Float, param.attribute("default").as_float()});
        }
    }
    return parameters;
}
std::string retrieveEntryState(const pugi::xml_node& root_node)
{
    pugi::xml_node entry_node = root_node.child("EntryState");
    if (!entry_node) throw IllegalOperationException("An AnimationTree must have a \"EntryState\"");
    std::string label = entry_node.attribute("label").as_string();
    if (label.empty()) throw IllegalOperationException("EntryState \"label\" attribute must be unempty");
    return label;
}
BlendTree handleBlendTreeNode(const pugi::xml_node& state,
    std::unordered_map<std::string, AnimationState>& states)
{
    const std::string blendTreeLabel = state.attribute("label").as_string();
    if (blendTreeLabel.empty()) throw IllegalOperationException("BlendTree node must have a \"label\" attribute");

    const std::string path = state.attribute("src").as_string();
    if (path.empty()) throw IllegalOperationException("BlendTree node must have a \"src\" attribute");

    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result)
    {
        std::cerr << "Could not open AnimatorTree file because " << result.description() << std::endl;
        throw IllegalOperationException("Could not open AnimatorTree file");
    }
    auto tree_node = doc.child("BlendTree");
    if (!tree_node)
    {
        throw IllegalOperationException("Root node of BlendTree must be named \"BlendTree\"");
    }

    std::vector<Transition> externalTransitions;
    for (const auto& transi : state.children("Transition"))
    {
        externalTransitions.push_back(createTransition(transi));
    }

    std::vector<std::string> animationNames;
    std::vector<Transition> blendTreeTransitions;
    for (const auto& branch : tree_node.children())
    {
        if (const std::string name  = branch.name(); name!="Animation") continue;

        const std::string branchLabel = branch.attribute("label").as_string();
        animationNames.emplace_back(branchLabel);

        std::vector<Condition> conditions;
        for (const auto& cond : branch.children("Condition"))
        {
            loadConditions(cond, conditions);
        }
        blendTreeTransitions.emplace_back(branchLabel, conditions);
    }

    for (const auto& branch : tree_node.children())
    {
        if (const std::string name  = branch.name(); name!="Animation") continue;

        std::vector<Transition> transitions;
        for (const Transition& transition : externalTransitions)
        {
            transitions.push_back(transition);
        }

        for (const auto& branch2 : tree_node.children())
        {
            if (const std::string name = branch2.name(); name != "Animation") continue;

            const std::string label = branch.attribute("label").as_string();
            const std::string label2 = branch2.attribute("label").as_string();
            if (label2 == label) continue;

            for (const Transition& blendTreeTransition : blendTreeTransitions)
            {
                if (blendTreeTransition.target == label2)
                {
                    transitions.push_back(blendTreeTransition);
                    break;
                }
            }
        }

        states.try_emplace(
            branch.attribute("label").as_string(),
            branch.attribute("label").as_string(),
            branch.attribute("startx").as_uint(),
            branch.attribute("starty").as_int(),
            branch.attribute("length").as_uint(),
            branch.attribute("frameDuration").as_uint(),
            branch.attribute("loop").as_bool(),
            branch.attribute("breakable").as_bool(),
            transitions
        );
    }

    return {blendTreeLabel, animationNames, blendTreeTransitions};
}
std::unordered_map<std::string, AnimationState> createAnimationsMap(
    const pugi::xml_node& root_node,
    std::unordered_map<std::string, BlendTree>& blendTrees)
{
    auto tree_node = root_node.child("Tree");
    if (!tree_node) throw IllegalOperationException("AnimationTree must have a node named \"Tree\"");
    std::unordered_map<std::string, AnimationState> states;
    for (const auto& state : tree_node)
    {
        const std::string state_name = state.name();
        if (state_name == "Animation")
        {
            states.try_emplace(state.attribute("label").as_string(), createAnimationState(state));
        } else if (state_name == "BlendTree")
        {
            BlendTree blendTree = handleBlendTreeNode(state, states);
            blendTrees.try_emplace(blendTree.name, blendTree);
        } else
        {
            throw IllegalOperationException(R"(Each child node of "Tree" node must be named "Animation" or "BlendTree")");
        }
    }
    return states;
}
std::unique_ptr<AnimatorGraph> XMLParser::parseAnimationGraph(std::string_view path)
{
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.data()); !result)
    {
        std::cerr << "Could not open AnimatorTree file because " << result.description() << std::endl;
        throw IllegalOperationException("Could not open AnimatorTree file");
    }
    auto root_node = doc.child("AnimationTree");
    if (!root_node)
    {
        throw IllegalOperationException("Root node of AnimationTree must be named \"AnimationTree\"");
    }
    if (const std::string root_node_label = root_node.attribute("label").as_string(); root_node_label.empty())
    {
        throw IllegalOperationException("AnimationTree \"label\" attribute must not be empty");
    }

    std::unordered_map<std::string, BlendTree> blendTrees;
    auto ptr = std::make_unique<AnimatorGraph>(
        retrieveEntryState(root_node),
        createAnimationsMap(root_node, blendTrees),
        blendTrees,
        createParametersMap(root_node.child("Parameters"))
    );
    return std::move(ptr);
}
