#include <gl_widget.h>

#include <QtGui>
#include <QtOpenGL>

Gl_widget::Gl_widget(int width, int height, const QString& path, QWidget* parent)
    :QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
    , video_width(width)
    , video_height(height)
    , y_texture(0)
    , uv_texture(0)
    , y_data(0)
    , uv_data(0)
    , file(path)
    , timer(this)
    , program(this)
{
    setFocusPolicy(Qt::StrongFocus);

    y_data = new unsigned char[(width*height*3)>>1];
    uv_data = y_data+(width*height);
    file.open(QIODevice::ReadOnly);
}

Gl_widget::~Gl_widget()
{
    file.close();

    glDeleteTextures(1, &y_texture);
    glDeleteTextures(1, &uv_texture);

    uv_data = 0;
    delete [] y_data;
    y_data = 0;
}
void Gl_widget::initializeGL()
{
    qDebug() << program.addShaderFromSourceCode(QGLShader::Fragment,
     "uniform sampler2D y_texture;\n"
     "uniform sampler2D uv_texture;\n"
     "void main(void)\n"
     "{\n"
     "   float y, u, v, red, green, blue;\n"
     "   y = texture2D(y_texture, gl_TexCoord[0].st).r ;\n"
     "   y =  1.1643 * (y - 0.0625);\n"
     "   u = texture2D(uv_texture, gl_TexCoord[0].st).r - 0.5;\n"
     "   v = texture2D(uv_texture, gl_TexCoord[0].st).a - 0.5;\n"
     "   red = y+1.5958*v;\n"
     "   green = y-0.39173*u-0.81290*v;\n"
     "   blue = y+2.017*u;\n"
     "   gl_FragColor = vec4(red, green, blue, 1.0);\n"
     "}");

    qDebug() << program.link();
    qDebug() << program.bind();

    glGenTextures(1, &y_texture);
    glGenTextures(1, &uv_texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, y_texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, video_width, video_height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, y_data);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, uv_texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, video_width>>1, video_height>>1, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, uv_data);

    program.setUniformValue("y_texture", 0);
    program.setUniformValue("uv_texture", 1);

    connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    timer.setInterval(25);
    timer.start();
}

void Gl_widget::paintGL()
{
    file.read((char*)y_data, (video_width*video_height*3)>>1);

    glActiveTexture(GL_TEXTURE0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, video_width, video_height, GL_LUMINANCE, GL_UNSIGNED_BYTE, y_data);
    glActiveTexture(GL_TEXTURE1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, video_width>>1, video_height>>1, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, uv_data);


    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);              // Top Left
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 0.0f);              // Top Right
        glTexCoord2f(1.0f,1.0f); glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
    glEnd();
    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void Gl_widget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Gl_widget::on_timeout()
{
    updateGL();
}
