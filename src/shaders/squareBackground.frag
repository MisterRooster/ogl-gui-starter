// https://www.shadertoy.com/view/MdVXzw
// All rights belong to vamoss
uniform vec2 mousePos;
uniform vec3 resolution;
uniform float time;
uniform vec3 bgColor;
uniform vec3 rectColor;
uniform float total;		// number of rectangles

in vec2 vCoord;
out vec4 FragColor0;

// noise background
const float noiseIntensity = 2.8;
const float noiseDefinition = 0.6;

// rectangles
const float minSize = 0.03;			// rectangle min size
const float maxSize = 0.08 - minSize;	// rectangle max size
const float yDistribution = 0.5;


float random(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(in vec2 p)
{
	p *= noiseIntensity;
	vec2 i = floor(p);
	vec2 f = fract(p);
	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(mix(random(i + vec2(0.0, 0.0)),
	               random(i + vec2(1.0, 0.0)), u.x),
	           mix(random(i + vec2(0.0, 1.0)),
	               random(i + vec2(1.0, 1.0)), u.x), u.y);
}

float fbm(in vec2 uv)
{
	uv *= 5.0;
	mat2 m = mat2(1.6, 1.2, -1.2, 1.6);

	float f = 0.5000 * noise(uv); uv = m * uv;
	f += 0.2500 * noise(uv); uv = m * uv;
	f += 0.1250 * noise(uv); uv = m * uv;
	f += 0.0625 * noise(uv);

	return 0.5 + 0.5 * f;
}

/** Glow anchor in the same aspect corrected space as uv, placed so that the
	'uv + glowPos' below cancels out exactly at the mouse cursor. */
vec2 glowPos()
{
	vec2 m = mousePos * 2.0 - 1.0;
	m.x *= resolution.x / resolution.y;
	return vec2(-m.x, 2.0 - m.y);
}

vec3 bg(vec2 uv)
{
	float velocity = time / 1.6;
	float intensity = sin(uv.x * 3.0 + velocity * 2.0) * 1.1 + 1.5;
	uv.y -= 2.0;
	vec2 bp = uv + glowPos();
	uv *= noiseDefinition;

	// ripple
	float rb = fbm(vec2(uv.x * 0.5 - velocity * 0.03, uv.y)) * 0.1;
	uv += rb;

	// coloring
	float rz = fbm(uv * 0.9 + vec2(velocity * 0.35, 0.0));
	rz *= dot(bp * intensity, bp) + 1.2;

	vec3 col = bgColor / (0.1 - rz);
	return sqrt(abs(col));
}

float rectangle(vec2 uv, vec2 pos, float width, float height, float blur)
{
	pos = (vec2(width, height) + 0.01) / 2.0 - abs(uv - pos);
	pos = smoothstep(0.0, blur, pos);
	return pos.x * pos.y;
}

mat2 rotate2d(float angle)
{
	return mat2(cos(angle), -sin(angle),
	            sin(angle), cos(angle));
}

void main()
{
	vec2 fragCoord = vCoord * resolution.xy;

	vec2 uv = fragCoord / resolution.xy * 2.0 - 1.0;
	uv.x *= resolution.x / resolution.y;

	// background
	vec3 color = bg(uv) * (2.0 - abs(uv.y * 2.0));

	// rectangles
	float velX = -time / 8.0;
	float velY = time / 10.0;

	for (float i = 0.0; i < total; i++)
	{
		float index = i / total;
		float rnd = random(vec2(index));

		vec3 pos;
		pos.x = fract(velX * rnd + index) * 4.0 - 2.0;
		pos.y = sin(index * rnd * 1000.0 + velY) * yDistribution;
		pos.z = maxSize * rnd + minSize;

		vec2 uvRot = uv - pos.xy + pos.z / 2.0;
		uvRot = rotate2d(i + time / 2.0) * uvRot;
		uvRot += pos.xy + pos.z / 2.0;

		float rect = rectangle(uvRot, pos.xy, pos.z, pos.z, (maxSize + minSize - pos.z) / 2.0);
		color += rectColor * rect * pos.z / maxSize;
	}

	FragColor0 = vec4(color, 1.0);
}
