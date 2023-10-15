#version 330 core

uniform int iters;
uniform float threshold;
uniform vec2 windowSize;
uniform vec2 sizes;
uniform vec2 fromPos;

out vec4 out_col;

void main() {
    float threshold2 = threshold * threshold;

    vec2 xy0 = fromPos + ((gl_FragCoord.xy / windowSize) - 0.5) * sizes;
    vec2 xy = xy0;

    bool threshold_surpassed = false;
    bool smoothing = true;

    int iter = 0;
    int cnt = 0;
    for (; iter < iters; ++iter) {
        xy = vec2(
            xy.x * xy.x - xy.y * xy.y + xy0.x,
            2.0 * xy.x * xy.y + xy0.y);
        threshold_surpassed = threshold_surpassed || (xy.x * xy.x + xy.y * xy.y) > threshold2;
        cnt += threshold_surpassed ? 0 : 1;
    }
    float result = float(cnt);
    if (smoothing && iter != iters) {
        result = result - log(log(length(xy)) / log(threshold)) / log(2.0);
    }

    result = result / iters;

    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 0.7, 0.4);
    vec3 d = vec3(0.00, 0.15, 0.20);
    vec3 color = a + b * cos(2.0 * 3.14 * (c * result + d));

    out_col = vec4(color.rgb, 1.0);
}
