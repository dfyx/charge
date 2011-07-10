#include "objectloader.h"
#include <QList>
#include <QFile>

namespace Charge
{
    ObjectLoader::ObjectLoader()
    {
    }

    void ObjectLoader::loadObject(QString filename, GLuint vertexBuffer, GLuint normalBuffer, GLuint texCoordBuffer, GLuint indexBuffer) const
    {
        QList<QVector3D> vertices, normals;
        QList<QVector2D> texCoords;
        QList<QList<unsigned int> > faces;

        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly))
        {
            return;
        }

        // Parse file
        while(file.peek(1).size() > 0)
        {
            QStringList fields = QString(file.readLine()).split(' ');
            if(fields.size() == 0)
            {
                // Ignore empty lines
                continue;
            }

            if(fields[0] == "v")
            {
                vertices.push_back(loadVertex(fields));
            }
            else if(fields[0] == "vt")
            {
                texCoords.push_back(loadTextureCoords(fields));
            }
            else if(fields[0] == "vn")
            {
                normals.push_back(loadNormal(fields));
            }
            else if(fields[0] == "f")
            {
                faces.push_back(loadFace(fields));
            }
        }
        file.close();

        // Fill OpenGL Arrays. Unfortunately, some serious reordering is necessary because of
        // The way .obj handles its indices
        GLuint *vertexArray = new GLuint[faces.size() * 9];
        GLuint *normalArray = new GLuint[faces.size() * 9];
        GLuint *texCoordArray = new GLuint[faces.size() * 6];
        GLuint *indexArray = new GLuint[faces.size() * 3];

        unsigned int offset = 0;
        QList<QList<unsigned int> >::iterator iter;
        for(iter = faces.begin(); iter != faces.end(); iter++)
        {
            indexArray[3 * offset + 0] = offset + 0;
            indexArray[3 * offset + 1] = offset + 2;
            indexArray[3 * offset + 2] = offset + 2;

            for(int i = 0; i < 3; i++)
            {
                QVector3D vertex = vertices[(*iter)[3 * i] - 1];
                vertexArray[3 * offset + 0] = vertex.x();
                vertexArray[3 * offset + 1] = vertex.y();
                vertexArray[3 * offset + 2] = vertex.z();

                QVector2D texCoord = texCoords[(*iter)[3 * i + 1] - 1];
                texCoordArray[2 * offset + 0] = texCoord.x();
                texCoordArray[2 * offset + 1] = texCoord.y();

                QVector3D normal = normals[(*iter)[3 * i + 2] - 1];
                normalArray[3 * offset + 0] = normal.x();
                normalArray[3 * offset + 1] = normal.y();
                normalArray[3 * offset + 2] = normal.z();
            }

            offset++;
        }

        glBindBuffer(GL_ARRAY_BUFFER_ARB, vertexBuffer);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, faces.size() * 9 * sizeof(GLfloat), vertexArray, GL_STATIC_DRAW_ARB);

        glBindBuffer(GL_ARRAY_BUFFER_ARB, texCoordBuffer);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, faces.size() * 6 * sizeof(GLfloat), texCoordArray, GL_STATIC_DRAW_ARB);

        glBindBuffer(GL_ARRAY_BUFFER_ARB, normalBuffer);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, faces.size() * 9 * sizeof(GLfloat), normalArray, GL_STATIC_DRAW_ARB);

        glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer);
        glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, faces.size() * 3 * sizeof(GLuint), indexArray, GL_STATIC_DRAW_ARB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

        delete vertexArray;
        delete normalArray;
        delete texCoordArray;
        delete indexArray;
    }

    const QVector3D ObjectLoader::loadVertex(QStringList fields) const
    {
        QVector3D result;

        result.setX(fields[1].toDouble());
        result.setY(fields[2].toDouble());
        result.setZ(fields[3].toDouble());

        if(fields.size() > 4)
        {
            result /= fields[4].toDouble();
        }

        return result;
    }

    const QVector2D ObjectLoader::loadTextureCoords(QStringList fields) const
    {
        QVector2D result;

        result.setX(fields[1].toDouble());
        result.setY(fields[2].toDouble());

        // Ignore third component

        return result;
    }

    const QVector3D ObjectLoader::loadNormal(QStringList fields) const
    {
        QVector3D result;

        result.setX(fields[1].toDouble());
        result.setY(fields[2].toDouble());
        result.setZ(fields[3].toDouble());

        return result;
    }

    const QList<unsigned int> ObjectLoader::loadFace(QStringList fields) const
    {
        QList<unsigned int> result;

        for(int i = 1; i <= 3; i++)
        {
            QStringList indices = fields[i].split('/');
            for(int j = 0; j < 3; j++)
            {
                result.push_back(indices[j].toUInt());
            }
        }

        return result;
    }
}
