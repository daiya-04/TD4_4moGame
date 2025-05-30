#include "SceneFactory.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "DebugTestScene.h"

#include <unordered_map>
#include <functional>

std::unique_ptr<DaiEngine::IScene> SceneFactory::CreateScene(const std::string& sceneName){

    const std::unordered_map<std::string, std::function<std::unique_ptr<DaiEngine::IScene>()>> sceneTable {
        {"Game",[]() { return std::make_unique<GameScene>(); }},
        {"Title",[]() {return std::make_unique<TitleScene>(); }},
        {"Debug",[]() {return std::make_unique<DebugTestScene>(); }},
        {"Clear",[]() {return std::make_unique<ClearScene>(); }},
        {"GameOver",[]() {return std::make_unique<GameOverScene>(); }},
    };

    auto newScene = sceneTable.find(sceneName);
    if (newScene != sceneTable.end()) {
        return newScene->second();
    }

    return nullptr;
}
