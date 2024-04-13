uniform vec2 mousePos;

in vec2 vCoord;
out vec4 FragColor0;

void main()
{
	float intensity = clamp(1.0f / (0.8f + 20.0f * length(vCoord.xy - mousePos)), 0.0f, 1.0f);
	FragColor0.rgb = 0.5f * vec3(1.0f, 0.6f, 0.0f) * intensity;
	FragColor0.a = 1.0f;
}