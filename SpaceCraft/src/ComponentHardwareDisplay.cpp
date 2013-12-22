#include "ComponentHardwareDisplay.hpp"

#include "TypeInfo.hpp"
#include "ComponentCPU.hpp"
#include "SystemLog.hpp"
#include "SystemGraphics.hpp"
#include "Object.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreHardwarePixelBuffer.h"
#include "OGRE/OgreMaterial.h"

using namespace SpaceCraft;

TypeInfo *ComponentHardwareDisplay::mType = new TypeInfo("ComponentHardwareDisplay", &ComponentHardwareDisplay::createInstance);

ComponentHardwareDisplay::ComponentHardwareDisplay(Object *object, ParamMap &params) :
    ComponentHardware(object, params, mType, 0x7349F615, 0x1C6C8B36, 0x1802)
{
	LOG_IN("hardware");
	LOG_OUT("hardware");
}

ComponentHardwareDisplay::~ComponentHardwareDisplay()
{
	LOG_IN("hardware");
	LOG_OUT("hardware");
}

void *ComponentHardwareDisplay::createInstance(Object *object, ParamMap &params)
{
	LOG_IN("hardware");
	LOG_OUT("hardware");
    return new ComponentHardwareDisplay(object, params);
}

void ComponentHardwareDisplay::init()
{
	LOG_IN("hardware");

    Ogre::TextureManager& textureManager = Ogre::TextureManager::getSingleton();
	Ogre::String textureName = mObject->getName() + "Texture";

    mTexture = textureManager.createManual(textureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D, 128, 96, 0,
        Ogre::PF_R8G8B8, Ogre::TU_DEFAULT);

    
    Ogre::MaterialManager& materialManager = Ogre::MaterialManager::getSingleton();
    Ogre::MaterialPtr material = materialManager.create(mObject->getName() + "Material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            
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
	LOG_OUT("hardware");
}
    
void ComponentHardwareDisplay::update(float elapsedTime)
{
	LOG_IN_FRAME;
    draw();
	LOG_OUT_FRAME;
}

void ComponentHardwareDisplay::receiveMessage(Message *message)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentHardwareDisplay::interrupt()
{
	LOG_IN("hardware");
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
	LOG_OUT("hardware");
}

void ComponentHardwareDisplay::initData()
{
	LOG_IN("hardware");
    if(!mData)
        mData = new unsigned short[32*12];
    for(int i=0; i<32; i++)
        for(int j=0; j<12; j++)
            mData[i + 32*j] = 0x20 + ((i/16 + j/6*2) << 8);
            //mData[i + 32*j] = 0x20 + (i/2 << 8);
	LOG_OUT("hardware");
}

void ComponentHardwareDisplay::initPalette()
{
	LOG_IN("hardware");
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
	LOG_OUT("hardware");
}
void ComponentHardwareDisplay::initFont()
{
	LOG_IN("hardware");
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
	LOG_OUT("hardware");
}

void ComponentHardwareDisplay::draw()
{
	LOG_IN_FRAME;
    Ogre::Image::Box pixelBox;
        
    //Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mTexture->getBuffer();

    //pixelBuffer->lock(pixelBox,Ogre::HardwareBuffer::HBL_NORMAL);

    Ogre::uint8 *data = new Ogre::uint8[128*96*4]();
        
    const Ogre::PixelBox &pixBox = Ogre::PixelBox(128,96,0,Ogre::PF_R8G8B8, data);// pixelBuffer->getCurrentLock();

    Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixBox.data);
    
    for(int y=0; y<96; y++)
    {
        for(int x=0; x<128; x++)
        {
            int data = *(mData + x/4 + 32*(y/8));
            int color = mFont[data & 0x7F] & (1 << (32 - x%4 + 4*(y%8))) ? mPalette[(data >> 12) & 0xF] : mPalette[(data >> 8) & 0xF];

            int b = (((color >> 0) & 0xF) << 4) | (color >> 0) & 0xF;
            int g = (((color >> 4) & 0xF) << 4) | (color >> 4) & 0xF;
            int r = (((color >> 8) & 0xF) << 4) | (color >> 8) & 0xF;
                                    
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
    
    delete[] data;
	LOG_OUT_FRAME;
}