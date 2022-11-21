#define USAGE "\nUSAGE: load_waypoints <map.yaml>\n" \
              "  map.yaml: map description file\n"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "ros/ros.h"
#include "ros/console.h"
#include "yaml-cpp/yaml.h"

#include "visualization_msgs/Marker.h"
#include "visualization_msgs/MarkerArray.h"

class WaypointsServer
{
    public:
        /** Trivial constructor */
        WaypointsServer(const std::string& fname)
        {
            ros::NodeHandle private_nh("~");
            frame_id_ = "map";

            // Latched publisher for data
            waypoints_markers_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("visualization_marker_array", 1, true);

            if (!loadWaypointsFromYaml(fname))
            {
                exit(-1);
            }
        }

    private:
        ros::NodeHandle nh_;
        ros::Publisher waypoints_markers_pub_;

        std::string frame_id_;

        bool loadWaypointsFromYaml(std::string path_to_yaml)
        {
            // create yaml parser
            YAML::Node config = YAML::LoadFile(path_to_yaml);

            // loop into waypoints and add them to the marker array
            YAML::Node waypoints = config["waypoints"];
            if(waypoints)
            {
                std::cout << "Waypoints found." << std::endl;

                visualization_msgs::MarkerArray waypointsArrayMsg;
                for(int i = 0; i < waypoints.size(); i++) {
                    std::cout << "Loading waypoint #" << i << std::endl;

                    // create new marker array message
                    visualization_msgs::Marker waypointMsg;

                    waypointMsg.header.frame_id = frame_id_;
                    waypointMsg.id = waypoints[i]["id"].as<int>();
                    waypointMsg.pose.position.x = waypoints[i]["position"]["map"]["x"].as<float>();
                    waypointMsg.pose.position.y = waypoints[i]["position"]["map"]["y"].as<float>();

                    waypointMsg.action = 0; // add or modify

                    waypointMsg.type = 3;   // cylinder

                    waypointMsg.scale.x = 1.0;
                    waypointMsg.scale.y = 1.0;

                    // set marker color depending on type 
                    std::string type = waypoints[i]["info"]["type"].as<std::string>();
                    if(type == "Start")
                        waypointMsg.color.g = 1.0;
                    else if(type == "Aisle")
                        waypointMsg.color.b = 1.0;
                    else if(type == "Shelf")
                        waypointMsg.color.r = 1.0;

                    waypointMsg.color.a = 1.0;    

                    // append marker to array
                    waypointsArrayMsg.markers.push_back(waypointMsg);
                }

                // load links?

                waypoints_markers_pub_.publish(waypointsArrayMsg);
            }
            else
            {
                ROS_ERROR("The map does not contain a waypoints tag or it is invalid.");
                return false;
            }

            return true;
        }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "waypoints_server", ros::init_options::AnonymousName);
    ros::NodeHandle nh("~");
    if(argc != 2)
    {
        ROS_ERROR("%s", USAGE);
        exit(-1);
    }

    std::string fname(argv[1]);

    try
    {
        WaypointsServer ms(fname);
        sleep(1);
        std::cout << "Added waypoint marker to array." << std::endl;
        ros::spinOnce();
    }
    catch(std::runtime_error& e)
    {
        ROS_ERROR("waypoints_server exception: %s", e.what());
        return -1;
    }

    return 0;
}