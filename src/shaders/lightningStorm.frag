// https://www.shadertoy.com/view/XXyGzh
// All rights belong to SnoopethDuckDuck
uniform vec3 resolution;
uniform float time;

in vec2 vCoord;
out vec4 FragColor0;

/** Saturating tanh, use instead of tanh below if the shader shows black artifacts. */
vec2 stanh(vec2 a)
{
	return tanh(clamp(a, -40., 40.));
}

void main()
{
	vec2 v = resolution.xy;
	vec2 u = .2 * (vCoord * resolution.xy * 2.0 - v) / v.y;

	vec4 z = FragColor0 = vec4(1, 2, 3, 0);

	for (float a = .5, t = time, i = 0.; ++i < 19.;)
	{
		v = cos(++t - 7. * u * pow(a += .03, i)) - 5. * u;

		u += tanh(40. * dot(u *= mat2(cos(i + .02 * t - z.wxzw * 11.)), u)
		              * cos(1e2 * u.yx + t)) / 2e2
		   + .2 * a * u
		   + cos(4. / exp(dot(FragColor0, FragColor0) / 1e2) + t) / 3e2;

		FragColor0 += (1. + cos(z + t))
		            / length((1. + i * dot(v, v))
		                   * sin(1.5 * u / (.5 - dot(u, u)) - 9. * u.yx + t));
	}

	FragColor0 = 25.6 / (min(FragColor0, 13.) + 164. / FragColor0)
	           - dot(u, u) / 250.;
}
