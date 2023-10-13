#version 330 core

uniform vec2 windowSize;
uniform vec2 sizes;
uniform vec2 fromPos;

out vec4 out_col;

void main() {
    float threshold = 256.0;
    float threshold2 = threshold * threshold;

    vec2 xy0 = fromPos + ((gl_FragCoord.xy / windowSize) - 0.5) * sizes;
    vec2 xy = xy0;

    bool threshold_surpassed = false;

    // to uniforms
    int iters = 10;
    bool smoothing = true;
    //

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

    out_col = vec4(result, 0., 0., 1.);
}