#version 330

#ifdef GL_ES
precision highp int;
precision highp float;
#endif
/* A very simple shader that doesn't use any textures
 */

//pull in 3d vertices from buffer 0
//layout(location = 0) in vec3 vertex;
attribute highp vec3 vertex;


//the 4x4 MVP matix
uniform highp mat4 mvpMatrix;
//point size
uniform highp float pointSize;
uniform vec4 color;

//varying highp vec4 qt_TexCoord0;
out VertexData{
    vec4 mColor;
} VertexOut;


void main(void)
{
    gl_PointSize = pointSize;
    //compute position of vertex on screen
    gl_Position = mvpMatrix * vec4(vertex,1.0);

    VertexOut.mColor = color;
    //qt_TexCoord0 = qt_MultiTexCoord0;
}
