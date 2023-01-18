#include "MapManager.hpp"
#include "utils/PerlinNoise.h"
#include "utils/MathUtils.h"

#include "data/LevelData.hpp"
#include "data/TileMapData.hpp"

#include <algorithm>

#ifdef __APPLE__
    #include <TargetConditionals.h>
#endif

MapManager::MapManager(const int tileSizeInWorldUnits,
                       const int maxScreenShakeOffsetY,
                       const float screenShakeDuration,
                       const vector<int>& slidableTileTypes,
                       PlatformManager* pPlatformManager) : Sprite(pPlatformManager)
{
    initializeMembers();
    
    m_pPlatformManager = pPlatformManager;
    m_tileSizeInWorldUnits = tileSizeInWorldUnits;
    m_maxScreenShakeOffsetY = maxScreenShakeOffsetY;
    m_screenShakeDuration = screenShakeDuration;
    m_slidableTileTypes = slidableTileTypes;
    m_worldSize = pPlatformManager->getWorldSizeUnits();
    
    setPivotAtCenter(true);
}

MapManager::~MapManager()
{
    unloadMap();
    
    initializeMembers();
}

void MapManager::unloadMap()
{
    for (int i = 0; i < m_metaTileItems.size(); i++)
    {
        MetaTileItem* pMetaTile = m_metaTileItems[i];
        delete pMetaTile;
    }
    m_metaTileItems.clear();
}

void MapManager::loadMap(LevelData* pLevelData)
{
    if (m_pCurrentLevelData != nullptr)
        unloadMap();

    m_pCurrentLevelData = pLevelData;
    const TileMapData& tileMapData = pLevelData->getTileMapData();
    m_tileMapSize = GameSize(tileMapData.getWidth(), tileMapData.getHeight());
    
    // creates the map as a huge sprite
    setXY(0,0);
    setSize(m_tileMapSize.w * m_tileSizeInWorldUnits,
            m_tileMapSize.h * m_tileSizeInWorldUnits);
    setPivotAtCenter(true);

    // reads the ground and the meta layer
    const vector<TileMapLayer*>& layersArray = tileMapData.getLayers();
    for (int i = 0; i < layersArray.size(); i++)
    {
        TileMapLayer* pLayer = layersArray[i];
        string layerName = pLayer->getName();
        string groundLayerName = pLevelData->getGroundLayer();
        string metaLayerName = pLevelData->getMetaLayer();

        if (layerName.compare(groundLayerName) == 0)
        {
            m_pGroundLayer = pLayer;
        }
        else if (layerName.compare(metaLayerName) == 0)
        {
            m_pMetaLayer = pLayer;
        }
    }

    loadMetaLayerConfiguration();

    loadMapLayers();

    loadMapMiscElements();
}

void MapManager::loadMetaLayerConfiguration()
{
    assert(m_pCurrentLevelData != nullptr);
    assert(m_pMetaLayer != nullptr);

    const TileMapData& tileMapData = m_pCurrentLevelData->getTileMapData();
    vector<TileSet*> tileSets = tileMapData.getTileSets();

    // find the meta tileset
    vector<int> layerDataArray = m_pMetaLayer->getData();
    TileSet* pMetaTileSet = NULL;
    for (int i = 0; i < tileSets.size(); i++)
    {
        TileSet* pTileSet = tileSets[i];
        TileSetConfig* pTileSetConfig = pTileSet->getConfig();
        string tileSetName = pTileSetConfig->getName();
        if (tileSetName.compare(m_pMetaLayer->getName()) == 0)
        {
            pMetaTileSet = pTileSet;
            break;
        }
    }

    // reads where the enemies should be placed
    // in the map and where are the entry points to the map
    for (int y = 0; y < m_pMetaLayer->getHeight(); y++)
    {
        for (int x = 0; x < m_pMetaLayer->getWidth(); x++)
        {
            int tileMapIndex = y * m_tileMapSize.w + x;
            if (tileMapIndex >= layerDataArray.size())
            {
                continue;
            }

            int tileData = layerDataArray[tileMapIndex];
            if (tileData != 0)
            {
                int invertedTileY = invertTileY(y);
                int tileIndex = tileData - pMetaTileSet->getFirstGid();
                const TileConfig* pTileConfig = pMetaTileSet->getConfig()->getTileConfig(tileIndex);
                string tileType = pTileConfig->getTileType();
                string tileName = pTileConfig->getTileName();

                // start & exit entrypoints
                if (tileType.compare("map-property") == 0)
                {
                    if (tileName.compare("start") == 0)
                    {
                        m_levelStart.x = x;
                        m_levelStart.y = invertedTileY;
                    }
                    else
                    {
                        m_levelExit.x = x;
                        m_levelExit.y = invertedTileY;
                    }
                }
                else
                {
                    MetaTileItem* pMetaTileConfiguration = new MetaTileItem();
                    pMetaTileConfiguration->name = tileName;
                    pMetaTileConfiguration->type = tileType;
                    pMetaTileConfiguration->m_x = x * m_tileSizeInWorldUnits;
                    pMetaTileConfiguration->m_y = invertedTileY * m_tileSizeInWorldUnits;

                    m_metaTileItems.push_back(pMetaTileConfiguration);
                }
            }
        }
    }
}

void MapManager::loadMapLayers()
{
    assert(m_pCurrentLevelData != nullptr);

    const TileMapData& tileMapData = m_pCurrentLevelData->getTileMapData();
    const vector<TileMapLayer*>& layersArray = tileMapData.getLayers();
    for (int i = 0; i < layersArray.size(); i++)
    {
        TileMapLayer* pLayer = layersArray[i];
        if (pLayer->isVisible() && pLayer != m_pMetaLayer)
        {
            Sprite* pSpriteLayer = new Sprite(m_pPlatformManager);
            pSpriteLayer->setXY(0, 0);
            pSpriteLayer->setSize(pLayer->getWidth() * m_tileSizeInWorldUnits,
                                  pLayer->getHeight() * m_tileSizeInWorldUnits);
            pSpriteLayer->setPivotAtCenter(true);
            pSpriteLayer->setTileMap(true);
            addChild(pSpriteLayer);
            
            for (int x = 0; x < pLayer->getWidth(); x++)
            {
                for (int y = 0; y < pLayer->getHeight(); y++)
                {
                    createTile(x, y, pSpriteLayer, pLayer);
                }
            }
        }
    }
}

void MapManager::createTile(int x, int y, Sprite* pSpriteLayer, TileMapLayer* pLayer)
{
    // tiles from Tiled software are placed from the top to the bottom (screen starts from top left)
    // however in this "engine" we place the tiles from the bottom to the top (screen starts from bottom left)
    int yNormal = y;
    y = invertTileY(y);

    // reading texture coordinates
    const TileMapData& tileMapData = m_pCurrentLevelData->getTileMapData();
    int tileMapIndex = y * tileMapData.getWidth() + x;
    const vector<int>& tileDataArray = pLayer->getData();
    
    if (tileDataArray.size() <= tileMapIndex)
    {
        // invalid tile coordinates
        return;
    }
    
    int tileType = tileDataArray[tileMapIndex];
    
    TileSet* pTileSet = tileMapData.getTileSetFromTileData(tileType);
    if (pTileSet == NULL)
    {
        // no tile set for this tileType
        return;
    }
    
    TileSetConfig* pTileSetConfig = pTileSet->getConfig();

    int tileNumberInTileSet = tileType - pTileSet->getFirstGid();
    
    const TileConfig* pTileConfig = pTileSetConfig->getTileConfig(tileNumberInTileSet);

    // finally, creating the sprite
    int width = m_tileSizeInWorldUnits;
    int height = m_tileSizeInWorldUnits;

    Sprite* pTileSprite = new Sprite(m_pPlatformManager);
    pTileSprite->setXY(x * width, yNormal * height);
    pTileSprite->setSize(width, height);
    pTileSprite->setPivotAtCenter(true);
    pTileSprite->setTextureCoordinates(pTileConfig->getX(),
                                       pTileConfig->getY(),
                                       pTileConfig->getWidth(),
                                       pTileConfig->getHeight());
    pTileSprite->setTileMap(true);
    
    pTileSprite->loadTexture(pTileConfig->getImageName());
    pSpriteLayer->addChild(pTileSprite);
}

void MapManager::loadMapMiscElements()
{}

void MapManager::update(float deltaTime)
{
    updateMapMiscElements(deltaTime);
    
    updateCameraShake(deltaTime);
    
    Sprite::update(deltaTime);
}

void MapManager::updateCamera(float centerCameraX, float centerCameraY)
{
    m_cameraCenterX = centerCameraX;
    m_cameraCenterY = centerCameraY;
    
    const int halfScreenHorizontal = m_worldSize.w/2;
    const int halfScreenVertical = m_worldSize.h/2;
    float levelMaxX = getWorldLimitX() - halfScreenHorizontal;
    
    if (m_cameraCenterX < halfScreenHorizontal)
    {
        m_xOffSet = 0;
    }
    else if (m_cameraCenterX > levelMaxX)
    {
        m_xOffSet = levelMaxX - halfScreenHorizontal;
    }
    else
    {
        m_xOffSet = m_cameraCenterX - halfScreenHorizontal;
    }
    
    if (m_cameraCenterY < halfScreenVertical)
    {
        m_yOffSet = 0;
    }
    else
    {
        m_yOffSet = m_cameraCenterY - halfScreenVertical;
    }
    
    float x = getTransformedX();
    float y = getTransformedY();
    float targetX = -m_xOffSet + m_cameraShakeOffsetX;
    float targetY = -m_yOffSet + m_cameraShakeOffsetY;
    
    if (m_cameraShaking)
    {
        x = targetX;
        y = targetY;
    }

    // smoothing the camera
    // http://www.mathforgameprogrammers.com/gdc2016/GDC2016_Eiserloh_Squirrel_JuicingYourCameras.pdf
    x = (.8 * x) + (.2 * targetX);
    y = (.8 * y) + (.2 * targetY);

    setXY(x,y);
}

void MapManager::updateMapMiscElements(float deltaTime)
{
}

int MapManager::invertTileY(const int originalTileY) const
{
    return (m_tileMapSize.h - 1) - originalTileY;
}

const float MapManager::getWorldLimitX() const
{
    return m_pCurrentLevelData->getTileMapData().getWidth() * m_tileSizeInWorldUnits;
}

const vector<MetaTileItem*>& MapManager::getMetaTileItems() const
{
    return m_metaTileItems;
}

bool MapManager::isTileGround(int tileX, int tileY) const
{
    int tileData = getTileData(tileX, tileY);
    int tileTopData = getTileData(tileX, tileY + 1);
    return tileData != 0 && tileTopData == 0;
}

bool MapManager::isTileWall(int tileX, int tileY) const
{
    int tileData = getTileData(tileX, tileY);
    return tileData != 0;
}

bool MapManager::isGround(float x, float y) const
{
    int tileX = getTileX(x);
    int tileY = getTileY(y);
    //int invertedTileY = invertTileY(tileY);
    return isTileGround(tileX, tileY);
}

bool MapManager::isWall(float x, float y, float width, float height, bool movingRight) const
{
    float tileSize = m_tileSizeInWorldUnits;

    int tileXBefore = (int) floor(x/tileSize);
    int tileY = (int) floor(y/tileSize);
    bool tileBeforeWall = isTileWall(tileXBefore, tileY);

    if (movingRight)
    {
        int tileXAfter = tileXBefore + 1;
        bool tileAfterWall = isTileWall(tileXAfter, tileY);

        return tileAfterWall && collide(x, y, width, height, tileXAfter * tileSize, tileY * tileSize, tileSize, tileSize);
    }

    return tileBeforeWall && collide(x, y, width, height, tileXBefore * tileSize, tileY * tileSize, tileSize, tileSize);
}

int MapManager::getTileData(int tileX, int tileY) const
{
    tileY = invertTileY(tileY);
    const vector<int>& tiles = m_pGroundLayer->getData();
    int index = tileY * m_pGroundLayer->getWidth() + tileX;
    
    if (index >= tiles.size()) {
        return -1;
    }
    
    return tiles[index];
}

int MapManager::getTileX(float x) const
{
    return round(x/m_tileSizeInWorldUnits);
}

int MapManager::getTileXCeil(float x) const
{
    return ceil(x/m_tileSizeInWorldUnits);
}

int MapManager::getTileXFloor(float x) const
{
    return floor(x/m_tileSizeInWorldUnits);
}

int MapManager::getTileY(float y) const
{
    return round(y/m_tileSizeInWorldUnits);
}

float MapManager::getGroundTileY(float x, float y) const
{
    int tileX = getTileX(x);

    int startTileY = getTileY(y);
    //int invertedTileY = invertTileY(startTileY);

    for (int tileY = startTileY; tileY >= 0; tileY--)
    {
        if (isTileGround(tileX, tileY))
        {
            return (tileY + 1) * m_tileSizeInWorldUnits;
        }
    }
    return 0;
}

float MapManager::getNextWall(float x, float y, bool facingRight) const
{
    int tileX = getTileX(x);

    int tileSize = m_tileSizeInWorldUnits;
    
    int worldUnits = m_worldSize.w;
    for (int i = 0; i < worldUnits; i++)
    {
        float curX = tileX * tileSize;
        if (isWall(curX, y, tileSize, tileSize, facingRight))
        {
            return curX;
        }
        
        if (facingRight)
            tileX++;
        else
            tileX--;
    }
    
    return 0;
}

float MapManager::getDistanceToGroundTile(float x, float y) const
{
    int tileY = getTileY(y);
    int groundTile = getGroundTileY(x, y);
    
    return std::abs(tileY - groundTile);
}

int MapManager::getTileMapWidth() const
{
    return m_tileMapSize.w;
}

int MapManager::getTileMapHeight() const
{
    return m_tileMapSize.h;
}

bool MapManager::isSlidableWall(float x, float y, float width, float height, bool facingRight) const
{
       bool wall = isWall(x, y, width, height, facingRight);

       if (!wall)
           return false;

       int tileX = facingRight ? getTileXCeil(x) : getTileXFloor(x);
       int tileY = getTileY(y);

    int tileData = getTileData(tileX, tileY);
    for(int i = 0; i < m_slidableTileTypes.size(); i++)
    {
        if (tileData == m_slidableTileTypes[i])
        {
            return true;
        }
    }
    
    return false;
}

int MapManager::getWorldUnitsY() const
{
    return CLOCK_MONOTONIC_RAW_APPROX;
}

float MapManager::getXOffSet() const
{
    return m_xOffSet;
}

float MapManager::getYOffSet() const
{
    return m_yOffSet;
}

const Vector2 MapManager::collideWithWall(float x, float y, float width, float height) const
{
    float tileSize = m_tileSizeInWorldUnits;
    
    int tileXBefore = (int) floor(x/tileSize);
    int tileXAfter = tileXBefore + 1;
    int tileYBefore = (int) floor(y/tileSize);
    int tileYAfter = tileYBefore + 1;
    
    bool tileBeforeWall = isTileWall(tileXBefore, tileYBefore);
    bool tileAfterWall = isTileWall(tileXAfter, tileYAfter);
    
    if (tileBeforeWall && collide(x, y, width, height, tileXBefore * tileSize, tileYBefore * tileSize, tileSize, tileSize))
        return Vector2(tileXAfter * tileSize, tileYAfter * tileSize);
    
    if (tileAfterWall && collide(x, y, width, height, tileXAfter * tileSize, tileYAfter * tileSize, tileSize, tileSize))
        return Vector2(tileXBefore * tileSize, tileYBefore * tileSize);
    
    return Vector2(x,y);
    
}

bool MapManager::collide(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2)
{
    return checkCollision(x1, y1, width1, height1, x2, y2, width2, height2);
}

bool MapManager::isVisible(float x, float y) const
{
    return x > m_xOffSet && x < (m_xOffSet + m_worldSize.w) && y > m_yOffSet && y < (m_yOffSet + getWorldUnitsY());
}

float MapManager::getLevelStartX() const
{
    return m_levelStart.x * m_tileSizeInWorldUnits;
}

float MapManager::getLevelStartY() const
{
    return m_levelStart.y * m_tileSizeInWorldUnits;
}

float MapManager::getLevelExitX() const
{
    return m_levelExit.x * m_tileSizeInWorldUnits;
}

float MapManager::getLevelExitY() const
{
    return m_levelExit.y * m_tileSizeInWorldUnits;
}

bool MapManager::hasReachedEndOfLevel(Vector2 point) const
{
    float endPositionX = getLevelExitX();
    float endPositionY = getLevelExitY();
    
    int width = m_tileSizeInWorldUnits;
    int height = m_tileSizeInWorldUnits;
    return collide(point.x, point.y, 1, 1, endPositionX, endPositionY, width, height);
}

bool MapManager::isVisibleInParent(Sprite* pChild) const
{
    if (pChild == nullptr || !pChild->hasTexture())
        return true;
    
    // not rendering/updating tiles that are not visible within the camera offset
    
    const glm::vec2& childCoords = pChild->getUntransformedCoordinates();
    
    float sizeOffset = 4 * m_tileSizeInWorldUnits;
    
    float lowerLimitX = m_xOffSet - sizeOffset;
    float higherLimitX = m_xOffSet + m_worldSize.w + sizeOffset;
    
    if (childCoords.x < lowerLimitX || childCoords.x > higherLimitX)
        return false;
    
    float lowerLimitY = m_yOffSet - sizeOffset;
    float higherLimitY = m_yOffSet + m_worldSize.h + sizeOffset;
    
    if (childCoords.y < lowerLimitY || childCoords.y > higherLimitY)
        return false;
    
    return true;
}

void MapManager::updateCameraShake(float deltaTime)
{
    if (!m_cameraShaking)
        return;
    
    float maxAngle = .1f;
    float maxOffset = m_maxScreenShakeOffsetY;
    float seed = getRandomFloat(1, 10);
    
    m_cameraShakeAngle = maxAngle * m_cameraShakeIntensity * perlin::noise(seed, deltaTime);
    m_cameraShakeOffsetX = maxOffset * m_cameraShakeIntensity * perlin::noise(seed + 1, deltaTime);
    m_cameraShakeOffsetY = maxOffset * m_cameraShakeIntensity * perlin::noise(seed + 2, deltaTime);
    m_cameraShakeAccumulator += deltaTime;
    
    if (m_cameraShakeAccumulator > m_screenShakeDuration)
    {
        m_cameraShaking = false;
        m_cameraShakeAngle = 0;
        m_cameraShakeOffsetX = 0;
        m_cameraShakeOffsetY = 0;
        m_cameraShakeAccumulator = 0;
        updateCamera(m_cameraCenterX, m_cameraCenterY);
    }
}

void MapManager::shakeCamera(float intensity)
{
    if (intensity < 0)
    {
        intensity = getRandomFloat(5, 10);
    }

    m_cameraShaking = true;
    m_cameraShakeIntensity = intensity;
}
