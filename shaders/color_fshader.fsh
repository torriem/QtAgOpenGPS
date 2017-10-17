/* A very simple fragment shader that paints everything a
 * single color, defined by the color attribute
 */
uniform vec4 color;

void main(void)
{
    gl_FragColor = color;
}
