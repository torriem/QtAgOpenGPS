#ifdef GL_ES
precision highp int;
precision highp float;
#endif
/* A very simple fragment shader that varies color by
 * interpolating between the vertices of the triangle
 * (each vertex has a color).
 */
varying vec4 fColor;

void main(void)
{
    gl_FragColor = fColor;
}
