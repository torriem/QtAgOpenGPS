#version 120
//#ifdef GL_ES
//precision highp int;
//precision highp float;
//#endif
/* A very simple fragment shader combines a texture color
 * with a solid color.
 */
uniform vec4 color;
uniform sampler2D texture;
varying vec2 texcoord;

void main(void)
{
    //get pixel from texture
    vec4 temp = texture2D(texture, texcoord);
    gl_FragColor = temp;
    //TODO: blend color with texture
}
