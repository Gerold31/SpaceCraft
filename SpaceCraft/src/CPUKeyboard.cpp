#include "CPUKeyboard.hpp"

#include "ENGINE.hpp"
#include "CPU.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"

SpaceShipPart::SpaceShipPartInfo CPUKeyboard::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, -0.5, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

CPUKeyboard::CPUKeyboard(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :Hardware(0x30cf7406, 0x1C6C8B36, 0x1802, PART_FLOORMOUNT, pos, ori, parent, name, "CPU_Keyboard", engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPUKeyboard.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);

    commonConstructor();
}

CPUKeyboard::CPUKeyboard(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine)
    :Hardware(0x30cf7406, 0x1C6C8B36, 0x1802, PART_FLOORMOUNT, pos, ori, parent, staticGeometry, name, "CPU_Keyboard", engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPUKeyboard.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
	//setupInstancedMaterialToEntity(mEntity);
    staticGeometry->addEntity(mEntity, pos, ori);

    commonConstructor();
}

void CPUKeyboard::commonConstructor()
{
    mInterruptMsg = 0;

    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
}

bool CPUKeyboard::update(float elapsedTime)
{
    return true;
}

void CPUKeyboard::interrupt()
{
    switch(mCPU->getRegister(0))
    {
    case 0: // Clear buffer
        while(mKeyBuffer.size() > 0)
            mKeyBuffer.pop();
        break;
    case 1: // Next key in C, 0 if buffer is empty
        if(mKeyBuffer.empty())
            mCPU->setRegister(2, 0);
        else
        {
            mCPU->setRegister(2, mKeyBuffer.front());
            mKeyBuffer.pop();
        }
        break;
    case 2: // C to 1 if B is pressed, 0 else
        if(mInput->getKeyboard()->isKeyDown(keyToCode(mCPU->getRegister(1))))
            mCPU->setRegister(2, 1);
        else mCPU->setRegister(2, 0);
        break;
    case 3: // B != 0 enable interrupt with message B, B == 0 disable interrupt
        mInterruptMsg = mCPU->getRegister(1);
        break;
    default:
        break;
    }
}

bool CPUKeyboard::keyPressed(const OIS::KeyEvent &e)
{
    if(mKeyBuffer.size() < 16)
        mKeyBuffer.push(codeToKey(e.key));
    if(mInterruptMsg)
    {
        mCPU->interrupt(mInterruptMsg);
    }
    return true;
}

bool CPUKeyboard::keyReleased(const OIS::KeyEvent &e)
{
    if(mInterruptMsg)
        mCPU->interrupt(mInterruptMsg);
    return true;
}

OIS::KeyCode CPUKeyboard::keyToCode(WORD key)
{
    switch(key)
    {
    case ' ':
        return OIS::KC_SPACE;
    case 0x10:
        return OIS::KC_BACK;
    case '\n':
    case 0x11:
        return OIS::KC_RETURN;
    case 0x12:
        return OIS::KC_INSERT;
    case 0x13:
        return OIS::KC_DELETE;
    case 0x80:
        return OIS::KC_UP;
    case 0x81:
        return OIS::KC_DOWN;
    case 0x82:
        return OIS::KC_LEFT;
    case 0x83:
        return OIS::KC_RIGHT;
    case '`':
        return OIS::KC_GRAVE;
    case '-':
        return OIS::KC_MINUS;
    case '=':
        return OIS::KC_EQUALS;
    case '[':
        return OIS::KC_LBRACKET;
    case ']':
        return OIS::KC_RBRACKET;
    case ';':
        return OIS::KC_SEMICOLON;
    case '\'':
        return OIS::KC_APOSTROPHE;
    case '\\':
        return OIS::KC_BACKSLASH;
    case ',':
        return OIS::KC_COMMA;
    case '.':
        return OIS::KC_PERIOD;
    case '/':
        return OIS::KC_SLASH;
    case '0':
        return OIS::KC_0;
    case '1':
        return OIS::KC_1;
    case '2':
        return OIS::KC_2;
    case '3':
        return OIS::KC_3;
    case '4':
        return OIS::KC_4;
    case '5':
        return OIS::KC_5;
    case '6':
        return OIS::KC_6;
    case '7':
        return OIS::KC_7;
    case '8':
        return OIS::KC_8;
    case '9':
        return OIS::KC_9;
    case 'a':
    case 'A':
        return OIS::KC_A;
    case 'b':
    case 'B':
        return OIS::KC_B;
    case 'c':
    case 'C':
        return OIS::KC_C;
    case 'd':
    case 'D':
        return OIS::KC_D;
    case 'e':
    case 'E':
        return OIS::KC_E;
    case 'f':
    case 'F':
        return OIS::KC_F;
    case 'g':
    case 'G':
        return OIS::KC_G;
    case 'h':
    case 'H':
        return OIS::KC_H;
    case 'i':
    case 'I':
        return OIS::KC_I;
    case 'j':
    case 'J':
        return OIS::KC_J;
    case 'k':
    case 'K':
        return OIS::KC_K;
    case 'l':
    case 'L':
        return OIS::KC_L;
    case 'm':
    case 'M':
        return OIS::KC_M;
    case 'n':
    case 'N':
        return OIS::KC_N;
    case 'o':
    case 'O':
        return OIS::KC_O;
    case 'p':
    case 'P':
        return OIS::KC_P;
    case 'q':
    case 'Q':
        return OIS::KC_Q;
    case 'r':
    case 'R':
        return OIS::KC_R;
    case 's':
    case 'S':
        return OIS::KC_S;
    case 't':
    case 'T':
        return OIS::KC_T;
    case 'u':
    case 'U':
        return OIS::KC_U;
    case 'v':
    case 'V':
        return OIS::KC_V;
    case 'w':
    case 'W':
        return OIS::KC_W;
    case 'x':
    case 'X':
        return OIS::KC_X;
    case 'y':
    case 'Y':
        return OIS::KC_Y;
    case 'z':
    case 'Z':
        return OIS::KC_Z;
    default:return OIS::KC_UNASSIGNED;
    }
}

WORD CPUKeyboard::codeToKey(OIS::KeyCode code)
{
    switch(code)
    {
    case OIS::KC_SPACE:
        return ' ';
    case OIS::KC_RETURN:
        return 0x10;
    case OIS::KC_BACK:
        return 0x11;
    case OIS::KC_INSERT:
        return 0x12;
    case OIS::KC_DELETE:
        return 0x13;
    case OIS::KC_UP:
        return 0x80;
    case OIS::KC_DOWN:
        return 0x81;
    case OIS::KC_LEFT:
        return 0x82;
    case OIS::KC_RIGHT:
        return 0x83;
    case OIS::KC_GRAVE:
        return '`';
    case OIS::KC_MINUS:
        return '-';
    case OIS::KC_EQUALS:
        return '=';
    case OIS::KC_LBRACKET:
        return '[';
    case OIS::KC_RBRACKET:
        return ']';
    case OIS::KC_SEMICOLON:
        return ';';
    case OIS::KC_APOSTROPHE:
        return '\'';
    case OIS::KC_BACKSLASH:
        return '\\';
    case OIS::KC_COMMA:
        return ',';
    case OIS::KC_PERIOD:
        return '.';
    case OIS::KC_SLASH:
        return '/';
    case OIS::KC_0:
        return '0';
    case OIS::KC_1:
        return '1';
    case OIS::KC_2:
        return '2';
    case OIS::KC_3:
        return '3';
    case OIS::KC_4:
        return '4';
    case OIS::KC_5:
        return '5';
    case OIS::KC_6:
        return '6';
    case OIS::KC_7:
        return '7';
    case OIS::KC_8:
        return '8';
    case OIS::KC_9:
        return '9';
    case OIS::KC_A:
        return 'a';
    case OIS::KC_B:
        return 'b';
    case OIS::KC_C:
        return 'c';
    case OIS::KC_D:
        return 'd';
    case OIS::KC_E:
        return 'e';
    case OIS::KC_F:
        return 'f';
    case OIS::KC_G:
        return 'g';
    case OIS::KC_H:
        return 'h';
    case OIS::KC_I:
        return 'i';
    case OIS::KC_J:
        return 'j';
    case OIS::KC_K:
        return 'k';
    case OIS::KC_L:
        return 'l';
    case OIS::KC_M:
        return 'm';
    case OIS::KC_N:
        return 'n';
    case OIS::KC_O:
        return 'o';
    case OIS::KC_P:
        return 'p';
    case OIS::KC_Q:
        return 'q';
    case OIS::KC_R:
        return 'r';
    case OIS::KC_S:
        return 's';
    case OIS::KC_T:
        return 't';
    case OIS::KC_U:
        return 'u';
    case OIS::KC_V:
        return 'v';
    case OIS::KC_W:
        return 'w';
    case OIS::KC_X:
        return 'x';
    case OIS::KC_Y:
        return 'y';
    case OIS::KC_Z:
        return 'z';
    default:
        return 0;
    }
}
