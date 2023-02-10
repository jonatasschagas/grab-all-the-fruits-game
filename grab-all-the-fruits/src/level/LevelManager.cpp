#include "LevelManager.hpp"

#include <rapidjson/document.h>
#include "utils/FileUtils.h"
#include "platform/PlatformManager.h"
#include "data/TileMapData.hpp"
#include "core/Vector2.h"
#include "imgui/imgui.h"

vector<LevelData> LevelManager::sm_levels;
Vector2 LevelManager::sm_tileSize = Vector2::ZERO;
string LevelManager::sm_levelsFolder;
string LevelManager::sm_tilesetsFolder;

using namespace rapidjson;

LevelManager::LevelManager(
    const string& levelsFile, 
    Sprite* pStage, 
    World* pWorld, 
    AnimatedObjectsFactory* pAnimatedObjectsFactory, 
    EventListener* pEventListener) : EventListener()
{
    initializeMembers();

    m_pStage = pStage;
    m_pWorld = pWorld;
    m_pAnimatedObjectsFactory = pAnimatedObjectsFactory;
    m_pEventListener = pEventListener;

    loadLevelsData(levelsFile);
}

LevelManager::~LevelManager()
{
    delete m_pMap;
    // remove the tilemap first
    m_pStage->removeChildForced(m_pCurrentLevelTileMap);
    
    initializeMembers();
}
    
void LevelManager::loadLevel(const int levelindex)
{
    delete m_pMap;
    delete m_pCurrentLevelTileMap;
    delete m_pCurrentLevelBackground;

    PlatformManager* pPlatformManager = m_pStage->getPlatformManager();
    
    m_currentLevelIndex = levelindex;
    LevelData level = sm_levels[levelindex];

    string levelConfigFile = sm_levelsFolder + level.configFile;
    string levelBackgroundFile = sm_levelsFolder + level.background;
    string levelTilesetFile = sm_tilesetsFolder + level.configFile;

    m_pCurrentLevelBackground = new LevelBackground(level.backgroundTileSize, m_pStage->getPlatformManager(), level.background, level.backgroundTextureSize);
    m_pCurrentLevelBackground->setXY(0, 0);
    m_pStage->addChild(m_pCurrentLevelBackground);
    m_pCurrentLevelBackground->fillParent();

    TileMapData* pTileMapData = new TileMapData(level.configFile, sm_levelsFolder, sm_tilesetsFolder);
    Vector2 mapSize(pTileMapData->getWidth() * sm_tileSize.x, pTileMapData->getHeight() * sm_tileSize.y);
    m_pWorld->initWorld(mapSize);

    m_pCurrentLevelTileMap = new TileMapSprite(sm_tileSize, m_pStage->getPlatformManager(), m_pAnimatedObjectsFactory);
    m_pCurrentLevelTileMap->loadMap(pTileMapData, "meta");
    m_pCurrentLevelTileMap->setXY(0, 0);
    m_pCurrentLevelTileMap->setPivotAtCenter(true);
    m_pStage->addChild(m_pCurrentLevelTileMap);
    m_pCurrentLevelTileMap->fillParent();

    m_pMap = new Map(m_pWorld, m_pCurrentLevelTileMap);

    Event levelStartEvent("level-start");
    levelStartEvent.setTarget(level.title);
    m_pEventListener->receiveEvent(&levelStartEvent);
}

void LevelManager::updateEditor()
{
    if (!m_initialized)
    {
        return;
    }

    ImGui::Begin("Levels", nullptr, 
    ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize);

    for (int i = 0; i < sm_levels.size(); i++)
    {
        if (ImGui::Button(sm_levels[i].title.c_str()))
        {
            Event eventLoadLevel("load-level");
            eventLoadLevel.setData(i);
            m_pEventListener->receiveEvent(&eventLoadLevel);
        }
    }
    
    ImGui::End();
}

void LevelManager::loadLevelsData(const string& levelsFile)
{
    if (sm_levels.size() > 0)
    {
        return;
    }

    string configurationJSON = loadFile(levelsFile);
    
    Document configurationDocument;
    configurationDocument.Parse(configurationJSON.c_str());
    
    sm_tilesetsFolder = configurationDocument["tileSetsFolder"].GetString();
    sm_levelsFolder = configurationDocument["levelsFolder"].GetString();
    sm_tileSize.x = configurationDocument["tileSize"]["x"].GetInt();
    sm_tileSize.y = configurationDocument["tileSize"]["y"].GetInt();

    auto levelsData = configurationDocument["levels"].GetArray();
    for (int i = 0; i < levelsData.Size(); i++)
    {
        auto levelData = levelsData[i].GetObject();
        LevelData level;
        level.title = levelData["title"].GetString();
        level.configFile = levelData["configFile"].GetString();
        level.background = levelData["background"].GetString();
        level.backgroundTileSize.x = levelData["backgroundTileSize"]["x"].GetInt();
        level.backgroundTileSize.y = levelData["backgroundTileSize"]["y"].GetInt();
        level.backgroundTextureSize.x = levelData["backgroundTextureSize"]["x"].GetInt();
        level.backgroundTextureSize.y = levelData["backgroundTextureSize"]["y"].GetInt();
        level.numFruits = levelData["numFruits"].GetInt();

        auto platforms = levelData["platforms"].GetArray();
        for (int j = 0; j < platforms.Size(); j++)
        {
            auto platformData = platforms[j].GetObject();
            Platform platform;
            platform.tileX = platformData["tileX"].GetInt();
            platform.tileY = platformData["tileY"].GetInt();
            platform.targetTileX = platformData["targetTileX"].GetInt();
            platform.targetTileY = platformData["targetTileY"].GetInt();
            platform.widthInTiles = platformData["widthInTiles"].GetFloat();
            platform.heightInTiles = platformData["heightInTiles"].GetFloat();
            platform.type = platformData["type"].GetString();
            platform.speed = platformData["speed"].GetFloat();
            level.platforms.push_back(platform);
        }

        auto traps = levelData["traps"].GetArray();
        for (int j = 0; j < traps.Size(); j++)
        {
            auto trapData = traps[j].GetObject();
            Trap trap;
            trap.tileX = trapData["tileX"].GetInt();
            trap.tileY = trapData["tileY"].GetInt();
            trap.targetTileX = trapData["targetTileX"].GetInt();
            trap.targetTileY = trapData["targetTileY"].GetInt();
            trap.widthInTiles = trapData["widthInTiles"].GetFloat();
            trap.heightInTiles = trapData["heightInTiles"].GetFloat();
            trap.type = trapData["type"].GetString();
            level.traps.push_back(trap);
        }

        auto trampolines = levelData["trampolines"].GetArray();
        for (int j = 0; j < trampolines.Size(); j++)
        {
            auto trampolineData = trampolines[j].GetObject();
            Trampoline trampoline;
            trampoline.tileX = trampolineData["tileX"].GetInt();
            trampoline.tileY = trampolineData["tileY"].GetInt();
            trampoline.bounciness = trampolineData["bounciness"].GetInt();
            level.trampolines.push_back(trampoline);
        }

        sm_levels.push_back(level);
    }

    m_initialized = true;
}

const Platform* LevelManager::findPlatform(int tileX, int tileY) const
{
    for (int i = 0; i < sm_levels[m_currentLevelIndex].platforms.size(); i++)
    {
        Platform platform = sm_levels[m_currentLevelIndex].platforms[i];
        if (platform.tileX == tileX && platform.tileY == tileY)
        {
            return &sm_levels[m_currentLevelIndex].platforms[i];
        }
    }
    return nullptr;
}

const Trap* LevelManager::findTrap(int tileX, int tileY) const
{
    for (int i = 0; i < sm_levels[m_currentLevelIndex].traps.size(); i++)
    {
        Trap trap = sm_levels[m_currentLevelIndex].traps[i];
        if (trap.tileX == tileX && trap.tileY == tileY)
        {
            return &sm_levels[m_currentLevelIndex].traps[i];
        }
    }
    return nullptr;
}

const Trampoline* LevelManager::findTrampoline(int tileX, int tileY) const
{
    for (int i = 0; i < sm_levels[m_currentLevelIndex].trampolines.size(); i++)
    {
        Trampoline trampoline = sm_levels[m_currentLevelIndex].trampolines[i];
        if (trampoline.tileX == tileX && trampoline.tileY == tileY)
        {
            return &sm_levels[m_currentLevelIndex].trampolines[i];
        }
    }
    return nullptr;
}

void LevelManager::addSpriteToTileMap(Sprite* pSprite)
{
    m_pCurrentLevelTileMap->addChild(pSprite);
}

const Vector2 LevelManager::getTileMapSize() const
{
    Vector2 tileMapSize;
    tileMapSize.x = m_pCurrentLevelTileMap->getWidth() * sm_tileSize.x;
    tileMapSize.y = m_pCurrentLevelTileMap->getHeight() * sm_tileSize.y;
    return tileMapSize;
}

Vector2 LevelManager::getTileMapPosition() const
{
    return m_pCurrentLevelTileMap->getGamePosition();
}

void LevelManager::updateCameraPosition(const Vector2& rCameraPosition)
{
    m_pMap->updateCameraPosition(rCameraPosition);
}

const bool LevelManager::hasCompletedLevel() const
{
    return m_numFruitsCollected == sm_levels[m_currentLevelIndex].numFruits;
}

void LevelManager::receiveEvent(Event* pEvent)
{
    if (pEvent->getName().compare("fruit-collected") == 0) {
        m_numFruitsCollected++;
        Event event("update-fruit-collected-hud");
        event.setData(m_numFruitsCollected);
        m_pEventListener->receiveEvent(&event);
    }
}