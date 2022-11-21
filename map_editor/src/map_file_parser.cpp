#include "map_file_parser.h"

MapFileParser::MapFileParser(std::string filepath)
    : configFilePath(filepath)
{
    config = YAML::LoadFile(configFilePath);

    readMapParams();
    readMapEditorParams();
    readWaypoints();
    readLinks();
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

void MapFileParser::readWaypoints()
{
    waypoints = std::vector<Waypoint *>();

    if(config["waypoints"])
    {
        for(int i = 0; i < config["waypoints"].size(); i++)
        {
            YAML::Node waypointNode = config["waypoints"][i];
            Waypoint *waypoint = new Waypoint();
            
            // set type
            YAML::Node infoNode = waypointNode["info"];
            waypoint->setWaypointType(infoNode["type"].as<std::string>());
            waypoint->setAisle(infoNode["aisle"].as<int>());
            waypoint->setShelf(infoNode["shelf"].as<std::string>());

            // set position
            YAML::Node positionNode = waypointNode["position"];
            waypoint->setResolution(mapConfig->resolution);
            waypoint->setOrigin(mapConfig->origin);

            // qDebug() << positionNode["map"]["x"].as<float>();
            // qDebug() << positionNode["map"]["y"].as<float>();

            // should set the map position at the same time
            int size = 20;
            if(infoNode["size"])
                int size = infoNode["size"].as<int>();
            waypoint->setPos(QPointF(positionNode["scene"]["x"].as<float>() + 0.5 * size,
                                     positionNode["scene"]["y"].as<float>() + 0.5 * size));

            // append to vector
            waypoints.push_back(waypoint);
        }        
    }
}

void MapFileParser::readLinks()
{
    links = std::vector<std::pair<int, int>>();

    if(config["links"])
    {
        for(int i = 0; i < config["links"].size(); i++)
        {
            YAML::Node linkNode = config["links"][i];

            std::pair<int, int> link(linkNode["start"].as<int>(),
                                     linkNode["end"].as<int>());
            links.push_back(link);
        }
    }
}

void MapFileParser::saveWaypointsAndLinks(std::vector<Waypoint *> waypoints)
{
    YAML::Node waypointsNode;
    YAML::Node linksNode;
    for(int i = 0; i < waypoints.size(); i++)
    {
        YAML::Node scene;
        scene["x"] = waypoints[i]->pos().x();
        scene["y"] = waypoints[i]->pos().y();

        YAML::Node map;
        map["x"] = waypoints[i]->getMapX();
        map["y"] = waypoints[i]->getMapY();

        YAML::Node position;
        position["scene"] = scene;
        position["map"] = map;
        
        YAML::Node info;
        info["type"] = waypoints[i]->getTypeInQString().toStdString();
        info["aisle"] = waypoints[i]->getAisle();
        info["shelf"] = waypoints[i]->getShelf().toStdString();
        info["size"] = waypoints[i]->getSize();

        YAML::Node waypoint;
        waypoint["id"] = i;
        waypoint["position"] = position;
        waypoint["info"] = info;

        waypointsNode[i] = waypoint;

        // check for new links to add
        std::vector<Link *> links = waypoints[i]->getLinksAsStdVector();
        for(int j = 0; j < links.size(); j++)
        {
            int startItemIndex = findWaypointIndexInVector(waypoints, links[j]->startItem());
            int endItemIndex = findWaypointIndexInVector(waypoints, links[j]->endItem());

            if((startItemIndex == i && endItemIndex > i) || (startItemIndex > i && endItemIndex == i))
            {
                YAML::Node link;
                link["start"] = startItemIndex;
                link["end"] = endItemIndex;
                link["weight"] = calculateLinkWeight(waypoints[startItemIndex], waypoints[endItemIndex]);

                linksNode.push_back(link);
            }
        }

    }

    config["waypoints"] = waypointsNode;
    config["links"] = linksNode;

    std::ofstream fout(configFilePath);
    fout << config;
    fout.close();
}

int MapFileParser::findWaypointIndexInVector(std::vector<Waypoint *> waypoints, Waypoint *waypoint)
{
    auto it = find(waypoints.begin(), waypoints.end(), waypoint);
    if (it != waypoints.end()) 
        return it - waypoints.begin();
    else
        return -1;
}

double MapFileParser::calculateLinkWeight(Waypoint *start, Waypoint *end)
{
    return sqrt(pow(start->getMapX() - end->getMapX(), 2) + pow(start->getMapY() - end->getMapY(), 2));
}