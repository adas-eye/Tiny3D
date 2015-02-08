//
//  TinyNode.cpp
//  proj_mac
//
//  Created by reuben chao on 2/8/15.
//  Copyright (c) 2015 reuben chao. All rights reserved.
//

#include "TinyNode.h"
#include "TinyMemoryAlloc.h"

namespace Tiny
{
    TinyNode::TinyNode(std::string &name)
        : mPosition(0, 0, 0)
        , mDerivedPosition(0, 0, 0)
        , mOrientation(0, 0, 0, 1)
        , mDerivedOrientation(0, 0, 0, 1)
        , mScale(0, 0, 0)
        , mDerivedScale(0, 0, 0)
        , mParent(nullptr)
        , mName(name)
    {
        
    }
    
    TinyNode::~TinyNode()
    {
        
    }
    
    TinyNode* TinyNode::createChild()
    {
        auto child = TINY_NEW TinyNode();
        addChild(node);
        return node;
    }
    
    void TinyNode::addChild(TinyNode *node)
    {
        if (node->getParent())
        {
            assert(result.second && "Child was not added because it has already added to a parent");
        }
        else
        {
            auto result = mChildren->insert(std::pair<std::string, TinyNode>(node->getName(), node));
            assert(result.second && "Child was not added because of a doublicate name.");
            node->setParent(this);
        }
    }
    
    void TinyNode::removeChild(TinyNode *node)
    {
        mChildren->erase(node->getname());
        node->setParent(nullptr);
    }
    
    void TinyNode::getChild(std::string& name)
    {
        TinyNode* ret = nullptr;
        auto iter = mChildren->find(name);
        if (iter != mChildren->end())
        {
            ret = iter->second;
        }
        return ret;
    }
    
    void TinyNode::updateFromParent()
    {
        if (mParent)
        {
            auto parentDerivedPosition = mParent->getDerivedPosition();
            mDerivedPosition = parentDerivedPosition + mPosition;
            
            auto parentDerivedOrientation = mParent->getDerivedOrientation();
            kmQuaternionMultiply(&mDerivedOrientation, &parentDerivedOrientation, &mOrientation);

            auto parentDerivedScale = mParent->getScale();
            kmVec3Dot(&mDerivedScale, &parentDerivedScale, &mScale);
        }
        else
        {
            mDerivedPosition = mPosition;
            mDerivedOrientation = mOrientation;
            mDerivedScale = mScale;
        }
    }

    void TinyNode::update()
    {
        updateFromParent();
        auto iter = mChildren->begin();
        for (; iter != mChildren->end(); iter ++)
        {
            iter->second->update();
        }
    }
    
    void TinyNode::setPosition(kmVec3 &pos)
    {
        kmVec3Fill(&mPosition, pos.x, pos.y, pos.z);
    }
    
    void TinyNode::setScale(kmVec3 &scale)
    {
        kmVec3Fill(&mScale, scale.x, scale.y, scale.z);
    }
    
    void TinyNode::setOrientation(kmVec3 &orientation)
    {
        kmQuaternionRotationYawPitchRoll(&mOrientation, orientation.z, orientation.y, orientation.x);
    }
    
    kmVec3& TinyNode::getPosition()
    {
        return mPosition;
    }
    
    kmVec3& TinyNode::getScale()
    {
        return mScale;
    }
    
    kmVec3& TinyNode::getOrientation()
    {
        return mOrientation;
    }
    
    kmVec3& TinyNode::getDerivedPosition()
    {
        return mDerivedPosition;
    }
    kmVec3& TinyNode::getDerivedScale()
    {
        return mDerivedScale;
    }
    
    kmQuaternion& TinyNode::getDerivedOrientation()
    {
        return mDerivedOrientation;
    }
    
    std::string& TinyNode::getName()
    {
        return mName;
    }
    
    TinyNode* TinyNode::getParent()
    {
        return mParent;
    }
    
    void TinyNode::setParent(TinyNode* parent)
    {
        mParent = parent;
    }
}




