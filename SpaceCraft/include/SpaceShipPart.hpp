#ifndef _SPACESHIPPART_HPP_
#define _SPACESHIPPART_HPP_

/*     Neighbor indexing:
X
^           _____
|          |     |     
|          |  0  |
|     _____|_____|_____
|    |     |     |     |
|    |  3  | this|  1  |
|    |_____|_____|_____|
|          |     |
|          |  2  |
|          |_____|
|
+--------------------------> Z

*/

#include "Entity.hpp"

#include <vector>

class SpaceShipPart : public Entity
{
public:
    enum PART_TYPE
    {
        PART_FLOOR,
        PART_WALL
    };

    SpaceShipPart(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    
    virtual bool update(float elapsedTime){return true;}

    int getNumberNeighbors() {return mNumberNeighbors;}
    SpaceShipPart *getNeighbor(int i) {return mNeighbor.at(i);}
    void setNeighbor(SpaceShipPart *p, int i) {mNeighbor.at(i) = p;}

private:
    Ogre::Entity *mEntity;
    std::vector<SpaceShipPart *> mNeighbor;
    PART_TYPE mPartType;
    int mNumberNeighbors;
};

#endif