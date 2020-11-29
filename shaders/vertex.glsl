#version 330 core

// Input variables from the vertex buffer
layout (location = 0) in vec3 aPos;

// Uniforms
uniform mat4 Transform;

// Output variables for the fragment shader
//

void main()
{
  gl_Position = Transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
