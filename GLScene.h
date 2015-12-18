/**
 * Project 3DScanner
 */


#ifndef _GLSCENE_H
#define _GLSCENE_H

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qopenglwidget.h>

#include "GLCamera.h"
#include "Drawable.h"

#include <vector>

class GLScene : public QOpenGLWidget{
public: 
    
	GLScene(QWidget* parent = NULL);
	~GLScene();
    /**
     * @param obj
     */
    void addDrawable(Drawable obj);
    
    void removeDrawables();
    
    void renderScene();
    
signals:
	void signalError(QString err);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
private: 
    GLCamera m_cam;
    std::vector<Drawable> m_drawableObj;

    GLuint m_vao_ground, m_vbo_ground;

};

#endif //_GLSCENE_H
