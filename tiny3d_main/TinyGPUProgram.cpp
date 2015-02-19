//
//  TinyGPUProgram.cpp
//  proj_mac
//
//  Created by reuben chao on 2/3/15.
//  Copyright (c) 2015 reuben chao. All rights reserved.
//

#include "TinyGPUProgram.h"
#include "TinyMemoryAlloc.h"
#include "TinyPlatform.h"

namespace Tiny
{
    TinyGPUProgram::TinyGPUProgram(std::string& vs, std::string& fs)
        : mParams(TINY_NEW TinyGPUProgramParameters()),
        , mHandler(0)
        , mVertexShaderSource(vs);
        , mFragmentShaderSource(fs);
    {
        //
    }
    
    TinyGPUProgram::~TinyGPUProgram()
    {
        TINY_DELETE mParams;
    }
    
    void TinyGPUProgram::load()
    {
        
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        GLint result = GL_FALSE;
        int infoLogLength;
        char *shaderInfoMessage = nullptr;
        char *programInfoMessage = nullptr;
        
        // Compile Vertex Shader
        glShaderSource(vertexShader, 1, &mVertexShaderSource , NULL);
        glCompileShader(vertexShader);
        
        // Check Vertex Shader
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            shaderInfoMessage = (char *)malloc(infoLogLength + 1);
            glGetShaderInfoLog(vertexShader, infoLogLength, NULL, shaderInfoMessage);
            TINYLOG("vertex shder info: %s", shaderInfoMessage);
            free(shaderInfoMessage);
        }
        
        // Compile Fragment Shader
        glShaderSource(fragmentShader, 1, &mFragmentShaderSource , NULL);
        glCompileShader(fragmentShader);
        
        // Check Fragment Shader
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            shaderInfoMessage = (char *)malloc(infoLogLength + 1);
            glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, shaderInfoMessage);
            TINYLOG("fragment shder info: %s", shaderInfoMessage);
            free(shaderInfoMessage);
        }
        
        // Link the program
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        
        // Check the program
        glGetProgramiv(program, GL_LINK_STATUS, &result);
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            programInfoMessage = (char *)malloc(infoLogLength + 1);
            glGetProgramInfoLog(program, infoLogLength, NULL, programInfoMessage);
            TINYLOG("program info: %s", programInfoMessage);
            free(programInfoMessage);
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        mHandler = program;
    }
    
    void TinyGPUProgram::useProgram()
    {
        glUseProgram(mHandler);
        mParams->bindParametersToProgram(mHandler);
    }
    
    void TinyGPUProgram::setParameter(const TinyGPUProgramParameter& value)
    {
        mParams->setParameter(value);
    }
    
    GLuint TinyGPUProgram::getHandler()
    {
        return mHandler;
    }
    
    TinyGPUProgramParameters::TinyGPUProgramParameters()
    {
        //
    }
    
    TinyGPUProgramParameters::~TinyGPUProgramParameters()
    {
        //
    }
    
    void TinyGPUProgramParameters::setParameter(const TinyGPUProgramParameter& value)
    {
        mParams[value.mName] = value;
    }
    
    void TinyGPUProgramParameters::bindParametersToProgram(GLuint program)
    {
        auto iter = mParams.begin();
        uint8 textureCnter = 0;
        for (; iter != mParams.end(); iter ++)
        {
            iter->second.calcLocation(program);
            GLint location = iter->second.mLocation;
            switch (iter->second.mType)
            {
                case GP_FLOAT1:
                {
                    glUniform1fv(location, 1, iter->second.mBindData);
                    break;
                }
                case GP_FLOAT2:
                {
                    glUniform2fv(location, 1, iter->second.mBindData);
                    break;
                }
                case GP_FLOAT3:
                {
                    glUniform3fv(location, 1, iter->second.mBindData);
                    break;
                }
                case GP_FLOAT4:
                {
                    glUniform14v(location, 1, iter->second.mBindData);
                    break;
                }
                case GP_INT1:
                {
                    glUniform1iv(location, 1, iter->second.mBindData);
                    break;
                }
                case GP_INT2:
                {
                    glUniform2iv(location, 1, iter->second.mBindData);
                    break;
                }
                case GP_INT3:
                {
                    glUniform3iv(location, 1, iter->second.mBindData);
                    break;
                }
                case GP_INT4:
                {
                    glUniform4iv(location, 1, iter->second.mBindData);
                    break;
                }
                case GP_MATRIX_2X2:
                {
                    glUniformMatrix2fv(location, 1, GL_FALSE, iter->second.mBindData);
                    break;
                }
                case GP_MATRIX_3X3:
                {
                    glUniformMatrix3fv(location, 1, GL_FALSE, iter->second.mBindData);
                    break;
                }
                case GP_MATRIX_4X4:
                {
                    glUniformMatrix4fv(location, 1, GL_FALSE, iter->second.mBindData);
                    break;
                }
                case GP_SAMPLER:
                {
                    glActiveTexture(GL_TEXTURE0 + textureCnter);
                    glEnable(GL_TEXTURE_MAP);
                    glBindTexture(GL_TEXTURE_MAP, cubeTexture);
                    glUniform1i(location, textureCnter);
                    ++ textureCnter;
                    break;
                }
                case GP_SAMPLERCUBE:
                {
                    glActiveTexture(GL_TEXTURE0 + textureCnter);
                    glEnable(GL_TEXTURE_CUBE_MAP);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
                    glUniform1i(location, textureCnter);
                    ++ textureCnter;
                    break;
                }
                default:
                    break;
            }
        }
    }
    
    void TinyGPUProgramParameter::calcLocation(GLuint program)
    {
        mLocation = glGetUniformLocation(program, mName);
    }
}




