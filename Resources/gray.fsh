uniform sampler2D tex0;
uniform float   time;
uniform vec2    resolution;

varying vec2 v_texCoord;

void main()
{
    //float grey = 1.0;
    float grey = dot(texture2D(tex0,v_texCoord).rgb, vec3(0.299,0.587,0.114));
    float alpha = clamp(time/2.0, 0.0, 1.0);
    gl_FragColor = vec4(grey, grey, grey, alpha);
//gl_FragColor = vec4(1.0, 1.0, 0.0, 0.0);
}