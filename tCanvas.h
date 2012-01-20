//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//----------------------------------------------------------------------
/*!\file    tCanvas.h
 *
 * \author  Max Reichardt
 * \author  Tobias Föhst
 *
 * \date    2012-01-09
 *
 * \brief Contains tCanvas
 *
 * \b tCanvas
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__canvas__tCanvas_h__
#define __rrlib__canvas__tCanvas_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <type_traits>

#include "rrlib/serialization/tMemoryBuffer.h"
#include "rrlib/serialization/tOutputStream.h"
#include "rrlib/math/tVector.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/canvas/definitions.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace rrlib
{
namespace canvas
{

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! Canvas to paint to.
/*!
 *  This class represents a canvas that shapes etc. can be drawn to.
 *  Currently, it is mainly intended to serialize such shapes/geometry
 *  for visualization in a remote process (such as the fingui).
 *
 *  Regarding usage, it has some similarities with typical GUI toolkit
 *  canvas classes (such as Java AWT's Graphics2D - which is used for
 *  the actual drawing in fingui).
 *
 *  Geometry is painted to tCanvas serially and cannot be removed.
 *  When a tCanvas is to be reused, Clear() can be used to remove
 *  everything.
 *
 *  This is the base class of tCanvas2D and tCanvas3D.
 *  It has native support for the following geometrical primitives that
 *  make sense in both 2D and 3D:
 *
 *  Points, lines, rectangles, ellipses, polygons, bezier-splines, text
 */
class tCanvas : public rrlib::serialization::tSerializable, public boost::noncopyable
{
//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  tCanvas();

  virtual ~tCanvas() {}

  /*!
   * Clear canvas
   */
  void Clear();

  /*!
   * Reset Canvas' current transformation (to identity matrix)
   */
  void ResetTransformation()
  {
    this->AppendCommandRaw(eRESET_TRANSFORMATION);
  }

  /*!
   * Set the canvas' fill and edge color
   */
  void SetColor(uint8_t r, uint8_t g, uint8_t b)
  {
    uint8_t buffer[] = { r, g, b };
    this->AppendCommandRaw(eSET_COLOR, buffer, sizeof(buffer));
  }
  void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
  {
    this->SetColor(r, g, b);
    this->SetAlpha(a);
  }

  /*!
   * Set the canvas' color for edges
   */
  void SetEdgeColor(uint8_t r, uint8_t g, uint8_t b)
  {
    uint8_t buffer[] = { r, g, b };
    this->AppendCommandRaw(eSET_EDGE_COLOR, buffer, sizeof(buffer));
  }
  void SetEdgeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
  {
    this->SetEdgeColor(r, g, b);
    this->SetAlpha(a);
  }

  /*!
   * Set the canvas' color for filling
   */
  void SetFillColor(uint8_t r, uint8_t g, uint8_t b)
  {
    uint8_t buffer[] = { r, g, b };
    this->AppendCommandRaw(eSET_FILL_COLOR, buffer, sizeof(buffer));
  }
  void SetFillColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
  {
    this->SetFillColor(r, g, b);
    this->SetAlpha(a);
  }

  /*!
   * Should objects be filled?
   */
  void SetFill(bool fill_objects)
  {
    uint8_t buffer = fill_objects ? 1 : 0;
    this->AppendCommandRaw(eSET_FILL, &buffer, 1);
  }

  /*!
   * Set the canvas' alpha channel
   */
  void SetAlpha(uint8_t alpha)
  {
    this->AppendCommandRaw(eSET_ALPHA, &alpha, sizeof(alpha));
  }

  /*!
   * Draw Text
   */
  template <typename T, typename S>
  void DrawText(T x, T y, const S& text)
  {
    T values[] = { x, y };
    AppendCommand(eDRAW_STRING, values, 2);
    stream.WriteString(text);
  }
  template <typename T, typename S>
  void DrawText(const math::tVector<2, T> &position, const S &text)
  {
    this->DrawText(position.X(), position.Y(), text);
  }

//----------------------------------------------------------------------
// Protected methods
//----------------------------------------------------------------------
protected:

  virtual void Serialize(rrlib::serialization::tOutputStream& os) const;

  virtual void Deserialize(rrlib::serialization::tInputStream& is);

  /*!
   * Adds command to canvas data
   *
   * \param opcode Opcode
   * \param values Buffer with values
   * \param value_count Number of values in buffer
   */
  template <typename T>
  inline void AppendCommand(tCanvasOpCode opcode, const T *values, size_t value_count)
  {
    // TODO could be optimized
    this->stream << static_cast<uint8_t>(opcode) << static_cast<uint8_t>(tNumberType<T>::value);
    this->stream.Write(values, value_count * sizeof(T));
  }

  /*!
   * Adds command to canvas data
   *
   * \param opcode Opcode
   * \param buffer Raw byte buffer
   * \param bytes Size in bytes of raw buffer
   */
  void AppendCommandRaw(tCanvasOpCode opcode, void* buffer = NULL, size_t bytes = 0);

  /*!
   * Adds raw data to canvas data
   */
  template <typename TIterator>
  inline void AppendData(TIterator data_begin, TIterator data_end)
  {
    typedef typename std::remove_reference < decltype(*data_begin) >::type tData;
    typedef typename std::conditional<std::is_fundamental<tData>::value, tData, typename tData::tElement>::type tElement;
    this->stream << static_cast<uint8_t>(tNumberType<tElement>::value);
    std::for_each(data_begin, data_end, [this](const tData &vector)
    {
      this->stream.Write(&vector, sizeof(tData));
    });
  }

//----------------------------------------------------------------------
// Protected methods
//----------------------------------------------------------------------
protected:

  inline rrlib::serialization::tOutputStream &Stream()
  {
    return this->stream;
  }

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  /*! Buffer that disposable geometry is serialized to */
  rrlib::serialization::tMemoryBuffer buffer;

  /*! Stream to serialize to disposable geometry buffer */
  rrlib::serialization::tOutputStream stream;
};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
