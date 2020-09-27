#version 330 core

int p[] = int[](151,160,137,91,90,15,                                     
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
    151,160,137,91,90,15,                                     
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,  
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
);

float fade(float t)
{
    return t * t * t * (t * (6 * t - 15) + 10);
}

float grad(int hash, float x, float y, float z) 
{
    switch (hash & 0xF)
    {
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        case 0x4: return  x + z;
        case 0x5: return -x + z;
        case 0x6: return  x - z;
        case 0x7: return -x - z;
        case 0x8: return  y + z;
        case 0x9: return -y + z;
        case 0xA: return  y - z;
        case 0xB: return -y - z;
        case 0xC: return  y + x;
        case 0xD: return -y + z;
        case 0xE: return  y - x;
        case 0xF: return -y - z;
        default: return 0.;
    }
}

float noise(vec3 pos)
{
    int X = int(floor(pos.x)) & 255;
    int Y = int(floor(pos.y)) & 255;
    int Z = int(floor(pos.z)) & 255;

    float x = fract(pos.x);
    float y = fract(pos.y);
    float z = fract(pos.z);

    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    int aaa = p[p[p[X]+ Y] + Z];
    int aba = p[p[p[X] + Y + 1] + Z];
    int aab = p[p[p[X] + Y] + Z + 1];
    int abb = p[p[p[X] + Y + 1] + Z + 1];
    int baa = p[p[p[X + 1] + Y] + Z];
    int bba = p[p[p[X + 1] + Y + 1] + Z];
    int bab = p[p[p[X + 1] + Y] + Z + 1];
    int bbb = p[p[p[X + 1] + Y + 1] + Z + 1];

    float x1, x2, y1, y2;
    x1 = mix(grad(aaa, x, y, z),
             grad(baa, x - 1, y, z),
             u);
    x2 = mix(grad(aba, x, y - 1, z),
             grad(bba, x - 1, y - 1, z),
             u);
    y1 = mix(x1, x2, v);
    
    x1 = mix(grad(aab, x, y, z - 1),
             grad(bab, x - 1, y, z - 1),
             u);
    x2 = mix(grad(abb, x, y - 1, z - 1),
             grad(bbb, x - 1, y - 1, z - 1),
             u);
    y2 = mix(x1, x2, v);

    return (mix(y1, y2, w) + 1) * .5;
}

float noise(vec3 pos, float scale, float pers, float lac, int octaves)
{
    float val = 0;
    float freq = 1;
    float amp = 1;
    float maxval = 0;

    for (int i = 0; i < octaves; ++i)
    {
        maxval += amp;
        val += amp * noise(pos * scale * freq);
        amp *= pers;
        freq *= lac;
    }

    return val / max(maxval * .9, 1);  
}

in vec3 position;
in vec2 uv;
in vec3 normal;

out vec3 color;

uniform sampler2D textureSampler;
uniform vec3 eyePosition; // worldspace
uniform vec3 lightPos; // worldspace

uniform float scale;
uniform float pers;
uniform float lac;
uniform int octaves;

uniform bool textureOn;
uniform bool noiseOn;
uniform bool lightsOn;

uniform float time;

void main()
{    
    vec3 lightColor = vec3(1, 1, 1);
    float lightPower = 2.;

    vec3 diffuseColor = vec3(1);
    if (textureOn)
        diffuseColor = texture(textureSampler, uv).rgb;
    if (noiseOn)
        diffuseColor *= vec3(noise(vec3(uv, time), scale, pers, lac, octaves));

    if (lightsOn)
    {
        vec3 ambientColor = vec3(0.1, 0.1, 0.1) * diffuseColor;
        vec3 specularColor = vec3(0.3, 0.3, 0.3);

        vec3 n = normalize(normal);
        vec3 l = normalize(lightPos - position);
        float diff = max(dot(n, l), 0);

        vec3 d = normalize(eyePosition - position);
        vec3 e = reflect(-l, n);
        float spec = pow(max(dot(d, e), 0), 5);

        color = ambientColor + 
                diffuseColor * lightColor * lightPower * diff +
                specularColor * lightColor * lightPower * spec;
    }
    else
        color = diffuseColor;
}