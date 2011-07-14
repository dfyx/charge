#include "model.h"
#include "objectloader.h"
#include <QImage>

namespace Charge
{
    Model::Model(QString filename)
    {
        this->filename = filename;

        if(cache.contains(filename))
        {
            cache[filename].count++;
            vertexBuffer = cache[filename].vertexBuffer;
            normalBuffer = cache[filename].normalBuffer;
            texCoordBuffer = cache[filename].texCoordBuffer;
            indexBuffer = cache[filename].indexBuffer;
            numFaces = cache[filename].numFaces;
        }
        else
        {
            ObjectLoader loader;

            glGenBuffersARB(1, &vertexBuffer);
            glGenBuffersARB(1, &normalBuffer);
            glGenBuffersARB(1, &texCoordBuffer);
            glGenBuffersARB(1, &indexBuffer);

            numFaces = loader.loadObject(filename, vertexBuffer, normalBuffer, texCoordBuffer, indexBuffer);

            cache[filename].count = 1;
            cache[filename].vertexBuffer = vertexBuffer;
            cache[filename].normalBuffer = normalBuffer;
            cache[filename].texCoordBuffer = texCoordBuffer;
            cache[filename].indexBuffer = indexBuffer;
            cache[filename].numFaces = numFaces;
        }
    }

    Model::~Model()
    {
        cache[filename].count--;
        if(cache[filename].count == 0)
        {
            cache.remove(filename);
            glDeleteBuffersARB(1, &vertexBuffer);
            glDeleteBuffersARB(1, &normalBuffer);
            glDeleteBuffersARB(1, &texCoordBuffer);
            glDeleteBuffersARB(1, &indexBuffer);
        }

        QMap<QString, GLuint>::iterator iter;
        for(iter = textures.begin(); iter != textures.end(); iter++)
        {
            GLuint id = iter.value();
            glDeleteTextures(1, &id);
        }
    }

    void Model::loadTexture(QString texname, QString filename)
    {
        const QImage image(filename);

        if(image.isNull())
        {
            return;
        }

        if(!textures.contains(texname))
        {
            GLuint id;
            glGenTextures(1, &id);
            textures[texname] = id;
        }

        glBindTexture(GL_TEXTURE_2D, textures[texname]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    }

    void Model::draw(QGLShaderProgram *shaderProgram)
    {
        // Prepare
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

        glEnableClientState(GL_VERTEX_ARRAY);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexBuffer);
        glVertexPointer(3, GL_FLOAT, 0, 0);

        glEnableClientState(GL_NORMAL_ARRAY);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalBuffer);
        glNormalPointer(GL_FLOAT, 0, 0);

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, texCoordBuffer);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);

        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer);

        // Enable textures
        unsigned int num = 0;
        QMap<QString, GLuint>::iterator iter;
        for(iter = textures.begin(); iter != textures.end(); iter++)
        {
            GLuint id = iter.value();
            QString texname = iter.key();
            glActiveTexture(GL_TEXTURE0 + num);
            glBindTexture(GL_TEXTURE_2D, id);
            if(shaderProgram)
            {
                shaderProgram->setUniformValue((const char*) texname.data(), num);
            }
            num++;
        }

        // Draw
        glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);

        // Reset states
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

        for(int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + num);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glPopClientAttrib();
    }
}
