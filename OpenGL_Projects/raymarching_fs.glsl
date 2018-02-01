#version 430
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;
out vec4 Frag_Color;

uniform float slider;

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 1000.0;
const float EPSILON = 0.001;
const vec3 eye_position = vec3(10.0,20.0,-15.0) * 0.1;
const float PI = 3.14159265;

float intersectSDF(float distA, float distB)
{
    return max(distA,distB);
}

float unionSDF(float distA, float distB)
{
    return min(distA,distB);
}

float differentSDF(float distA,float distB)
{
    return max(distA,-distB);
}

/**
 * Signed distance function for a cube centered at the origin
 * with width = height = length = 2.0
 */
float cubeSDF(vec3 p) {
    // If d.x < 0, then -1 < p.x < 1, and same logic applies to p.y, p.z
    // So if all components of d are negative, then p is inside the unit cube
    vec3 d = abs(p) - vec3(1.0, 1.0, 1.0);
    
    // Assuming p is inside the cube, how far is it from the surface?
    // Result will be negative or zero.
    float insideDistance = min(max(d.x, max(d.y, d.z)), 0.0);
    
    // Assuming p is outside the cube, how far is it from the surface?
    // Result will be positive or zero.
    float outsideDistance = length(max(d, 0.0));
    
    return insideDistance + outsideDistance;
}

/**
 * Signed distance function for a sphere centered at the origin with radius 1.0;
 */
float sphereSDF(vec3 p) {
    p.y = 0.0 + 1.2* p.y - abs(p.x)*sqrt(((25.0- abs(p.x))/40.0));

    float raidus = (1 + 0.1*pow((0.5 + 0.5*sin(PI* time)),8));
    float c = length(p) - pow(raidus,2);
    return c;
}

float getAngle(vec2 v1, vec2 v2)
{
    return acos(dot(v1,v2)/(length(v1)*length(v2)));
}

float curva1(vec3 p)
{
    // map Descartes to polar coordinates
    vec2 xz_vector = p.xz - vec2(0.0,0.0); 
    float rad = getAngle(xz_vector,vec2(1.0,0.0)); // radian 
    float y_part = rad* slider * 0.1;
    
    float p1 = length(xz_vector) - 0.1;
    float p2 = p.y - y_part;
    return length(vec2(p1,p2));
    //return p1;
}

/**
 * Signed distance function describing the scene.
 * 
 * Absolute value of the return value indicates the distance to the surface.
 * Sign indicates whether the point is inside or outside the surface,
 * negative indicating inside.
 */
float sceneSDF(vec3 samplePoint) {
    //return cubeSDF(samplePoint);
    // float sphereDist = sphereSDF(samplePoint / 1.2) * 1.2;
    // float cubeDist = cubeSDF(samplePoint) * 1.2;
    // return intersectSDF(cubeDist, sphereDist);
    //return sphereSDF(samplePoint);
    return curva1(samplePoint);
}

/**
 * Return the shortest distance from the eyepoint to the scene surface along
 * the marching direction. If no part of the surface is found between start and end,
 * return end.
 * 
 * eye: the eye point, acting as the origin of the ray
 * marchingDirection: the normalized direction to march in
 * start: the starting distance away from the eye
 * end: the max distance away from the ey to march before giving up
 */
float shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, float start, float end) {
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = sceneSDF(eye + depth * marchingDirection);
        if (dist < EPSILON) {
			return depth;
        }
        depth += dist;
        if (depth >= end) {
            return end;
        }
    }
    return end;
}
            

/**
 * Return the normalized direction to march in from the eye point for a single pixel.
 * 
 * fieldOfView: vertical field of view in degrees
 * size: resolution of the output image
 * fragCoord: the x,y coordinate of the pixel in the output image
 */
vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}

/**
 * Using the gradient of the SDF, estimate the normal on the surface at point p.
 */
vec3 estimateNormal(vec3 p) {
    return normalize(vec3(
        sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),
        sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),
        sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

/**
 * Lighting contribution of a single point light source via Phong illumination.
 * 
 * The vec3 returned is the RGB color of the light's contribution.
 *
 * k_a: Ambient color
 * k_d: Diffuse color
 * k_s: Specular color
 * alpha: Shininess coefficient
 * p: position of point being lit
 * eye: the position of the camera
 * lightPos: the position of the light
 * lightIntensity: color/intensity of the light
 *
 * See https://en.wikipedia.org/wiki/Phong_reflection_model#Description
 */
vec3 phongContribForLight(vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye,
                          vec3 lightPos, vec3 lightIntensity) {
    vec3 N = estimateNormal(p);
    vec3 L = normalize(lightPos - p);
    vec3 V = normalize(eye - p);
    vec3 R = normalize(reflect(-L, N));
    
    float dotLN = dot(L, N);
    float dotRV = dot(R, V);
    
    if (dotLN < 0.0) {
        // Light not visible from this point on the surface
        return vec3(0.0, 0.0, 0.0);
    } 
    
    if (dotRV < 0.0) {
        // Light reflection in opposite direction as viewer, apply only diffuse
        // component
        return lightIntensity * (k_d * dotLN);
    }
    return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

/**
 * Lighting via Phong illumination.
 * 
 * The vec3 returned is the RGB color of that point after lighting is applied.
 * k_a: Ambient color
 * k_d: Diffuse color
 * k_s: Specular color
 * alpha: Shininess coefficient
 * p: position of point being lit
 * eye: the position of the camera
 *
 * See https://en.wikipedia.org/wiki/Phong_reflection_model#Description
 */
vec3 phongIllumination(vec3 k_a, vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye) {
    const vec3 ambientLight = 0.5 * vec3(1.0, 1.0, 1.0);
    vec3 color = ambientLight * k_a;
    
    vec3 light1Pos = vec3(4.0 * sin(time),
                          2.0,
                          4.0 * cos(time));
    vec3 light1Intensity = vec3(0.4, 0.4, 0.4);
    
    color += phongContribForLight(k_d, k_s, alpha, p, eye,
                                  light1Pos,
                                  light1Intensity);
    
    vec3 light2Pos = vec3(2.0 * sin(0.37 * time),
                          2.0 * cos(0.37 * time),
                          2.0);
    vec3 light2Intensity = vec3(0.4, 0.4, 0.4);
    
    color += phongContribForLight(k_d, k_s, alpha, p, eye,
                                  light2Pos,
                                  light2Intensity);    
    return color;
}

/**
 * Return a transform matrix that will transform a ray from view space
 * to world coordinates, given the eye point, the camera target, and an up vector.
 *
 * This assumes that the center of the camera is aligned with the negative z axis in
 * view space when calculating the ray marching direction. See rayDirection.
 */
mat4 viewMatrix(vec3 eye, vec3 center, vec3 up) {
    // Based on gluLookAt man page
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);
    return mat4(
        vec4(s, 0.0),
        vec4(u, 0.0),
        vec4(-f, 0.0),
        vec4(0.0, 0.0, 0.0, 1)
    );
}

// Camera rotation
mat3 rotationMatrix()
{
    float r = radians(2*PI*sin(time));
    vec3 x = vec3(cos(r),-sin(r),0.0);
    vec3 y = vec3(sin(r),cos(r),0.0);
    vec3 z = vec3(0.0,0.0,0.0);
    return mat3(x,y,z);
}

void main()
{
    vec2 uv = gl_FragCoord.xy/resolution.xy;
    vec2 mouse_uv = mouse.xy/resolution.xy;
    vec3 bg = vec3(250.0,243.0,225.0)/vec3(255.0);

    vec3 color = bg;
    vec3 viewDir = rayDirection(45.0,resolution.xy,gl_FragCoord.xy);
    
    // Rotation
    vec3 eye = eye_position;
    
    mat4 viewToWorld = viewMatrix(eye,vec3(0.0,0.2,0.0),vec3(0.0,1.0,0.0));
    vec3 worldviewDir = (viewToWorld * vec4(viewDir,0.0)).xyz;
    
    float dist = shortestDistanceToSurface(eye,worldviewDir,MIN_DIST,MAX_DIST);

    if(dist > MAX_DIST-EPSILON)
    {
        //Frag_Color = vec4(0.0,0.0,0.0,0.0);
        Frag_Color = vec4(bg,1.0);
        return;
    }

    vec3 p = eye + dist * viewDir;

    vec3 K_a = vec3(0.2,0.2,0.2);
    vec3 K_d = vec3(0.7,0.2,0.2);
    vec3 K_s = vec3(1.0,1.0,1.0);
    float shiness = 10.0;

    // color = phongIllumination(K_a,K_d,K_s,shiness,p,eye);

    color = vec3(1.0,0.0,0.0);

    Frag_Color = vec4(color,1.0);
}