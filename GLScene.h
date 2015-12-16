/**
 * Project 3DScanner
 */


#ifndef _GLSCENE_H
#define _GLSCENE_H

#include <QtWidgets/qwidget.h>

#include "GLCamera.h"
#include "Drawable.h"

#include <vector>

class GLScene {
public: 
    
    /**
     * @param obj
     */
    void addDrawable(Drawable obj);
    
    void removeDrawables();
    
    void renderScene();
    
    /**
     * @param w
     * @param h
     */
    void onResize(int w, int h);
private: 
    GLCamera m_cam;
    std::vector<Drawable> m_drawableObj;
};

#endif //_GLSCENE_H
