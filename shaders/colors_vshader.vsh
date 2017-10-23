#version 120
/* A very simple shader that doesn't use any textures
 */

//pull in 3d vertices from buffer
//layout(location = 0) in vec3 vertex;
attribute highp vec3 vertex;
//pull in interleved color from buffer
//layout(location = 1) in vec4 color;
attribute highp vec4 color;

//attribute highp vec4 qt_MultiTexCoord0;

//the 4x4 MVP matix
uniform highp mat4 mvpMatrix;

//interpolate color across triangle
varying highp vec4 fColor;

//point size
uniform highp float pointSize = 1.0;


void main(void)
{
    gl_PointSize = pointSize;
    //compute position of vertex on screen
    gl_Position = mvpMatrix * vec4(vertex,1.0);
    //qt_TexCoord0 = qt_MultiTexCoord0;
    fColor = color;
}
