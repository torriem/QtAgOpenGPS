#ifdef GL_ES
precision highp int;
precision highp float;
#endif
/* A very simple fragment shader combines a texture color
 * with a solid color.
 */
uniform vec4 color;
uniform bool useColor;
uniform sampler2D texture;
varying vec2 texcoord;

void main(void)
{
    //get pixel from texture
    vec4 temp1 = texture2D(texture, texcoord);

    if (useColor) {
        //calculate a luminosity value for texture pixel (convert to
        //grey scale)
        //float lum = temp1.r * 0.25 + temp1.g * 0.5 + temp1.b* 0.25;

        //adjust the color brightness according to the lum calculated,
        //then apply the color's alpha channel to the texture's alpha.
        //This is not quite what AOG does; maybe there's a different
        //way we want to blend the colors to make the ground look better,
        //and work with the font texture.
        //gl_FragColor  = vec4(color.r * lum, color.g*lum, color.b*lum, temp1.a * color.a);
        gl_FragColor  = vec4(color.r * temp1.r, color.g*temp1.g, color.b*temp1.b, temp1.a * color.a);
    } else {
        //just draw texture
        gl_FragColor = temp1;
    }
}
