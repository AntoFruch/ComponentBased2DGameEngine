//
// Created by Antonin Fruchet on 26/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_INPUTMANAGER_H
#define COMPONENT_BASED_ARCH_INPUTMANAGER_H
#include <filesystem>
#include <map>
#include <unordered_map>

#include "InputAction.h"

class InputManager {
    const std::filesystem::path input_config_file_path;

    std::unordered_map<std::string, std::unique_ptr<InputAction>> actions{};

    /**
     * Currently pressed keys
     */
    std::set<sf::Keyboard::Key> pressed_keys{};

    void loadActions();
public:

    explicit InputManager(std::string_view configFilePath);
    InputAction* findAction(const std::string& action_name);

    void beginFrame();
    void processEvents(std::optional<sf::Event> event);
};



#endif //COMPONENT_BASED_ARCH_INPUTMANAGER_H
