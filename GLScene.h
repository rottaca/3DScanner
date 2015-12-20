/**
 * Project 3DScanner
 */


#ifndef _GLSCENE_H
#define _GLSCENE_H

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qopenglwidget.h>

#include <QtGui/qopenglvertexarrayobject.h>
#include <QtGui/qopenglfunctions.h>
#include <QtGui/qopenglbuffer.h>
#include <QtGui/qopenglshaderprogram.h>
#include <QtGui/qevent.h>
#include "GLCamera.h"
#include "Drawable.h"

#include <vector>

class GLScene : public QOpenGLWidget{
    Q_OBJECT
public: 
	GLScene(QWidget* parent = NULL);
	~GLScene();
    /**
     * @param obj
     */
    void addDrawable(Drawable* obj);
    
    void removeDrawables();

    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
	void signalError(std::string err);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private: 
    GLCamera m_cam;
    std::vector<Drawable*> m_drawableObj;

	QVector<QVector4D> m_groundVertices;
    QOpenGLVertexArrayObject m_vaoGround;
    QOpenGLBuffer m_vboGround;

    QOpenGLShaderProgram m_shaderProg;

    int m_attrPos, m_attrCol, m_uniMVP, m_uniModelMatrix;

    QPointF m_oldMousePos;
    bool isPressedML;
    bool isPressedMR;
    bool isPressedMM;

};

#endif //_GLSCENE_H
