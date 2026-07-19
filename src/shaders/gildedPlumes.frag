// golfing 1893 chars "Gilded Plumes New" by fractalpark. https://shadertoy.com/view/NfcGDH
//                     Adapted from FractalPark (MIT) https://fractalpark.com
uniform vec3 resolution;
uniform float time;

in vec2 vCoord;
out vec4 FragColor0;

void main()
{
    float t = abs(mod(time/64.,2.) - 1.), s, i;
    vec4 o = vec4(0, 0, 0, 1);
    vec2 z = vCoord * resolution.xy;

    for (z = (z+z-(o.xy=resolution.xy))/o.y 
           * mat2(cos(2.87-1.6*t + vec4(0,11,33,0))) / exp(6.13*t+.16)
           + vec2(.902302, -2.17323); //.9023
         ++i<2e2 && dot(z,z)<6e5; 
         o = .5 + .5 * cos(6.3*(++s/vec4(2,2,4,1)/i + vec4(.8,.9,.3,0)))
        )
        s += sin(8.*atan(z.y, z.x)),
        z = vec2(cosh(z.x)*cos(z.y) - .043213, sinh(z)*sin(z.y) + .084741);
        FragColor0 = o;
}