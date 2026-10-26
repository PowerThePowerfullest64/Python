#version 330 core
out vec4 FragColor;

in vec3 position;
uniform float u_time;

// Fast pseudo-random noise generator to warp the grid lines
float noise(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

void main()
{
    // 1. Warp space like melting liquid using sine modulations
    vec2 uv = position.xy;
    uv.x += sin(uv.y * 3.0 + u_time * 0.8) * 0.15;
    uv.y += cos(uv.x * 2.5 - u_time * 0.6) * 0.15;
    
    // 2. Create ultra-sharp, glowing grid lines (the scanline/matrix texture)
    vec2 grid = abs(sin(uv * 12.0 + u_time * 0.3));
    vec2 lineWeight = smoothstep(0.0, 0.08, grid);
    float gridPattern = 1.0 - min(lineWeight.x, lineWeight.y);
    
    // 3. Chromatic Aberration: Shift the R, G, and B channels separately for a digital glitch look
    float r = sin(uv.x * 2.0 + u_time * 1.2) * 0.5 + 0.5;
    float g = cos(uv.y * 2.5 + u_time * 1.0) * 0.5 + 0.5;
    float b = sin((uv.x + uv.y) * 1.8 + u_time * 1.5) * 0.5 + 0.5;
    
    // 4. Combine the liquid distortions into sharp digital bands
    vec3 ribbonColor = vec3(r, g * 0.3, b * 1.5) * gridPattern;
    
    // 5. Add a searing white core beam cutting through the center
    float beam = 0.01 / abs(uv.y - 0.1 * sin(uv.x * 4.0 + u_time * 2.0));
    vec3 finalColor = ribbonColor + vec3(beam * 0.6, beam * 0.9, beam * 1.0);
    
    // Push the contrast and add a clean vignettes
    finalColor = pow(finalColor, vec3(1.2));
    finalColor *= 1.4 - length(position.xy);
    
    FragColor = vec4(finalColor, 1.0);
}