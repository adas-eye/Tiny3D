//
//  TinyImage.cpp
//  proj_mac
//
//  Created by reuben chao on 2/19/15.
//  Copyright (c) 2015 reuben chao. All rights reserved.
//

#include <stdio.h>
#include "TinyImage"

namespace Tiny
{
    TinyImage::TinyImage()
        : mIsMipmap(true)
    {
        
    }
    
    TinyImage::~TinyImage()
    {
        auto iter = mDataList.begin();
        for (; iter != mDataList.end(); iter ++)
        {
            free(iter->second);
        }
    }
}