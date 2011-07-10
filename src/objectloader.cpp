#include "objectloader.h"
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

namespace Charge
{
    ObjectLoader::ObjectLoader()
    {
    }

    unsigned int ObjectLoader::loadObject(QString filename, GLuint vertexBuffer, GLuint normalBuffer, GLuint texCoordBuffer, GLuint indexBuffer) const
    {
        QList<QVector3D> vertices, normals;
        QList<QVector2D> texCoords;
        QList<QList<unsigned int> > faces;

        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly))
        {
            return 0;
        }

        QTextStream stream(&file);

        // Parse file
        while(!stream.atEnd())
        {
            QString line = stream.readLine();
            QStringList fields = line.split(QRegExp("\\s+"));
            if(fields.size() == 0)
            {
                // Ignore empty lines
            }
            else if(fields[0] == "v")
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
        GLfloat *vertexArray = new GLfloat[faces.size() * 9];
        GLfloat *normalArray = new GLfloat[faces.size() * 9];
        GLfloat *texCoordArray = new GLfloat[faces.size() * 6];
        GLuint *indexArray = new GLuint[faces.size() * 3];

        unsigned int offset = 0;
        QList<QList<unsigned int> >::iterator iter;
        for(iter = faces.begin(); iter != faces.end(); iter++)
        {
            indexArray[3 * offset + 0] = 3 * offset + 0;
            indexArray[3 * offset + 1] = 3 * offset + 1;
            indexArray[3 * offset + 2] = 3 * offset + 2;

            for(int i = 0; i < 3; i++)
            {
                QVector3D vertex = vertices[(*iter)[3 * i] - 1];
                vertexArray[3 * (3 * offset + i) + 0] = vertex.x();
                vertexArray[3 * (3 * offset + i) + 1] = vertex.y();
                vertexArray[3 * (3 * offset + i) + 2] = vertex.z();

                QVector2D texCoord = texCoords[(*iter)[3 * i + 1] - 1];
                texCoordArray[2 * (3 * offset + i) + 0] = texCoord.x();
                texCoordArray[2 * (3 * offset + i) + 1] = texCoord.y();

                QVector3D normal = normals[(*iter)[3 * i + 2] - 1];
                normalArray[3 * (3 * offset + i) + 0] = normal.x();
                normalArray[3 * (3 * offset + i) + 1] = normal.y();
                normalArray[3 * (3 * offset + i) + 2] = normal.z();
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

        return faces.size();
    }

    const QVector3D ObjectLoader::loadVertex(QStringList fields) const
    {
        float x = fields[1].toFloat();
        float y = fields[2].toFloat();
        float z = fields[3].toFloat();

        if(fields.size() > 4 && fields[4].length() > 0)
        {
            float w = fields[4].toFloat();
            x /= w;
            y /= w;
            z /= w;
        }

        return QVector3D(x, y, z);
    }

    const QVector2D ObjectLoader::loadTextureCoords(QStringList fields) const
    {
        float x = fields[1].toFloat();
        float y = fields[2].toFloat();
        // Ignore third component

        return QVector2D(x, y);
    }

    const QVector3D ObjectLoader::loadNormal(QStringList fields) const
    {
        float x = fields[1].toFloat();
        float y = fields[2].toFloat();
        float z = fields[3].toFloat();

        return QVector3D(x, y, z);
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
