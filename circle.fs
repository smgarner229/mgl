#version 330 core
varying vec2 val;

void main()
{
    float R = 1.0;
    float dist = sqrt(dot(val,val));
    if (dist >= R)
    {
        discard;
    }
    gl_FragData[0]=vec4(0.0,0.0,1.0,0.0);
}