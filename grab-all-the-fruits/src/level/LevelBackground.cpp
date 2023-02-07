#include "LevelBackground.hpp"

#include <math.h>

LevelBackground::LevelBackground(const Vector2& backgroundTileSizeInGameUnits, PlatformManager* pPlatformManager, const string& textureFileName, const Vector2& textureSize)
    : Sprite(pPlatformManager)
{
    initializeMembers();
    
    m_pPlatformManager = pPlatformManager;

    loadTexture(textureFileName);

    const Vector2 screenSize = pPlatformManager->getScreenSizeInGameUnits();
    int numTileHorizontally = screenSize.x / backgroundTileSizeInGameUnits.x;
    int numTileVertically = ceil(screenSize.y / backgroundTileSizeInGameUnits.y);
    float bgHeight = numTileVertically * backgroundTileSizeInGameUnits.y;
    
    m_pBg1 = new Sprite(pPlatformManager);
    addChild(m_pBg1);
    m_pBg1->setSize(screenSize.x, bgHeight);
    m_pBg1->setXY(0, 0);

    m_pBg2 = new Sprite(pPlatformManager);
    addChild(m_pBg2);
    m_pBg2->setSize(screenSize.x, bgHeight);
    m_pBg2->setXY(0, m_pBg1->getY() + m_pBg1->getSize().y);
    
    for (int bg = 0; bg < 2; ++bg)
    {
        for (int i = 0; i < numTileHorizontally; ++i)
        {
            for (int j = 0; j < numTileVertically; ++j)
            {
                Sprite* pTile = new Sprite(pPlatformManager);
                pTile->loadTexture(textureFileName);
                pTile->setXY(i * backgroundTileSizeInGameUnits.x, j * backgroundTileSizeInGameUnits.y);
                pTile->setSize(backgroundTileSizeInGameUnits);
                pTile->setTextureCoordinates(0, 0, textureSize.x, textureSize.y);
                
                if (bg == 0)
                {
                    m_pBg1->addChild(pTile);
                }
                else
                {
                    m_pBg2->addChild(pTile);
                }
            }
        }
    }
    
}

LevelBackground::~LevelBackground()
{
    initializeMembers();
}

void LevelBackground::update(float delta)
{
    float y = m_pBg1->getY();
    //TODO: Make this speed configurable
    m_pBg1->setY(y - 0.045f);    
    m_pBg2->setY(m_pBg1->getY() + m_pBg1->getSize().y);

    if (m_pBg1->getY() < -m_pBg1->getSize().y)
    {
        Sprite* pTemp = m_pBg1;
        m_pBg1 = m_pBg2;
        m_pBg2 = pTemp;
    }

    Sprite::update(delta);
}

void LevelBackground::render()
{
    Sprite::render();
}