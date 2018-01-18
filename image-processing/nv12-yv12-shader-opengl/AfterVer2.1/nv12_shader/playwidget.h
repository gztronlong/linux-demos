#ifndef GLPLAYWIDGET_H
#define GLPLAYWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QFile>
#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4
class CPlayWidget:public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    CPlayWidget(int widget, int height, const char* pathname, QWidget* parent = 0);
    ~CPlayWidget();
    void PlayOneFrame();
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
private:
    GLuint textureUniformY; //y纹理数据位置
    GLuint textureUniformUV; //u纹理数据位置
    GLuint id_y; //y纹理对象ID
    GLuint id_uv; //uv纹理对象ID
    QOpenGLTexture* m_pTextureY;  //y纹理对象
    QOpenGLTexture* m_pTextureUV;  //uv纹理对象
    QOpenGLShader *m_pVSHader;  //顶点着色器程序对象
    QOpenGLShader *m_pFSHader;  //片段着色器对象
    QOpenGLShaderProgram *m_pShaderProgram; //着色器程序容器
    const char* path;
    int m_nVideoW; //视频分辨率宽
    int m_nVideoH; //视频分辨率高
    unsigned char* m_pBufYuv420p;
    FILE* m_pYuvFile;
};
#endif
