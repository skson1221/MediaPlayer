#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

attribute vec4	a_Position;
attribute vec2	a_TexCoord;
attribute vec4  a_translate;

uniform mat4		mTranslate;
uniform float		xMargin;
uniform float		yMargin;

varying vec2	v_TexCoord;

void main()
{
	gl_Position = mTranslate * a_Position;
	//gl_Position.x = gl_Position.x * xMargin;
	//gl_Position.y = gl_Position.y * yMargin;
	v_TexCoord  = a_TexCoord;
	v_TexCoord.x = v_TexCoord.x * xMargin;
	v_TexCoord.y = v_TexCoord.y * yMargin;
}