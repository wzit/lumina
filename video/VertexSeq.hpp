#pragma once

#include "VertexSeq.fpp"
#include "HotVertexSeq.fpp"
#include "HotProgram.fpp"
#include "GLObject.hpp"
#include "../config/BaseProxy.hpp"

#include <GL/glew.h>
#include <cstddef>
#include <memory>
#include <iostream>


namespace lumina {
// =============================================================================
// Definition of VertexSeq
// =============================================================================
/** Represents a sequence of vertices.
* VertexSeq represents a sequence of vertices with arbitrary attributes, which 
* may be indexed. It uses an OpenGL vertex buffer and an optional index buffer.
*  
* The typical creation cycle is: 
* * Creating a new instance of VertexSeq (which is useless on it's own)
* * Call `create` to create the underlying data structures
* * Call `prime` to obtain a HotVertexSeq (the VertexSeq needs to be 
* created first)
*
* \see HotVertexSeq
* \see HotVertexSeq<>
*/
class VertexSeq : public GLObject {
public:
  // default constructor
  VertexSeq();

  // copy constructor and copy assignment operator
  VertexSeq(const VertexSeq& copy);
  VertexSeq &operator=(const VertexSeq &copy);

  // move constructor and move assignment operator
  VertexSeq(VertexSeq&& m);
  VertexSeq& operator=(VertexSeq&& m);

  // destructor
  ~VertexSeq();

  /** Creates internal data structures.
   * Creates a OpenGL vertex buffer (VBO), a vertex array object (VAO),
   * and an optional index buffer when indexCount is not 0.
   * \note You cannot call this while another VertexSeq is primed!
   * 
   * @param vertexSize The size of one vertex in number of GLfloats. If your
   * vertex contains just a position of three floats (x,y,z) the size is 3.
   * @param vertexCount The number of vertices this sequence should hold. If 
   * you want to represent a triangle, you need 3 vertices.
   * @param indexCount The number of indicies in the index buffer (0 if you
   * don't need an index buffer)
   */
  void create(uint16_t vertexSize,
              uint32_t vertexCount,
              uint32_t indexCount = 0);

  /** Primes the VertexSeq to obtain a HotVertexSeq.
  * Binds all OpenGL resources in order to use (writing data) the VertexSeq.
  * \note You cannot call prime while another VertexSeq is primed!
  *
  * \param lambda A functor that accepts a HotVertexSeq& as parameter. It will 
  * be called after the HotVertexSeq is created.
  * \tparam Cs A list of types of which a vertex consists. If your vertices 
  * contain a 3D-Position and a 2D-UV-Coordinate, use those 
  * templates: Vec3f, Vec2f
  */
  template <typename... Cs, typename L>
  void prime(L lambda);

  /** Primes the VertexSeq to obtain a HotVertexSeq.
  * Binds all OpenGL resources in order to use (writing data) the VertexSeq.
  * This is the type-unsafe overload of prime. Whenever you can, you should 
  * use the type-safe variant.
  * \note You cannot call prime while another VertexSeq is primed!
  *
  * \param func A functor that accepts a HotVertexSeq<>& as parameter. It will 
  * be called after the HotVertexSeq is created.
  */
  void prime(std::function<void(HotVertexSeq<>&)> func);

  /// Returns the native OpenGL handle of the vertex buffer
  GLuint nativeVertexHandle() const;

  /// Returns the native OpenGL handle of the index buffer
  GLuint nativeIndexHandle() const;

  /// Returns the native OpenGL handle of the vertex array object
  GLuint nativeVAOHandle() const;


  /// Returns the number of vertices in the sequence
  int size() const;

  /// Determines if a vertex layout is active
  bool isVertexLayoutActive() const;

  static void setupOpenGL();

private:
  GLuint m_vertexHandle;
  GLuint m_indexHandle;
  GLuint m_vertexArrayObject;
  uint32_t m_vertexCount;
  uint32_t m_indexCount;
  uint16_t m_vertexSize;
  bool m_layoutActive;

  static bool s_isPrimed;

  void bindAll();
  void unbindAll();

  void bindVAO() const;
  void unbindVAO() const;

  friend internal::HotVertexSeqBase;
  friend HotProgram;
  template <typename... Cs> friend class HotVertexSeq;
};

} // namespace lumina

// include out of line definitions
#include "VertexSeq.tpp"