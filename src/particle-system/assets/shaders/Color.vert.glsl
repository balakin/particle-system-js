#version 300 es

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec4 VertexColor;

out vec4 Color;

uniform mat4 ViewProjectionMatrix;

void main()
{
  Color = VertexColor;
  gl_Position = ViewProjectionMatrix * vec4(VertexPosition, 1.0);
}