﻿#include "Texture3D.h"

Texture3D::Texture3D(const std::vector<GLfloat>& rawData, unsigned int width, unsigned int height, unsigned int depth, Sampler3D sampler, unsigned int maxMipLevel, bool bGenerateMip)
{
   glGenTextures(1, &m_id);
   glBindTexture(GL_TEXTURE_3D, m_id);

   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, sampler.MinFilter);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, sampler.MagFilter);

   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, sampler.WrapS);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, sampler.WrapR);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, sampler.WrapT);

   glTexStorage3D(GL_TEXTURE_3D, maxMipLevel, GL_RGBA8, width, height, depth);
   glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, width, height, depth, 0, GL_RGBA, GL_FLOAT, rawData.data());
   if (bGenerateMip)
   {
      glGenerateMipmap(GL_TEXTURE_3D);
   }

   glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::Bind(unsigned int slot)
{
   glActiveTexture(GL_TEXTURE0 + slot);
   glBindTexture(GL_TEXTURE_3D, m_id);
}

void Texture3D::Unbind(unsigned slot)
{
   glActiveTexture(GL_TEXTURE0 + slot);
   glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::Clear(GLfloat clearColor[4])
{
   GLint prevBoundTexture = 0;
   glGetIntegerv(GL_TEXTURE_BINDING_3D, &prevBoundTexture);
   glBindTexture(GL_TEXTURE_3D, m_id);
   glClearTexImage(m_id, 0, GL_RGBA, GL_FLOAT, &clearColor);
   glBindTexture(GL_TEXTURE_3D, prevBoundTexture);
}
