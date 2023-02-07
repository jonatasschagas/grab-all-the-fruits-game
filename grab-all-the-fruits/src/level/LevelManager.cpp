#include "LevelManager.hpp"

#include <rapidjson/document.h>
#include "utils/FileUtils.h"
#include "platform/PlatformManager.h"
#include "data/TileMapData.hpp"
#include "core/Vector2.h"

using namespace rapidjson;

LevelManager::LevelManager(const string& levelsFile, Sprite* pStage, World* pWorld, AnimatedObjectsFactory* pAnimatedObjectsFactory)
{
    initializeMembers();

    m_pStage = pStage;
    m_pWorld = pWorld;
    m_pAnimatedObjectsFactory = pAnimatedObjectsFactory;

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

    LevelData level = m_levels[levelindex];

    string levelConfigFile = m_levelsFolder + level.configFile;
    string levelBackgroundFile = m_levelsFolder + level.background;
    string levelTilesetFile = m_tilesetsFolder + level.configFile;

    m_pCurrentLevelBackground = new LevelBackground(level.backgroundTileSize, m_pStage->getPlatformManager(), level.background, level.backgroundTextureSize);
    m_pCurrentLevelBackground->setXY(0, 0);
    m_pStage->addChild(m_pCurrentLevelBackground);
    m_pCurrentLevelBackground->fillParent();

    TileMapData* pTileMapData = new TileMapData(level.configFile, m_levelsFolder, m_tilesetsFolder);
    Vector2 mapSize(pTileMapData->getWidth() * m_tileSize.x, pTileMapData->getHeight() * m_tileSize.y);
    m_pWorld->initWorld(mapSize);

    m_pCurrentLevelTileMap = new TileMapSprite(m_tileSize, m_pStage->getPlatformManager(), m_pAnimatedObjectsFactory);
    m_pCurrentLevelTileMap->loadMap(pTileMapData, "meta");
    m_pCurrentLevelTileMap->setXY(0, 0);
    m_pCurrentLevelTileMap->setPivotAtCenter(true);
    m_pStage->addChild(m_pCurrentLevelTileMap);
    m_pCurrentLevelTileMap->fillParent();

    m_pMap = new Map(m_pWorld, m_pCurrentLevelTileMap);
}

void LevelManager::updateEditor()
{

}

void LevelManager::loadLevelsData(const string& levelsFile)
{
    string configurationJSON = loadFile(levelsFile);
    
    Document configurationDocument;
    configurationDocument.Parse(configurationJSON.c_str());
    
    m_tilesetsFolder = configurationDocument["tileSetsFolder"].GetString();
    m_levelsFolder = configurationDocument["levelsFolder"].GetString();
    m_tileSize.x = configurationDocument["tileSize"]["x"].GetInt();
    m_tileSize.y = configurationDocument["tileSize"]["y"].GetInt();

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
        m_levels.push_back(level);
    }
}

void LevelManager::addSpriteToTileMap(Sprite* pSprite)
{
    m_pCurrentLevelTileMap->addChild(pSprite);
}

const Vector2 LevelManager::getTileMapSize() const
{
    Vector2 tileMapSize;
    tileMapSize.x = m_pCurrentLevelTileMap->getWidth() * m_tileSize.x;
    tileMapSize.y = m_pCurrentLevelTileMap->getHeight() * m_tileSize.y;
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
