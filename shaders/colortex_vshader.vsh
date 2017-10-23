#version 120
/* A very simple shader that doesn't use any textures
 */

//the 4x4 MVP matix
uniform highp mat4 mvpMatrix;

//pull in 3d vertices from buffer
attribute highp vec3 vertex;
//texture coordinate to map to
attribute highp vec2 texcoord_src;
//transmit the interpolated coordinate to fragment shader
varying highp vec2 texcoord;

void main(void)
{
    //compute position of vertex on screen
    gl_Position = mvpMatrix * vec4(vertex,1.0);
    texcoord = texcoord_src;
}
