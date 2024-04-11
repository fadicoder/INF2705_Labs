#version 400 core

layout(vertices = 4) out;

uniform mat4 modelView;


const float MIN_TESS = 4;
const float MAX_TESS = 64;

const float MIN_DIST = 30.0f;
const float MAX_DIST = 100.0f;


void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0) {

        vec4 p0 = gl_in[0].gl_Position; // (0,0)
        vec4 p1 = gl_in[1].gl_Position; // (1,0)
        vec4 p2 = gl_in[2].gl_Position; // (1,1)
        vec4 p3 = gl_in[3].gl_Position; // (0,1)

        //        OL3
        //     p3 a3 p2
        // OL0 a0    a2  OL2
        //     p0 a1 p1
        //        OL1

        vec4 a0 = (p0 + p3) / 2.0;
        vec4 a1 = (p0 + p1) / 2.0;
        vec4 a2 = (p1 + p2) / 2.0;
        vec4 a3 = (p2 + p3) / 2.0;

        a0 = modelView * a0;
        a1 = modelView * a1;
        a2 = modelView * a2;
        a3 = modelView * a3;

        float d0 = clamp(((distance(a0, vec4(0)) - MIN_DIST) / (MAX_DIST - MIN_DIST)), 0, 1);
        float d1 = clamp(((distance(a1, vec4(0)) - MIN_DIST) / (MAX_DIST - MIN_DIST)), 0, 1);
        float d2 = clamp(((distance(a2, vec4(0)) - MIN_DIST) / (MAX_DIST - MIN_DIST)), 0, 1);
        float d3 = clamp(((distance(a3, vec4(0)) - MIN_DIST) / (MAX_DIST - MIN_DIST)), 0, 1);

        float t0 = mix(MIN_TESS, MAX_TESS, d0);
        float t1 = mix(MIN_TESS, MAX_TESS, d1);
        float t2 = mix(MIN_TESS, MAX_TESS, d2);
        float t3 = mix(MIN_TESS, MAX_TESS, d3);

        gl_TessLevelOuter[0] = t0;
        gl_TessLevelOuter[1] = t1;
        gl_TessLevelOuter[2] = t2;
        gl_TessLevelOuter[3] = t3;
        gl_TessLevelInner[0] = max(t1, t3);
        gl_TessLevelInner[1] = max(t0, t2);
    }
}
