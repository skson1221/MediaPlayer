#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

varying vec2 		v_TexCoord;

uniform bool 		isYUVDraw;
uniform sampler2D 	texBG;
uniform sampler2D 	texY;
uniform sampler2D 	texU;
uniform sampler2D 	texV;

vec4 YUV2RGB()
{
	float nx = v_TexCoord.x;
	float ny = v_TexCoord.y;
	float y=texture2D(texY,vec2(nx, ny)).r;
	float u=texture2D(texU,vec2(nx, ny)).r;
	float v=texture2D(texV,vec2(nx, ny)).r;

	y=1.1643*(y-0.0625);
	u=u-0.5;
	v=v-0.5;
	
	float r=y+1.5958*v;
	float g=y-0.39173*u-0.81290*v;
	float b=y+2.017*u;

	return vec4(r,g,b,1);
}

void main()
{
	vec4 texColor;

	if(isYUVDraw)
		texColor = YUV2RGB();
	else
		texColor = texture2D(texBG, v_TexCoord);

	if (texColor.a == 0.0)
		discard;
	gl_FragColor = texColor;
}