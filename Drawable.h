/**
 * Project 3DScanner
 */


#ifndef _DRAWABLE_H
#define _DRAWABLE_H

#include <QtWidgets/qopenglwidget.h>
#include <QtGui/qopenglvertexarrayobject.h>
#include <QtGui/qopenglfunctions.h>
#include <QtGui/qopenglbuffer.h>
#include <QtGui/qopenglshaderprogram.h>
#include <QtGui/qvector4d.h>

class Drawable {

public: 
    Drawable(){}
    virtual ~Drawable(){}

    virtual bool createBuffers(int attrPos, int attrCol)=0;
    
    virtual void drawData()=0;
    
    // TODO
//    mat3 getModelMatrix();

protected:
	QVector<QVector4D> m_vert;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
};

#endif //_DRAWABLE_H
