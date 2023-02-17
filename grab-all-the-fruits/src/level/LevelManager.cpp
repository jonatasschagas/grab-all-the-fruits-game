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

    m_pMap = new Map(m_pWorld, m_pCurrentLevelTileMap, m_pEventListener);

    Event levelStartEvent("level-start");
    levelStartEvent.setTarget(level.title);
    m_pEventListener->receiveEvent(&levelStartEvent);

    pPlatformManager->loadMusic(level.backgroundMusic);
    pPlatformManager->playMusic(level.backgroundMusic);
}

void LevelManager::update(float deltaTime)
{
    m_pMap->updateCameraShake(deltaTime);
}

void LevelManager::updateEditor()
{
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
        level.backgroundMusic = levelData["backgroundMusic"].GetString();
        level.backgroundTileSize.x = levelData["backgroundTileSize"]["x"].GetInt();
        level.backgroundTileSize.y = levelData["backgroundTileSize"]["y"].GetInt();
        level.backgroundTextureSize.x = levelData["backgroundTextureSize"]["x"].GetInt();
        level.backgroundTextureSize.y = levelData["backgroundTextureSize"]["y"].GetInt();
        level.numFruits = levelData["numFruits"].GetInt();
        level.character = levelData["character"].GetString();

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
            
            if (trapData.HasMember("targetTileX") && trapData.HasMember("targetTileY"))
            {
                trap.targetTileX = trapData["targetTileX"].GetInt();
                trap.targetTileY = trapData["targetTileY"].GetInt();
            }
            
            trap.widthInTiles = trapData["widthInTiles"].GetFloat();
            trap.heightInTiles = trapData["heightInTiles"].GetFloat();
            trap.type = trapData["type"].GetString();
            
            if (trapData.HasMember("speed"))
            {
                trap.speed = trapData["speed"].GetFloat();
            }
            
            if (trapData.HasMember("idleTime") && trapData.HasMember("crushTime"))
            {
                trap.idleTime = trapData["idleTime"].GetFloat();
                trap.crushTime = trapData["crushTime"].GetFloat();
            }
            
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

        auto fallingBlocks = levelData["fallingBlocks"].GetArray();
        for (int j = 0; j < fallingBlocks.Size(); j++)
        {
            auto fallingBlockData = fallingBlocks[j].GetObject();
            FallingBlock fallingBlock;
            fallingBlock.tileX = fallingBlockData["tileX"].GetInt();
            fallingBlock.tileY = fallingBlockData["tileY"].GetInt();
            fallingBlock.distanceToGroundInTiles = fallingBlockData["distanceToGroundInTiles"].GetFloat();
            fallingBlock.weight = fallingBlockData["weight"].GetFloat();
            level.fallingBlocks.push_back(fallingBlock);
        }

        sm_levels.push_back(level);
    }
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

const FallingBlock* LevelManager::findFallingBlock(int tileX, int tileY) const
{
    for (int i = 0; i < sm_levels[m_currentLevelIndex].fallingBlocks.size(); i++)
    {
        FallingBlock fallingBlock = sm_levels[m_currentLevelIndex].fallingBlocks[i];
        if (fallingBlock.tileX == tileX && fallingBlock.tileY == tileY)
        {
            return &sm_levels[m_currentLevelIndex].fallingBlocks[i];
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
    if (pEvent->getName().compare("fruit-collected") == 0) 
    {
        m_numFruitsCollected++;
        Event event("update-fruit-collected-hud");
        event.setData(m_numFruitsCollected);
        m_pEventListener->receiveEvent(&event);
    }
    else if (pEvent->getName().compare("ground-collide") == 0) 
    {
        float intensity = pEvent->getFloatData();
        m_pMap->shakeCamera(intensity);
    }
}

const string& LevelManager::getCurrentCharacter() const
{
    return sm_levels[m_currentLevelIndex].character;
}

const int LevelManager::getNumberOfLevels() const
{
    return sm_levels.size();
}
