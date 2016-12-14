/*uniform sampler2D tex;
varying vec3 N;
varying vec4 position;

void main()
{
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    N = gl_NormalMatrix*gl_Normal;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    position = gl_ModelViewMatrix * gl_Vertex;
}
*/
varying vec3 lightDir,normal;
 
void main()
{
    normal = normalize(gl_NormalMatrix * gl_Normal);
 
    lightDir = normalize(vec3(gl_LightSource[0].position));
    gl_TexCoord[0] = gl_MultiTexCoord0;
 
    gl_Position = ftransform();
}
