// https://www.shadertoy.com/view/NflSD8
// All rights belong to OldEclipse
uniform vec3 resolution;
uniform float time;

in vec2 vCoord;
out vec4 FragColor0;

void main()
{
	vec2 I = vCoord * resolution.xy;

	// distance travelled, density
	float t = 0., v;
	FragColor0 = vec4(0.);

	// raymarching loop
	for (float i = 0.; i++ < 50.;)
	{
		vec3 p = t * normalize(vec3(I + I, 1) - resolution.xyy);

		// rotation of xy coordinates based on distance travelled
		p.xy *= mat2(cos(t * .15 + vec4(0, 11, 33, 0)));

		// move forward
		p.z -= time;

		// coordinate repetition
		p = mod(p, 4.) - 2.;

		// density from mix between sphere and line sdf based on distance travelled
		v = mix(abs(length(p) - 1.), length(p.xz), .5 - .5 * cos(t)) + .01;

		// travel forwards based on density
		t += v * .3;

		// color accumulation based on density and distance travelled
		FragColor0 += exp(sin(t + vec4(0, 2, 4, 0))) / v;
	}

	// tone mapping
	FragColor0 = tanh(FragColor0 / 2e2);
}
