#pragma once

#ifndef LevelData_hpp
#define LevelData_hpp

#include <stdio.h>
#include <string>

#include "data/TileMapData.hpp"

using namespace std;

class LevelData
{
public:
    
    LevelData(const string& title,
              const string& tileMapFilename,
              const string& levelsFolder,
              const string& tiletSetsFolder,
              const string& metaLayer,
              const string& groundLayer,
              const string& soundTrack);
    ~LevelData();
    
    const string& getTitle() const { return m_title; }
    const string& getTileMapFilename() const { return m_tileMapFilename; };
    const string& getMetaLayer() const { return m_metaLayer; };
    const string& getGroundLayer() const { return m_groundLayer; };
    const string& getSoundTrack() const { return m_soundtrack; };
    const TileMapData& getTileMapData() const { return *m_pTileMapData; };
    
private:
    
    string m_title;
    string m_tileMapFilename;
    string m_groundLayer;
    string m_metaLayer;
    string m_soundtrack;
    TileMapData* m_pTileMapData;
    
    void initializeMembers()
    {
        m_title = "";
        m_tileMapFilename = "";
        m_groundLayer = "";
        m_metaLayer = "";
        m_soundtrack = "";
        m_pTileMapData = nullptr;
    }
    
};

#endif /* LevelData_hpp */