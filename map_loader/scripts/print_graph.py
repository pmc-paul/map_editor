#!/usr/bin/env python

import networkx as nx
import matplotlib.pyplot as plt

from yaml import load, dump
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

if __name__ == "__main__":
    # load yaml data
    stream = file('/home/willi/map_editor_ws/maps/sim/sim.yaml', 'r')
    data = load(stream, Loader=Loader)

    # create graph
    graph = nx.Graph()

    # load waypoints as nodes in graph
    node_color = []
    if(data["waypoints"]):
        for waypoint in data["waypoints"]:
            id = waypoint["id"]
            x = waypoint["position"]["map"]["x"]
            y = waypoint["position"]["map"]["y"]

            if(waypoint["info"]["type"] == "Start"):
                node_color.append('green')
            elif(waypoint["info"]["type"] == "Aisle"):
                node_color.append('blue')
            elif(waypoint["info"]["type"] == "Shelf"):
                node_color.append('red')

            graph.add_node(id, pos=(x, y))
    
    # load links between as edges in graph
    edge_color = []
    if(data["links"]):
        for link in data["links"]:
            start = link["start"]
            end = link["end"]
            weight = "%.2f" % link["weight"]

            edge_color.append('black')

            graph.add_edge(start, end, weight=weight)
    

    # shortest path example from start to 4C
    sp = nx.shortest_path(graph, source=0, target=20)
    print(sp)

    # change edges and nodes color to display shortest path
    edge_color = ['black' if not edge in list(zip(sp,sp[1:])) else 'green' for edge in graph.edges()]

    for index in sp:
        node_color[index] = 'grey'

    # get graph attributes
    pos = nx.get_node_attributes(graph,'pos')
    edge_labels = nx.get_edge_attributes(graph, "weight")

    # nodes
    nx.draw_networkx(graph, pos, node_color= node_color, node_size=450)
    nx.draw_networkx_labels(graph, pos, node_color=node_color)

    # edges
    nx.draw_networkx_edges(graph, pos, edge_color=edge_color)
    nx.draw_networkx_edge_labels(graph, pos, edge_labels=edge_labels, edge_color=edge_color)

    # display graph
    plt.axis('off')
    plt.show() 