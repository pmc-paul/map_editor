#include "map_file_parser.h"

MapFileParser::MapFileParser(std::string filepath)
    : configFilePath(filepath)
{
    config = YAML::LoadFile(configFilePath);

    readMapParams();
    readMapEditorParams();
    readWaypoints();
}

void MapFileParser::readMapParams()
{
    mapConfig = new MapConfig;

    mapConfig->filePath = configFilePath.substr(0, configFilePath.find_last_of("\\/") + 1);
    mapConfig->filePath += config["image"].as<std::string>();

    mapConfig->resolution = config["resolution"].as<float>();

    for (int i = 0; i < 3; i++) {
        mapConfig->origin[i] = config["origin"][i].as<float>();
    }
}

void MapFileParser::readMapEditorParams()
{
    mapEditorConfig = new MapEditorConfig;

    if(config["map_editor"]) 
    {
        qDebug() << "Map editor Parameters found, loading config...";

        YAML::Node mapEditorParams = config["map_editor"];

        mapEditorConfig->scale = mapEditorParams["scale"].as<int>();
        mapEditorConfig->rotation = mapEditorParams["rotation"].as<int>();

        mapEditorConfig->verticalScrollBarValue = mapEditorParams["vertical_scroll_bar"].as<int>();
        mapEditorConfig->horizontalScrollBarValue = mapEditorParams["horizontal_scroll_bar"].as<int>();

        mapEditorConfig->viewWaypoints = mapEditorParams["view_waypoints"].as<bool>();
        mapEditorConfig->viewRestrictedZones = mapEditorParams["view_restricted_zones"].as<bool>();
    }
    else
    {
        qDebug() << "Map editor Parameters not found, creating new config...";

        mapEditorConfig->scale = 100;
        mapEditorConfig->rotation = 0;

        mapEditorConfig->verticalScrollBarValue = 0;
        mapEditorConfig->horizontalScrollBarValue = 0;

        mapEditorConfig->viewWaypoints = true;
        mapEditorConfig->viewRestrictedZones = true;
    }

}

void MapFileParser::saveMapEditorParams()
{
    YAML::Node mapEditorParams = config["map_editor"];

    mapEditorParams["scale"] = mapEditorConfig->scale;
    mapEditorParams["rotation"] = mapEditorConfig->rotation;

    mapEditorParams["vertical_scroll_bar"] = mapEditorConfig->verticalScrollBarValue;
    mapEditorParams["horizontal_scroll_bar"] = mapEditorConfig->horizontalScrollBarValue;

    mapEditorParams["view_waypoints"] = mapEditorConfig->viewWaypoints;
    mapEditorParams["view_restricted_zones"] = mapEditorConfig->viewRestrictedZones;

    std::ofstream fout(configFilePath);
    fout << config;
    fout.close();
}

std::vector<Waypoint *> MapFileParser::readWaypoints()
{
    if(config["waypoints"])
    {
        qDebug() << "Waypoints read from file.";
    }
    else
    {
        qDebug() << "Waypoints not found, creating empty vector.";
    }

    std::vector<Waypoint *> waypoints = {};

    return waypoints;
}

void MapFileParser::saveWaypoints(std::vector<Waypoint *> waypoints)
{
    YAML::Node waypointsNode = config["waypoints"];

    for(int i = 0; i < waypoints.size(); i++)
    {
        waypointsNode[i]["id"] = i;
        waypointsNode[i]["map_x"] = waypoints[i]->getMapX();
        waypointsNode[i]["map_y"] = waypoints[i]->getMapY();
        waypointsNode[i]["scene_x"] = waypoints[i]->pos().x();
        waypointsNode[i]["scene_y"] = waypoints[i]->pos().y();
    }

    std::ofstream fout(configFilePath);
    fout << config;
    fout.close();
}

MapConfig *MapFileParser::getMapConfig()
{
    return mapConfig;
}

MapEditorConfig *MapFileParser::getMapEditorConfig()
{
    return mapEditorConfig;
}
