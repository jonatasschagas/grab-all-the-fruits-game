#include "TileMapSprite.hpp"
#include "data/TileMapData.hpp"
#include "platform/PlatformManager.h"

TileMapSprite::TileMapSprite(const GameSize& tileSizeWorldUnits, PlatformManager* pPlatformManager) : Sprite(pPlatformManager)
{
    initializeMembers();
        
    m_tileSizeInWorldUnits = tileSizeWorldUnits;
    m_pPlatformManager = pPlatformManager;

    setPivotAtCenter(true);
}

TileMapSprite::~TileMapSprite()
{
    unloadMap();
    initializeMembers();
}

void TileMapSprite::update(float delta)
{
    Sprite::update(delta);

    float x = getTransformedX();
    float y = getTransformedY();
    float targetX = -m_xOffSet;
    float targetY = -m_yOffSet;
    
    // smoothing the camera
    // http://www.mathforgameprogrammers.com/gdc2016/GDC2016_Eiserloh_Squirrel_JuicingYourCameras.pdf
    x = (.8 * x) + (.2 * targetX);
    y = (.8 * y) + (.2 * targetY);

    setXY(x,y);
}

void TileMapSprite::render()
{
    Sprite::render();
}

void TileMapSprite::unloadMap()
{
    if (m_pCurrentMapData != nullptr)
    {
        delete m_pCurrentMapData;
        m_pCurrentMapData = nullptr;
    }
}

void TileMapSprite::loadMap(TileMapData* pMapData)
{
    if (pMapData != nullptr)
        unloadMap();

    m_pCurrentMapData = pMapData;
    GameSize m_tileMapSize(m_pCurrentMapData->getWidth(), m_pCurrentMapData->getHeight());
    
    // creates the map as a huge sprite
    setXY(0,0);
    setSize(m_tileMapSize.w * m_tileSizeInWorldUnits.w,
            m_tileMapSize.h * m_tileSizeInWorldUnits.h);
    setPivotAtCenter(true);

    loadMapLayers();
}

void TileMapSprite::loadMapLayers()
{
    assert(m_pCurrentMapData != nullptr);

    const vector<TileMapLayer*>& layersArray = m_pCurrentMapData->getLayers();
    for (int i = 0; i < layersArray.size(); i++)
    {
        TileMapLayer* pLayer = layersArray[i];
        if (pLayer->isVisible())
        {
            Sprite* pSpriteLayer = new Sprite(m_pPlatformManager);
            pSpriteLayer->setXY(0, 0);
            pSpriteLayer->setSize(pLayer->getWidth() * m_tileSizeInWorldUnits.w,
                                  pLayer->getHeight() * m_tileSizeInWorldUnits.h);
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

const float TileMapSprite::getWorldLimitX() const
{
    return m_pCurrentMapData->getWidth() * m_tileSizeInWorldUnits.w;
}

int TileMapSprite::getTileX(float x) const
{
    return (int)(x / m_tileSizeInWorldUnits.w);
}

int TileMapSprite::getTileXCeil(float x) const
{
    return (int)ceil(x / m_tileSizeInWorldUnits.w);
}

int TileMapSprite::getTileXFloor(float x) const
{
    return (int)floor(x / m_tileSizeInWorldUnits.w);
}

int TileMapSprite::getTileY(float y) const
{
    return invertTileY((int)(y / m_tileSizeInWorldUnits.h));
}

int TileMapSprite::getTileMapWidth() const
{
    return getWorldUnitsX() / m_tileSizeInWorldUnits.h;
}

int TileMapSprite::getTileMapHeight() const
{
    return getWorldUnitsY() / m_tileSizeInWorldUnits.h;
}

int TileMapSprite::getWorldUnitsX() const
{
    return m_pPlatformManager->getWorldSizeUnits().w;
}

int TileMapSprite::getWorldUnitsY() const
{
    return m_pPlatformManager->getWorldSizeUnits().h;
}

float TileMapSprite::getXOffSet() const
{
    return m_xOffSet;
}

void TileMapSprite::setXOffSet(float xOffSet)
{
    m_xOffSet = xOffSet;
}

float TileMapSprite::getYOffSet() const
{
    return m_yOffSet;
}

void TileMapSprite::setYOffSet(float yOffSet)
{
    m_yOffSet = yOffSet;
}

int TileMapSprite::getTileData(int tileX, int tileY, const string& tileLayerName) const
{
    tileY = invertTileY(tileY);
    TileMapLayer* pTileLayer = nullptr;
    for (int i = 0; i < m_pCurrentMapData->getLayers().size(); i++)
    {
        TileMapLayer* pLayer = m_pCurrentMapData->getLayers()[i];
        if (strcmp(pLayer->getName().c_str(),tileLayerName.c_str()) == 0)
        {
            pTileLayer = pLayer;
            break;
        }
    }
    const vector<int>& tiles = pTileLayer->getData();
    int index = tileY * pTileLayer->getWidth() + tileX;
    
    if (index >= tiles.size()) {
        return -1;
    }
    
    return tiles[index];
}

int TileMapSprite::getTileData(float x, float y, const string& tileLayerName) const
{
    return getTileData(getTileX(x), getTileY(y), tileLayerName);
}

bool TileMapSprite::isVisibleInParent(Sprite* pChild) const
{
    return true;
}

void TileMapSprite::createTile(int x, int y, Sprite* pSpriteLayer, TileMapLayer* pLayer)
{
    // tiles from Tiled software are placed from the top to the bottom (screen starts from top left)
    // however in this "engine" we place the tiles from the bottom to the top (screen starts from bottom left)
    int yNormal = y;
    y = invertTileY(y);

    // reading texture coordinates
    int tileMapIndex = y * m_pCurrentMapData->getWidth() + x;
    const vector<int>& tileDataArray = pLayer->getData();
    
    if (tileDataArray.size() <= tileMapIndex)
    {
        // invalid tile coordinates
        return;
    }
    
    int tileType = tileDataArray[tileMapIndex];
    
    TileSet* pTileSet = m_pCurrentMapData->getTileSetFromTileData(tileType);
    if (pTileSet == NULL)
    {
        // no tile set for this tileType
        return;
    }
    
    TileSetConfig* pTileSetConfig = pTileSet->getConfig();

    int tileNumberInTileSet = tileType - pTileSet->getFirstGid();
    
    const TileConfig* pTileConfig = pTileSetConfig->getTileConfig(tileNumberInTileSet);

    // finally, creating the sprite
    int width = m_tileSizeInWorldUnits.w;
    int height = m_tileSizeInWorldUnits.h;

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

int TileMapSprite::invertTileY(const int originalTileY) const
{
    return (m_pCurrentMapData->getHeight() - 1) - originalTileY;
}

const GameSize& TileMapSprite::getTileSizeInWorldUnits() const
{
    return m_tileSizeInWorldUnits;
}
