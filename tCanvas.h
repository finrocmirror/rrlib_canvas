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
#include "rrlib/math/tMatrix.h"
#include "rrlib/math/tVector.h"
#include "rrlib/math/tPose2D.h"
#include "rrlib/math/tPose3D.h"
#include "rrlib/serialization/tMemoryBuffer.h"
#include "rrlib/serialization/tOutputStream.h"

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
// Public methods and typedefs (no fields/variables)
//----------------------------------------------------------------------
public:

  tCanvas();

  virtual ~tCanvas() {}

  /*!
   * Clear canvas
   */
  void Clear();

  /*!
   * Set affine transformation of all following operations
   * Overwrites current transform completely.
   * Should only be used when this is not a problem
   * (=> when this code fragment is never used on top of another transformation)
   */
  inline void SetTransform(const rrlib::math::tPose2D& transform)
  {
    SetTransform(transform.GetTransformationMatrix());
  }
  template < typename TElement >
  void SetTransform(const rrlib::math::tMatrix<3, 3, TElement, rrlib::math::matrix::Full>& t)
  {
    TElement values[] = { t[0][0], t[1][0], t[0][1], t[1][1], t[0][2], t[1][2] };
    AppendCommand(eSET_TRANSFORM_2D, values, 6);
  }

  /*!
   * Applies an affine transform to the Canvas' current transformation.
   * (according to the rule last-specified-first-applied)
   */
  inline void Transform(const rrlib::math::tPose2D& transform)
  {
    SetTransform(transform.GetTransformationMatrix());
  }
  template < typename TElement >
  void Transform(const rrlib::math::tMatrix<3, 3, TElement, rrlib::math::matrix::Full>& t)
  {
    TElement values[] = { t[0][0], t[1][0], t[0][1], t[1][1], t[0][2], t[1][2] };
    AppendCommand(eTRANSFORM_2D, values, 6);
  }

  /*!
   * Applies a translation transform to the Canvas' current transformation.
   */
  template <typename T>
  void Translate(T x, T y)
  {
    T values[] = { x, y };
    AppendCommand(eTRANSLATE_2D, values, 2);
  }
  template <typename T>
  void Translate(T x, T y, T z)
  {
    T values[] = { x, y, z };
    AppendCommand(eTRANSLATE_3D, values, 3);
  }
  template <typename T>
  void Translate(const rrlib::math::tVector<2, T>& v)
  {
    AppendCommand(eTRANSLATE_2D, reinterpret_cast<T*>(&v), 2);
  }
  template <typename T>
  void Translate(const rrlib::math::tVector<3, T>& v)
  {
    AppendCommand(eTRANSLATE_3D, reinterpret_cast<T*>(&v), 3);
  }

  /*!
   * Applies a rotation transform to the Canvas' current transformation.
   */
  template <typename T>
  void Rotate(T x)
  {
    AppendCommand(eROTATE_2D, &x, 1);
  }

  /*!
   * Applies a scaling transform to the Canvas' current transformation.
   */
  template <typename T>
  void Scale(T x, T y)
  {
    T values[] = { x, y };
    AppendCommand(eSCALE_2D, values, 2);
  }
  template <typename T>
  void Scale(const rrlib::math::tVector<2, T>& v)
  {
    AppendCommand(eSCALE_2D, reinterpret_cast<T*>(&v), 2);
  }

  /*!
   * Reset Canvas' current transformation (to identity matrix)
   */
  void ResetTransform()
  {
    AppendCommandRaw(eRESET_TRANSFORM);
  }

  /*!
   * Set the canvas' fill and edge color
   */
  void SetColor(uint8_t r, uint8_t g, uint8_t b)
  {
    uint8_t buffer[] = { r, g, b };
    AppendCommandRaw(eSET_COLOR, buffer, 3);
  }

  /*!
   * Set the canvas' color for edges
   */
  void SetEdgeColor(uint8_t r, uint8_t g, uint8_t b)
  {
    uint8_t buffer[] = { r, g, b };
    AppendCommandRaw(eSET_EDGE_COLOR, buffer, 3);
  }

  /*!
   * Set the canvas' color for filling
   */
  void SetFillColor(uint8_t r, uint8_t g, uint8_t b)
  {
    uint8_t buffer[] = { r, g, b };
    AppendCommandRaw(eSET_FILL_COLOR, buffer, 3);
  }

  /*!
   * Should objects be filled?
   */
  void SetFill(bool fill_objects)
  {
    uint8_t buffer = fill_objects ? 1 : 0;
    AppendCommandRaw(eSET_FILL, &buffer, 1);
  }

  /*!
   * Set Z Coordinate for 2D-Shapes painted to canvas
   */
  template <typename T>
  void SetZ(T z)
  {
    AppendCommand(eSET_Z, &z, 1);
  }

  /*!
   * Draw Point
   */
  template <typename T>
  void DrawPoint(T x, T y)
  {
    T values[] = { x, y };
    AppendCommand(eDRAW_POINT_2D, values, 2);
  }
  template <typename T>
  void DrawPoint(T x, T y, T z)
  {
    T values[] = { x, y, z };
    AppendCommand(eDRAW_POINT_3D, values, 3);
  }
  template <typename T>
  void DrawPoint(const rrlib::math::tVector<2, T>& v)
  {
    AppendCommand(eDRAW_POINT_2D, reinterpret_cast<T*>(&v), 2);
  }
  template <typename T>
  void DrawPoint(const rrlib::math::tVector<3, T>& v)
  {
    AppendCommand(eDRAW_POINT_3D, reinterpret_cast<T*>(&v), 3);
  }

  /*!
   * Draw Line
   */
  template <typename T>
  void DrawLine(T x1, T y1, T x2, T y2)
  {
    T values[] = { x1, y1, x2, y2 };
    AppendCommand(eDRAW_LINE_2D, values, 4);
  }
  template <typename T>
  void DrawLine(const rrlib::math::tVector<2, T>& p1, const rrlib::math::tVector<2, T>& p2)
  {
    T values[] = { p1[0], p1[1], p2[0], p2[1] };
    AppendCommand(eDRAW_LINE_2D, values, 4);
  }

  /*!
   * Draw Rectangle
   */
  template <typename T>
  void DrawRectangle(T x, T y, T width, T height)
  {
    T values[] = { x, y, width, height };
    AppendCommand(eDRAW_RECTANGLE, values, 4);
  }
  template <typename T>
  void DrawRectangle(const rrlib::math::tVector<2, T>& top_left, T width, T height)
  {
    T values[] = { top_left[0], top_left[1], width, height };
    AppendCommand(eDRAW_RECTANGLE, values, 4);
  }

  /*!
   * Draw Ellipse
   */
  template <typename T>
  void DrawEllipse(T x, T y, T width, T height)
  {
    T values[] = { x, y, width, height };
    AppendCommand(eDRAW_ELLIPSE, values, 4);
  }
  template <typename T>
  void DrawEllipse(const rrlib::math::tVector<2, T>& top_left, T width, T height)
  {
    T values[] = { top_left[0], top_left[1], width, height };
    AppendCommand(eDRAW_ELLIPSE, values, 4);
  }

  /*!
   * Draw Polygon
   */
  template <typename T>
  void DrawPolygon(const std::vector<rrlib::math::tVector<2, T>>& points)
  {
    DrawPolygon(&(points[0]), points.size());
  }
  template <typename T>
  void DrawPolygon(const rrlib::math::tVector<2, T>* points, size_t point_count)
  {
    stream.WriteShort(point_count);
    AppendCommand(eDRAW_POLYGON_2D, points, point_count * 2);
  }

  /*!
   * Draw Spline
   */
  template <typename T>
  void DrawSpline(const std::vector<rrlib::math::tVector<2, T>>& points)
  {
    DrawPolygon(&(points[0]), points.size());
  }
  template <typename T>
  void DrawSpline(const rrlib::math::tVector<2, T>* points, size_t point_count)
  {
    stream.WriteShort(point_count);
    AppendCommand(eDRAW_SPLINE_2D, points, point_count * 2);
  }

  /*!
   * Draw Text
   */
  template <typename T, typename S>
  void DrawText(T x, T y, const S& text)
  {
    T values[] = { x, y };
    AppendCommand(eDRAW_STRING_2D, values, 3);
    stream.WriteString(text);
  }
  template <typename T, typename S>
  void DrawText(const rrlib::math::tVector<2, T>& v, const S& text)
  {
    DrawText(v[0], v[1], text);
  }

  /*!
   * Draw Cubic bezier curve
   */
  template <typename T>
  void DrawCubicBezierCurve(const rrlib::math::tVector<2, T>& p1, const rrlib::math::tVector<2, T>& p2, const rrlib::math::tVector<2, T>& p3, const rrlib::math::tVector<2, T>& p4)
  {
    const rrlib::math::tVector<2, T> buffer[] = { p1, p2, p3, p4 };
    DrawCubicBezierCurve(buffer);
  }
  template <typename T>
  void DrawCubicBezierCurve(const std::vector<rrlib::math::tVector<2, T>>& points)
  {
    assert(points.size() >= 4);
    AppendCommand(eDRAW_CUBIC_BEZIER_CURVE_2D, &(points[0]), 8);
  }
  template <typename T>
  void DrawCubicBezierCurve(const rrlib::math::tVector<2, T>* points)
  {
    AppendCommand(eDRAW_CUBIC_BEZIER_CURVE_2D, points, 8);
  }

  /*!
   * Start a path (of lines and curves)
   * Path ends when any of the above methods is called
   *
   * Specified position is start of path
   */
  template <typename T>
  void StartPath(T x, T y)
  {
    T values[] = { x, y };
    AppendCommand(ePATH_2D_START, values, 2);
    stream.WriteBoolean(false);
  }
  template <typename T>
  void StartPath(const rrlib::math::tVector<2, T>& p)
  {
    StartPath(p[0], p[1]);
  }

  /*!
   * Start a shape (with an edge of lines and curves)
   * Shape edge ends when any of the above methods is called.
   * Edge is closed automatically: start and end points are connected by a line.
   *
   * Specified position is start of edge
   */
  template <typename T>
  void StartShape(T x, T y)
  {
    T values[] = { x, y };
    AppendCommand(ePATH_2D_START, values, 2);
    stream.WriteBoolean(true);
  }
  template <typename T>
  void StartShape(const rrlib::math::tVector<2, T>& p)
  {
    StartShape(p[0], p[1]);
  }

  /*!
   * Append a line to the specified point to the current path or shape edge.
   * (Only valid after having started a path or shape)
   */
  template <typename T>
  void AppendLineSegment(T x, T y)
  {
    T values[] = { x, y };
    AppendCommand(ePATH_2D_LINE, values, 2);
  }
  template <typename T>
  void AppendLineSegment(const rrlib::math::tVector<2, T>& v)
  {
    AppendLineSegment(v[0], v[1]);
  }

  /*!
   * Append a quadratic curve to the current path or shape edge.
   * Point 1 is a control points.
   * The curves's destination will be point 2.
   * (Only valid after having started a path or shape)
   */
  template <typename T>
  void AppendQuadraticCurve(T x1, T y1, T x2, T y2)
  {
    T values[] = { x1, y1, x2, y2 };
    AppendCommand(ePATH_2D_QUAD_CURVE, values, 4);
  }
  template <typename T>
  void AppendQuadraticCurve(const rrlib::math::tVector<2, T>& p1, const rrlib::math::tVector<2, T>& p2)
  {
    AppendQuadraticCurve(p1[0], p1[1], p2[0], p2[1]);
  }

  /*!
   * Append a cubic bezier curve to the current path or shape edge.
   * Points 1 and 2 are control points.
   * The curves's destination will be point 3.
   * (Only valid after having started a path or shape)
   */
  template <typename T>
  void AppendCubicCurve(T x1, T y1, T x2, T y2, T x3, T y3)
  {
    T values[] = { x1, y1, x2, y2, x3, y3 };
    AppendCommand(ePATH_2D_CUBIC_CURVE, values, 6);
  }
  template <typename T>
  void AppendCubicCurve(const rrlib::math::tVector<2, T>& p1, const rrlib::math::tVector<2, T>& p2, const rrlib::math::tVector<2, T>& p3)
  {
    AppendCubicCurve(p1[0], p1[1], p2[0], p2[1], p3[0], p3[1]);
  }

//----------------------------------------------------------------------
// Protected methods (no fields/variables)
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
  inline void AppendCommand(tCanvasOpCode opcode, T* values, size_t value_count)
  {
    // TODO could be optimized
    stream << ((uint8_t)opcode) << ((uint8_t)tNumberType<T>::value);
    stream.Write(values, value_count * sizeof(T));
  }

  /*!
   * Adds command to canvas data
   *
   * \param opcode Opcode
   * \param buffer Raw byte buffer
   * \param bytes Size in bytes of raw buffer
   */
  void AppendCommandRaw(tCanvasOpCode opcode, void* buffer = NULL, size_t bytes = 0);

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
