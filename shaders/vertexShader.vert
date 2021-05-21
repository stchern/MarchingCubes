#version 330
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;
in vec3 vertex;
in vec3 normal;

out vec3 m_vertex;
out vec3 m_normal;

void main(void)
{
    m_vertex = (modelViewMatrix * vec4(vertex, 1.0)).xyz;
    m_normal = (normalMatrix * vec4(normal, 1.0)).xyz; 
    gl_Position = projectionMatrix * vec4(m_vertex, 1.0);
}
