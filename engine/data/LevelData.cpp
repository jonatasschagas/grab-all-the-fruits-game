#include "LevelData.hpp"

LevelData::LevelData(const string& title,
              const string& tileMapFilename,
              const string& levelsFolder,
              const string& tiletSetsFolder,
              const string& metaLayer,
              const string& groundLayer,
              const string& soundTrack)
{
    initializeMembers();
    
    m_title = title;
    m_tileMapFilename = tileMapFilename;
    m_groundLayer = groundLayer;
    m_metaLayer = metaLayer;
    m_soundtrack = soundTrack;
    m_pTileMapData = new TileMapData(m_tileMapFilename, levelsFolder, tiletSetsFolder);
}

LevelData::~LevelData()
{
    delete m_pTileMapData;
    
    initializeMembers();
}
