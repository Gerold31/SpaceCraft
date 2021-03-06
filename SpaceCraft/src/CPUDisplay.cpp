#include "CPUDisplay.hpp"

#include "ENGINE.hpp"
#include "CPU.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreHardwarePixelBuffer.h"
#include "OGRE/OgreMaterial.h"

#define SIZE_X (0.02)
#define SIZE_Y (1.92)
#define SIZE_Z (2.56)

#define LIGHT_RANGE (10.0)

SpaceShipPart::SpaceShipPartInfo CPUDisplay::mPartInfo[] = {SpaceShipPartInfo(PART_WALL, Ogre::Vector3(0, 0, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

std::string CPUDisplay::mType = "CPU_Display";

CPUDisplay::CPUDisplay(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :Hardware(0x7349F615, 0x1C6C8B36, 0x1802, PART_WALLMOUNT, pos, ori, parent, name, mType, engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPUDisplay.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);

    commonConstructor();
}

CPUDisplay::CPUDisplay(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine)
    :Hardware(0x7349F615, 0x1C6C8B36, 0x1802, PART_WALLMOUNT, pos, ori, parent, staticGeometry, name, mType, engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPUDisplay.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
	//setupInstancedMaterialToEntity(mEntity);
    staticGeometry->addEntity(mEntity, pos, ori);

    commonConstructor();
}

void CPUDisplay::commonConstructor()
{
    for(int i=0; i<4; i++)
    {
        mLight[i] = mEngine->getSceneMgr()->createLight(mName + "Light" + Ogre::StringConverter::toString(i));
        mLight[i]->setPosition(-SIZE_X/4, (i/2)*SIZE_Y/2-SIZE_Y/4, (i%2)*SIZE_Z/2-SIZE_Z/4);
        mLight[i]->setType(Ogre::Light::LT_POINT);
        mLight[i]->setAttenuation(LIGHT_RANGE, 1.0, 4.5/LIGHT_RANGE, 75.0/(LIGHT_RANGE*LIGHT_RANGE));
        mLight[i]->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
        mNode->attachObject(mLight[i]);
    }

    Ogre::TextureManager& textureManager = Ogre::TextureManager::getSingleton();
	Ogre::String textureName = mName + "Texture";

    mTexture = textureManager.createManual(textureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D, 128, 96, 0,
        Ogre::PF_R8G8B8, Ogre::TU_DEFAULT);

    
    Ogre::MaterialManager& materialManager = Ogre::MaterialManager::getSingleton();
    Ogre::MaterialPtr material = materialManager.create(mName + "Material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            
    Ogre::Technique *technique = material->getTechnique(0);
    Ogre::Pass *pass = technique->getPass(0);
    Ogre::TextureUnitState* textureUnit = pass->createTextureUnitState();
    textureUnit->setTextureScale(-1, -1); // @todo fix this
	textureUnit->setTextureName(textureName);

    mEntity->setMaterial(material);

    mData = mPalette = mDefaultPalette = NULL;
    mFont = mDefaultFont= NULL;
    
    initData();
    initFont();
    initPalette();
    mFont = mDefaultFont;
    mPalette = mDefaultPalette;
    draw();

    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
}


CPUDisplay::~CPUDisplay()
{
    for(int i=0; i<4; i++)
    {
        mEngine->getSceneMgr()->destroyLight(mLight[i]);
        mLight[i] = 0;
    }
}

bool CPUDisplay::update(float elapsedTime)
{
    draw();
    return true;
}

void CPUDisplay::interrupt()
{
    switch(mCPU->getRegister(0))
    {
    case 0: // MEM_MAP_SCREEN
        {
            int b = mCPU->getRegister(1);
            if(b)
                mData = mCPU->getMemory() + b;
            else
                ;// turn off
            break;
        }
    case 1: // MEM_MAP_FONT
        {
            int b = mCPU->getRegister(1);
            if(b)
                mFont = (unsigned int*)(mCPU->getMemory()) + b;
            else
                mFont = mDefaultFont;
            break;
        }
    case 2: // MEM_NAP_PALETTE
        {
            int b = mCPU->getRegister(1);
            if(b)
                mPalette = mCPU->getMemory() + b;
            else
                mPalette = mDefaultPalette;
            break;
        }
    case 3: // SET_BORDER_COLOR
        {
            break;
        }
    case 4: // MEM_DUMP_FONT
        {
            int b = mCPU->getRegister(1);
            if(b)
            {
                mCPU->halt(256);
                for(int i=0; i<128; i++)
                {
                    mCPU->getMemory()[i*2+b]   = (mDefaultFont[i] >> 16) & 0xFFFF;
                    mCPU->getMemory()[i*2+b+1] = (mDefaultFont[i] >>  0) & 0xFFFF;
                }
            }
            break;
        }
    case 5: // MEM_DUMP_PALETTE
        {
            int b = mCPU->getRegister(1);
            if(b)
            {
                mCPU->halt(16);
                for(int i=0; i<16; i++)
                {
                    mCPU->getMemory()[i+b]   = mDefaultPalette[i];
                }
            }
            break;
        }
    default:
        break;
    }
}

void CPUDisplay::initData()
{
    if(!mData)
        mData = new unsigned short[32*12];
    for(int i=0; i<32; i++)
        for(int j=0; j<12; j++)
            mData[i + 32*j] = 0x20 + ((i/16 + j/6*2) << 8);
            //mData[i + 32*j] = 0x20 + (i/2 << 8);
}
void CPUDisplay::initPalette()
{
    if(!mDefaultPalette)
        mDefaultPalette = new unsigned short[16];
    mDefaultPalette[0x0] = 0x0000;
    mDefaultPalette[0x1] = 0x000A;
    mDefaultPalette[0x2] = 0x00A0;
    mDefaultPalette[0x3] = 0x00AA;
    mDefaultPalette[0x4] = 0x0A00;
    mDefaultPalette[0x5] = 0x0A0A;
    mDefaultPalette[0x6] = 0x0A50;
    mDefaultPalette[0x7] = 0x0AAA;
    mDefaultPalette[0x8] = 0x0555;
    mDefaultPalette[0x9] = 0x055F;
    mDefaultPalette[0xA] = 0x05F5;
    mDefaultPalette[0xB] = 0x05FF;
    mDefaultPalette[0xC] = 0x0F55;
    mDefaultPalette[0xD] = 0x0F5F;
    mDefaultPalette[0xE] = 0x0FF5;
    mDefaultPalette[0xF] = 0x0FFF;
}
void CPUDisplay::initFont()
{
    if(!mDefaultFont)
        mDefaultFont = new unsigned int[128];
	std::ifstream ifs("font.png", std::ios::binary|std::ios::in);
	if (ifs.is_open())
	{
		Ogre::DataStreamPtr data_stream(new Ogre::FileStreamDataStream("font.png", &ifs, false));
		Ogre::Image img;
		img.load(data_stream, "png");
		Ogre::TextureManager::getSingleton().loadImage("font.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, img, Ogre::TEX_TYPE_2D, 0, 1.0f);

        for(size_t x=0; x<img.getWidth(); x+=4)
        {
            for(size_t y=0; y<img.getHeight(); y+=8)
            {
                int letter = 0;
                for(int i=0; i<4; i++)
                {
                    for(int j=0; j<8; j++)
                    {
                        letter |= (img.getColourAt(x+i, y+j, 0) == Ogre::ColourValue(1,1,1) ? 1 : 0) << (32 - i + j*4);
                    }
                }
                mDefaultFont[x/4 + 32*y/8] = letter;
            }
        }
    }
}

void CPUDisplay::draw()
{
    Ogre::Image::Box pixelBox;
        
    //Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mTexture->getBuffer();

    //pixelBuffer->lock(pixelBox,Ogre::HardwareBuffer::HBL_NORMAL);

    Ogre::uint8 *data = new Ogre::uint8[128*96*4]();
        
    const Ogre::PixelBox &pixBox = Ogre::PixelBox(128,96,0,Ogre::PF_R8G8B8, data);// pixelBuffer->getCurrentLock();

    Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixBox.data);

    int lightColor[4][3] = {0};
    
    for(int y=0; y<96; y++)
    {
        for(int x=0; x<128; x++)
        {
            int data = *(mData + x/4 + 32*(y/8));
            int color = mFont[data & 0x7F] & (1 << (32 - x%4 + 4*(y%8))) ? mPalette[(data >> 12) & 0xF] : mPalette[(data >> 8) & 0xF];

            int b = (((color >> 0) & 0xF) << 4) | (color >> 0) & 0xF;
            int g = (((color >> 4) & 0xF) << 4) | (color >> 4) & 0xF;
            int r = (((color >> 8) & 0xF) << 4) | (color >> 8) & 0xF;

            int i = (x>128/2?1:0) + (y<96/2?2:0);
            lightColor[i][0] += r;
            lightColor[i][1] += g;
            lightColor[i][2] += b;
                                    
            *pDest++ = b;
            *pDest++ = g;
            *pDest++ = r;
            *pDest++ = 0;
        }
    } 
    //pixelBuffer->unlock();

    Ogre::uchar* pData = static_cast<Ogre::uchar*>(pixBox.data);
    Ogre::Image img;
    img.loadDynamicImage(pData, mTexture->getWidth(), mTexture->getHeight(), mTexture->getDepth() ,mTexture->getFormat());

    //img.save("screen.png");

    mTexture->getBuffer()->blitFromMemory(img.getPixelBox());

    for(int i=0; i<4; i++)
    {
        Ogre::ColourValue c(lightColor[i][0]/(96*128/4*256.0),lightColor[i][1]/(96*128/4*256.0),lightColor[i][2]/(96*128/4*256.0));
        mLight[i]->setDiffuseColour(c);
        mLight[i]->setSpecularColour(c);
    }
    
    delete[] data;
}

void CPUDisplay::setMaterial()
{
    SpaceShipPart::setMaterial("Base/Black");
}