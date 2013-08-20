#ifndef _PATHFINDING_HPP_
#define _PATHFINDING_HPP_

#include <vector>
#include <map>

class SpaceShip;
class SpaceShipPart;


class Pathfinding
{
public:
    struct Path
    {
		Path(std::vector<SpaceShipPart *> *waypoints, double length);
        double mLength;
        std::vector<SpaceShipPart *> *mWaypoints;
    };
    
    static Path *findHumanPath(SpaceShip *ship, SpaceShipPart *from, SpaceShipPart *to);
    static Path *findPath(SpaceShip *ship, SpaceShipPart *from, SpaceShipPart *to);
    static Path *findPathWithout(SpaceShipPart *part, SpaceShipPart *from, SpaceShipPart *to);

private:
    static double cost(SpaceShipPart *p1, SpaceShipPart *p2);
    static double heuristic(SpaceShipPart *p1, SpaceShipPart *p2);
    static Path *reconstructPath(std::map<SpaceShipPart *, SpaceShipPart *> &cameFrom, SpaceShipPart *current, Path *path);
	static std::map<SpaceShipPart *, double>::iterator getSmallest(std::map<SpaceShipPart *, double>::iterator begin, std::map<SpaceShipPart *, double>::iterator end);

};

#endif
