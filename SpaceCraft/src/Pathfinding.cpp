#include "Pathfinding.hpp"

#include "SpaceShip.hpp"
#include "SpaceShipPart.hpp"

#include "OGRE/OgreSceneNode.h"

std::map<SpaceShipPart *, double>::iterator Pathfinding::getSmallest(std::map<SpaceShipPart *, double>::iterator begin, std::map<SpaceShipPart *, double>::iterator end)
{
    std::map<SpaceShipPart *, double>::iterator smallest = begin;
    ++begin;
    while(begin != end)
    {
        if(begin->second < smallest->second)
            smallest = begin;
        ++begin;
    }
    return smallest;
}

Pathfinding::Path *Pathfinding::findPath(SpaceShip *ship, SpaceShipPart *from, SpaceShipPart *to)
{
    std::vector<SpaceShipPart *> closedList;
    std::map<SpaceShipPart *, double> openList; // fScore
    std::map<SpaceShipPart *, double> gScore;
    std::map<SpaceShipPart *, SpaceShipPart*> cameFrom;

    gScore[from] = 0;
    openList[from] = gScore[from] + heuristic(from, to);

    while(!openList.empty())
    {
        std::map<SpaceShipPart *, double>::iterator current = getSmallest(openList.begin(), openList.end());

        std::map<SpaceShipPart *, double> links;
		for(int i=0; i<current->first->getNumberNeighbors(); i++)
		{
			if(current->first->getNeighbor(i))
				links[current->first->getNeighbor(i)] = current->first->getNeighborInfo(i)->mPos.length();
		}

        for(std::map<SpaceShipPart *, double>::iterator i=links.begin(); i!=links.end(); ++i)
        {
            SpaceShipPart *neighbor = i->first;

            if(find(closedList.begin(), closedList.end(), neighbor) != closedList.end())
                continue;

            double tentativeG = gScore[current->first] + cost(current->first, neighbor);

            if(openList.count(neighbor) > 0 && tentativeG >= gScore[neighbor])
                continue;

            cameFrom[neighbor] = current->first;
            gScore[neighbor] = tentativeG;
            double f = tentativeG + heuristic(neighbor, to);
            openList[neighbor] = f;
        }

        if(current->first == to)
            return reconstructPath(cameFrom, to, new Path(new std::vector<SpaceShipPart *>(), 0));

        closedList.push_back(current->first);
        openList.erase(current);
    }

    return NULL;
}

double Pathfinding::cost(SpaceShipPart *n1, SpaceShipPart *n2)
{
	return n1->getParentSceneNode()->getPosition().distance(n2->getParentSceneNode()->getPosition());
}

double Pathfinding::heuristic(SpaceShipPart *n1, SpaceShipPart *n2)
{
	return n1->getParentSceneNode()->getPosition().distance(n2->getParentSceneNode()->getPosition());
}

Pathfinding::Path *Pathfinding::reconstructPath(std::map<SpaceShipPart *, SpaceShipPart*> &cameFrom, SpaceShipPart *current, Path *path)
{
    if(cameFrom.count(current) > 0)
    {
		path->mLength += cost(current, cameFrom[current]);
        path = reconstructPath(cameFrom, cameFrom[current], path);
    }

	path->mWaypoints->push_back(current);
    return path;
}

Pathfinding::Path::Path(std::vector<SpaceShipPart *> *waypoints, double length)
{
	mWaypoints = waypoints;
	mLength = length;
}