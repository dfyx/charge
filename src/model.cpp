#include "model.h"
#include "objectloader.h"

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
    }

    void Model::draw()
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

        // Draw
        glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);


        // Reset states
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

        glPopClientAttrib();
    }
}
