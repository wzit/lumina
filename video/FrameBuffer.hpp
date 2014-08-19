#pragma once

#include "GLObject.hpp"
#include "Texture.hpp"
#include "FrameBuffer.fpp"
#include "HotFrameBuffer.fpp"
#include "RenderBuffer.hpp"
#include "../util/ColorCore.hpp"

#include <GL/glew.h>
#include <functional>
#include <memory>
#include <vector>

namespace lumina {

namespace internal {

struct ColorAttPoint {
  GLuint handle;
  TexFormat format;

  ColorAttPoint() : handle(0) {}
};


class FrameBufferInterface : public GLObject {
public:
  virtual ~FrameBufferInterface() = default;

  virtual void create(Vec2i size) = 0;
  virtual void prime(std::shared_ptr<FrameBufferInterface> fb,
                     std::function<void(HotFrameBuffer&)> func) = 0;
  virtual void attachColor(int index, const Tex2D& tex) = 0;
  virtual void attachDepth(const Tex2D& tex) = 0;
  virtual void attachDepthStencil(const Tex2D& tex) = 0;

  virtual void attachRenderBuffer(const RenderBuffer& buf) = 0;

  virtual int countAttachments() = 0;

  virtual void clearColor(int index, Color32fA color) = 0;
  virtual void clearDepth(float val) = 0;
  virtual void clearDepthStencil(float depth, int stencil) = 0;

  virtual explicit operator bool() const = 0;


protected:
  // true if a user defined framebuffer is primed
  static bool s_isPrimed;

  friend ::lumina::FrameBuffer;
};

class UserFrameBuffer : public FrameBufferInterface {
public:
  UserFrameBuffer();
  UserFrameBuffer(UserFrameBuffer&&);

  ~UserFrameBuffer();

  void create(Vec2i size) override final;
  void prime(std::shared_ptr<FrameBufferInterface> fb,
             std::function<void(HotFrameBuffer&)> func) override final;
  void attachColor(int index, const Tex2D& tex) override final;
  void attachDepth(const Tex2D& tex) final;
  void attachDepthStencil(const Tex2D& tex) final;

  void attachRenderBuffer(const RenderBuffer& buf) override final;

  int countAttachments() override final;

  void clearColor(int index, Color32fA color);
  void clearDepth(float val);
  void clearDepthStencil(float depth, int stencil);

  explicit operator bool() const;

private:
  GLuint m_handle;
  std::vector<internal::ColorAttPoint> m_colorAtts;
  GLuint m_depthAtt;
  GLuint m_depthStencilAtt;
  GLuint m_renderBuffer;
  Vec2i m_size;
  RenderBufferType m_bufferType;
  bool m_needsUpdate;

  static bool s_renderBufferBound;

  void updateState();
  void bind();
  void unbind();
};


class DefaultFrameBuffer : public FrameBufferInterface, public NotCloneable {
public:
  void create(Vec2i size) override final;
  void prime(std::shared_ptr<FrameBufferInterface> fb,
             std::function<void(HotFrameBuffer&)> func) override final;
  void attachColor(int index, const Tex2D& tex) override final;
  void attachDepth(const Tex2D& tex) final;
  void attachDepthStencil(const Tex2D& tex) final;

  void attachRenderBuffer(const RenderBuffer& buf) override final;
  
  int countAttachments() override final;

  void clearColor(int index, Color32fA color);
  void clearDepth(float val);
  void clearDepthStencil(float depth, int stencil);
  
  explicit operator bool() const;
};

} // namespace internal


/**
 * @brief Wrapper for either a UserFrameBuffer or the DefaultFrameBuffer 
 *        (provides a layer of indirection)
 * @details If created, it creates a UserFrameBuffer internally
 */
class FrameBuffer : public NotCopyable {
public:
  FrameBuffer();
  FrameBuffer(std::shared_ptr<internal::FrameBufferInterface> fb);

  void create(Vec2i size);
  void prime(std::function<void(HotFrameBuffer&)> func);
  void attachColor(int index, const Tex2D& tex);
  void attachDepth(const Tex2D& tex);
  void attachDepthStencil(const Tex2D& tex);
  void attachRenderBuffer(const RenderBuffer& buf);

  explicit operator bool() const;

  int countAttachments();

  static bool isPrimed();

private:
  std::shared_ptr<internal::FrameBufferInterface> m_fb;
};

}

#include "FrameBuffer.tpp"