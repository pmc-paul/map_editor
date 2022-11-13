#ifndef MAP_FILE_PARSER_H
#define MAP_FILE_PARSER_H

#include <QDebug>
#include <QString>

#include <fstream>

#include "yaml-cpp/yaml.h"
#include "waypoint.h"

struct MapConfig {
    std::string filePath;
    float resolution;
    float origin[3];
};

struct MapEditorConfig {
    int scale;
    int rotation;
    int verticalScrollBarValue;
    int horizontalScrollBarValue;
    bool viewWaypoints;
    bool viewRestrictedZones;
};

class MapFileParser
{
public:
   
    MapFileParser(std::string filepath);

    void readMapParams();
    void readMapEditorParams();
    std::vector<Waypoint *> readWaypoints();

    void saveMapEditorParams();
    void saveWaypoints(std::vector<Waypoint *>);

    MapConfig *getMapConfig();
    MapEditorConfig *getMapEditorConfig();

private:
    std::string configFilePath;

    YAML::Node config;

    MapConfig *mapConfig;
    MapEditorConfig *mapEditorConfig;
};

#endif // MAP_FILE_PARSER_H
