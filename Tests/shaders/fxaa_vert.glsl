#version 330

vec4 calcFXAAPos(vec2 screenSize, vec2 UV) {
    vec2 rcpFrame = vec2(1.0) / screenSize;
    vec4 pos_;
    pos_.xy = UV;
    pos_.zw = UV - (rcpFrame * 0.75);
    return pos_;
}
