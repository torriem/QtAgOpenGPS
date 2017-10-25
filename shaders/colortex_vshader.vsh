#ifdef GL_ES
precision highp int;
precision highp float;
#endif
/* A very simple shader that doesn't use any textures
 */

//the 4x4 MVP matix
uniform mat4 mvpMatrix;

//pull in 3d vertices from buffer
attribute vec3 vertex;
//texture coordinate to map to
attribute vec2 texcoord_src;
//transmit the interpolated coordinate to fragment shader
varying vec2 texcoord;

void main(void)
{
    //compute position of vertex on screen
    gl_Position = mvpMatrix * vec4(vertex,1.0);
    texcoord = texcoord_src;
}
