#ifndef MAP_FILE_PARSER_H
#define MAP_FILE_PARSER_H

#include <QDebug>
#include <QString>

#include <fstream>

#include "yaml-cpp/yaml.h"
#include "waypoint.h"
#include "link.h"

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
    void readWaypoints();
    void readLinks();

    void saveMapEditorParams();
    void saveWaypointsAndLinks(std::vector<Waypoint *>);

    MapConfig *getMapConfig() { return mapConfig; }
    MapEditorConfig *getMapEditorConfig() { return mapEditorConfig; }
    std::vector<Waypoint *> getWaypoints() { return waypoints; }
    std::vector<std::pair<int, int>> getLinks() { return links; }

private:
    std::string configFilePath;

    YAML::Node config;

    MapConfig *mapConfig;
    MapEditorConfig *mapEditorConfig;
    std::vector<Waypoint *> waypoints;
    std::vector<std::pair<int, int>> links;

    int findWaypointIndexInVector(std::vector<Waypoint *> waypoints, Waypoint *waypoint);
    double calculateLinkWeight(Waypoint *start, Waypoint *end);
};

#endif // MAP_FILE_PARSER_H
