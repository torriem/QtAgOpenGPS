#version 330

#ifdef GL_ES
precision highp int;
precision highp float;
#endif
/* A very simple fragment shader that paints everything a
 * single color, defined by the color attribute
 */
in VertexData{
    vec2 mTexCoord;
    vec4 mColor;
} VertexIn;

void main(void)
{
    gl_FragColor = VertexIn.mColor;
}
