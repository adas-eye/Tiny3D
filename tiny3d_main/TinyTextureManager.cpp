//
//  TinyTextureController.cpp
//  proj_mac
//
//  Created by reuben chao on 2/3/15.
//  Copyright (c) 2015 reuben chao. All rights reserved.
//

#include "TinyTextureManager.h"
#include "TinyMemoryAlloc.h"

namespace Tiny
{
    TinyTexture* TinyTextureManager::load(std::string& fileName)
    {
        uint8* data;
        uint32 length;
        getFileData(fileName, &data, length);
        TinyTexture* texture = TINY_NEW TinyTexture();
        texture->load(data, length);
        free(data);
        return texture;
    }
    
    TinyTexture* load(std::vector<std::string>& fileNames)
    {
        std::vector<uint8*> datas;
        std::vector<uint32> lengths;
        uint32 length = fileNames.size();
        for (int i = 0; i < length; i ++)
        {
            uint8* data;
            uint32 length;
            getFileData(fileNames[i], &data, length);
            datas.push_back(data);
            lengths.push_back(length);
        }
        TinyTexture* texture = TINY_NEW TinyTexture();
        texture->load(datas, lengths);
        for (int i = 0; i < length; i ++)
        {
            free(datas[i])
        }
        return texture;
    }
}
