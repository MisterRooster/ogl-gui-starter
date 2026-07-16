// Weird endless living creature
// https://www.shadertoy.com/view/tljXWy
// inspired by Inigo Quilez live stream shader deconstruction
// Leon Denise (ponk) 2019.08.28

// Using code from
// Inigo Quilez
// Morgan McGuire

uniform vec2 mousePos;
uniform vec3 resolution;
uniform float time;

// tweak zone
uniform int count;
uniform float speed;
uniform float balance;
uniform float range;
uniform float radius;
uniform float blend;
uniform float falloff;

// increment it at your own GPU risk
uniform float motionFrames;

in vec2 vCoord;
out vec4 FragColor0;

// toolbox (PI comes from common.inc)
#define repeat(p, r) (mod(p, r) - r / 2.)

float random(vec2 p)
{
	return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x))));
}

mat2 rot(float a)
{
	float c = cos(a), s = sin(a);
	return mat2(c, -s, s, c);
}

float smoothmin(float a, float b, float r)
{
	float h = clamp(.5 + .5 * (b - a) / r, 0., 1.);
	return mix(b, a, h) - r * h * (1. - h);
}

float sdSphere(vec3 p, float r)
{
	return length(p) - r;
}

vec3 look(vec3 eye, vec3 target, vec2 anchor, float fov)
{
	vec3 forward = normalize(target - eye);
	vec3 right = normalize(cross(forward, vec3(0, 1, 0)));
	vec3 up = normalize(cross(right, forward));
	return normalize(forward * fov + right * anchor.x + up * anchor.y);
}

float geometry(vec3 pos, float sceneTime)
{
	float scene = 1., a = 1.;

	float t = sceneTime * .5 + pos.x / 30.;
	t = floor(t) + smoothstep(0.0, .9, pow(fract(t), 2.));

	pos.x = repeat(pos.x + time, 5.);

	for (int i = count; i > 0; --i)
	{
		pos.x = abs(pos.x) - range * a;
		pos.xy *= rot(cos(t) * balance / a + a * 2.);
		pos.zy *= rot(sin(t) * balance / a + a * 2.);
		scene = smoothmin(scene, sdSphere(pos, radius * a), blend * a);
		a /= falloff;
	}

	return scene;
}

float raymarch(vec3 eye, vec3 ray, float sceneTime, out float total)
{
	float dither = random(ray.xy + fract(sceneTime));
	total = 0.0;

	const int steps = 20;
	for (int index = steps; index > 0; --index)
	{
		float dist = geometry(eye + total * ray, sceneTime);
		dist *= 0.9 + 0.1 * dither;
		total += dist;

		if (dist < 0.001 * total)
			return float(index) / float(steps);
	}

	return 0.;
}

vec3 camera(vec3 eye)
{
	// mousePos stays untouched until the scene view is dragged with LMB, so an
	// out of range value means "no mouse input yet" and keeps the default eye
	if (abs(mousePos.x - .5) > 0.48 || abs(mousePos.y - .5) > 0.48)
		return eye;

	vec2 mouse = mousePos * 2. - 1.;
	eye.yz *= rot(mouse.y * PI);
	eye.xz *= rot(mouse.x * PI);
	return eye;
}

void main()
{
	vec2 fragCoord = vCoord * resolution.xy;

	vec2 uv = 2. * (fragCoord - 0.5 * resolution.xy) / resolution.y;
	vec3 eye = camera(vec3(0, 0, 4));
	vec3 ray = look(eye, vec3(0), uv, 1.);

	float total = 0.0;
	FragColor0 = vec4(0);

	for (float index = motionFrames; index > 0.; --index)
	{
		float dither = random(ray.xy + fract(time + index));
		float sceneTime = time * speed + (dither + index) / 10. / motionFrames;
		FragColor0 += vec4(raymarch(eye, ray, sceneTime, total)) / motionFrames;
	}

	// extra color
	FragColor0.rgb *= vec3(.7, .8, .9);
	float d = smoothstep(7., 0., total);
	FragColor0.rgb += vec3(0.8, .6, .5) * d;
}
