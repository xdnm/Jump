varying vec4 v_position;

uniform vec2 center;
uniform float u_innerRadius;
uniform float u_radius;
uniform float u_time;
uniform float u_timeCircle;
uniform float u_alphaParm;
uniform vec4 u_color;

void main(void)
{
    float dis = distance(center, vec2(v_position.x, v_position.y));
    float alpha;
	float timeCircle = u_timeCircle;
	float time = mod(u_time, timeCircle);
	time = abs(time - timeCircle/2.0);
	if(dis < u_radius)
	{
		alpha =1.0- smoothstep(u_innerRadius, u_radius, dis);
		//alpha = alpha * clamp((time + 1.0) / timeCircle/2.0, 0.0f, 1.0f);
		alpha = alpha * (time / timeCircle/2.0f +u_alphaParm);
		//gl_FragColor = vec4(1.0, 0.0, 0.0, alpha);
		gl_FragColor = vec4(u_color.rgb, alpha);
	}
	else
	{
		gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	}

}