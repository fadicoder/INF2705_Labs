#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 size;
layout (location = 4) in float timeToLive;

out vec3 positionMod;
out vec3 velocityMod;
out vec4 colorMod;
out vec2 sizeMod;
out float timeToLiveMod;

uniform float time;
uniform float dt;

uint seed = uint(time * 1000.0) + uint(gl_VertexID);
uint randhash( ) // entre  0 et UINT_MAX
{
    uint i=((seed++)^12345391u)*2654435769u;
    i ^= (i<<6u)^(i>>26u); i *= 2654435769u; i += (i<<5u)^(i>>12u);
    return i;
}
float random() // entre  0 et 1
{
    const float UINT_MAX = 4294967295.0;
    return float(randhash()) / UINT_MAX;
}

const float PI = 3.14159265359f;
vec3 randomInCircle(in float radius, in float height)
{
    float r = radius * sqrt(random());
    float theta = random() * 2 * PI;
    return vec3(r * cos(theta), height, r * sin(theta));
}

float randomBetween(in float low, in float high){
    return low + (random() / (1/(high-low)));
}

const float MAX_TIME_TO_LIVE = 2.0f;
const float INITIAL_RADIUS = 0.2f;
const float INITIAL_HEIGHT = 0.0f;
const float FINAL_RADIUS = 0.5f;
const float FINAL_HEIGHT = 5.0f;

const float INITIAL_SPEED_MIN = 0.5f;
const float INITIAL_SPEED_MAX = 0.6f;

const float INITIAL_TIME_TO_LIVE_RATIO = 0.85f;

const float INITIAL_ALPHA = 0.0f;
const float ALPHA = 0.1f;
const vec3 YELLOW_COLOR = vec3(1.0f, 0.9f, 0.0f);
const vec3 ORANGE_COLOR = vec3(1.0f, 0.4f, 0.2f);
const vec3 DARK_RED_COLOR = vec3(0.1, 0.0, 0.0);

const vec3 ACCELERATION = vec3(0.0f, 0.1f, 0.0f);

void main()
{
    if (timeToLive < 0.0){
        // Init particule
        positionMod = randomInCircle(INITIAL_RADIUS, INITIAL_HEIGHT);
        // Direction
        vec3 direction = randomInCircle(FINAL_RADIUS, FINAL_HEIGHT);
        // Speed
        velocityMod = vec3(direction.x, direction.y, randomBetween(INITIAL_SPEED_MIN, INITIAL_SPEED_MAX));
        // TTL
        timeToLiveMod = randomBetween(1.7, 2.0);
        // Color
        colorMod = vec4(YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, INITIAL_ALPHA);
        // Size
        sizeMod = vec2(1.0, 1.0);
        return;
    }

    float timeToLiveNormalised = 1 - (timeToLive / MAX_TIME_TO_LIVE);

    positionMod = positionMod + (velocity * dt);
    velocityMod = velocity + (ACCELERATION * dt);

    vec3 nextColor = DARK_RED_COLOR;
    if (timeToLiveNormalised < 0.25) {
        nextColor = YELLOW_COLOR;
    } else if (timeToLiveNormalised < 0.3) {
        nextColor = smoothstep(YELLOW_COLOR, ORANGE_COLOR, vec3(timeToLiveNormalised));
    } else if (timeToLiveNormalised < 0.5) {
        nextColor = ORANGE_COLOR;
    } else {
        nextColor = smoothstep(ORANGE_COLOR, DARK_RED_COLOR, vec3(timeToLiveNormalised));
    }
    float alpha = smoothstep(0.0, 0.2, color.a) * 1 - smoothstep(0.8, 1, color.a);
    colorMod = vec4(nextColor.r, nextColor.g, nextColor.b, alpha);
    sizeMod = size * mix(1.0, 1.5, timeToLiveNormalised);
    timeToLiveMod = timeToLive - dt;
}
