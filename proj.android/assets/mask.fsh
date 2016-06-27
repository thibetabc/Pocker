varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
    vec4 normalColor = texture2D(CC_Texture0, v_texCoord);
    float alp = texture2D(CC_Texture1, v_texCoord).a;
    gl_FragColor = vec4(normalColor.rgb, alp);
}
