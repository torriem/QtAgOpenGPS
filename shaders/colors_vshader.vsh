#version 330
/* A very simple shader that doesn't use any textures
 */

//pull in 2d vertices from buffer 0
layout(location = 0) in vec2 vertex;
layout(location = 1) in vec4 color;

//attribute highp vec4 qt_Vertex;
//attribute highp vec4 qt_MultiTexCoord0;

//the 4x4 MVP matix
uniform highp mat4 mvpMatrix;

//interpolate color across triangle
varying highp vec4 fColor;

void main(void)
{
    //compute position of vertex on screen
    gl_Position = mvpMatrix * vec4(vertex,0,1.0);
    //qt_TexCoord0 = qt_MultiTexCoord0;
    fColor = color;
}
