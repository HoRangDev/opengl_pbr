#version 450 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 worldPosGeom[];
in vec2 texCoordsGeom[];
in vec3 worldNormalGeom[];
in mat3 tbnGeom[];

out vec3 worldPosFrag;
out vec2 texCoordsFrag;
out vec3 worldNormalFrag;
out mat3 tbnFrag;

void main()
{
   //vec3 faceNormal = abs(worldNormalGeom[0] + worldNormalGeom[1] + worldNormalGeom[2]);
   //uint dominantAxis = faceNormal[1] > faceNormal[0] ? 1 : 0;
   //dominantAxis = faceNormal[2] > faceNormal[dominantAxis] ? 2 : dominantAxis;

   //vec2 side0N = normalize(worldPosGeom[1].xy - worldPosGeom[0].xy);
   //vec2 side1N = normalize(worldPosGeom[2].xy - worldPosGeom[1].xy);
   //vec2 side2N = normalize(worldPosGeom[0].xy - worldPosGeom[2].xy);

   //for (uint idx = 0; idx < 3; ++idx)
   //{
   //   worldPosFrag = worldPosGeom[idx];
   //   texCoordsFrag = texCoordsGeom[idx];
   //   worldNormalFrag = worldNormalGeom[idx];
   //   tbnFrag = tbnGeom[idx];

   //   if (dominantAxis == 0)
   //   {
   //      gl_Position = vec4(worldPosFrag.zy, 0.0, 1.0);
   //   }
   //   else if (dominantAxis == 1)
   //   {
   //      gl_Position = vec4(worldPosFrag.xz, 0.0, 1.0);
   //   }
   //   else
   //   {
   //      gl_Position = vec4(worldPosFrag.xy, 0.0, 1.0);
   //   }

   //   EmitVertex();
   //}

   //EndPrimitive();

   vec3 p1 = worldPosGeom[1] - worldPosGeom[0];
   vec3 p2 = worldPosGeom[2] - worldPosGeom[0];
   vec3 p = abs(cross(p1, p2));
   for (uint idx = 0; idx < 3; ++idx)
   {
      worldPosFrag = worldPosGeom[idx];
      worldNormalFrag = worldNormalGeom[idx];
      texCoordsFrag = texCoordsGeom[idx];
      tbnFrag = tbnGeom[idx];

      vec3 outPos;
      if (p.z > p.x && p.z > p.y)
      {
         outPos = vec3(worldPosFrag.x, worldPosFrag.y, 0.0);
      }
      else if (p.x > p.y && p.x > p.z)
      {
         outPos = vec3(worldPosFrag.y, worldPosFrag.z, 0.0);
      }
      else
      {
         outPos = vec3(worldPosFrag.x, worldPosFrag.z, 0.0);
      }
      gl_Position = vec4(outPos, 1.0);

      EmitVertex();
   }

   EndPrimitive();
}