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
/*!\file    tCanvas2D.h
 *
 * \author  Max Reichardt
 * \author  Tobias Föhst
 *
 * \date    2012-01-13
 *
 * \brief Contains tCanvas2D
 *
 * \b tCanvas2D
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__canvas__tCanvas2D_h__
#define __rrlib__canvas__tCanvas2D_h__

#include "rrlib/canvas/tCanvas.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/math/tMatrix.h"
#include "rrlib/math/tPose2D.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

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
//! 2D Canvas to paint to.
/*!
 *  This class represents a 2D canvas that shapes etc. can be drawn to.
 *  Currently, it is mainly intended to serialize such shapes/geometry
 *  for visualization in a remote process (such as the fingui).
 *
 *  Regarding usage, it has some similarities with typical GUI toolkit
 *  canvas classes (such as Java AWT's Graphics2D - which is used for
 *  the actual drawing in fingui).
 *
 *  Geometry is painted to tCanvas2D serially and cannot be removed.
 *  When a tCanvas2D is to be reused, Clear() can be used to remove
 *  everything.
 */
class tCanvas2D : public tCanvas
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  inline tCanvas2D();

  /*!
   * Set affine transformation of all following operations
   * Overwrites current transform completely.
   * Should only be used when this is not a problem
   * (=> when this code fragment is never used on top of another transformation)
   */
  template <typename T>
  void SetTransformation(const math::tMatrix<3, 3, T> &t);

  inline void SetTransformation(const math::tPose2D &transformation);

  /*!
   * Applies an affine transform to the Canvas' current transformation.
   * (according to the rule last-specified-first-applied)
   */
  template <typename T>
  void Transform(const math::tMatrix<3, 3, T> &t);

  inline void Transform(const math::tPose2D &transformation);

  /*!
   * Applies a translation transform to the Canvas' current transformation.
   */
  template <typename T>
  void Translate(T x, T y);

  template <typename T>
  void Translate(const math::tVector<2, T> &v);

  /*!
   * Applies a rotation transform to the Canvas' current transformation.
   */
  template <typename T>
  void Rotate(T x);

  /*!
   * Applies a scaling transform to the Canvas' current transformation.
   */
  template <typename T>
  void Scale(T x, T y);

  template <typename T>
  inline void Scale(const math::tVector<2, T> &v);

  /*!
   * Set Z Coordinate for 2D-Shapes painted to canvas
   */
  template <typename T>
  void SetZ(T z);

  /*!
   * Draw Point
   */
  template <typename T>
  void DrawPoint(T x, T y);

  template <typename T>
  inline void DrawPoint(const math::tVector<2, T> &v);

  /*!
   * Draw Line
   */
  template <typename T>
  void DrawLine(T x1, T y1, T x2, T y2);

  template <typename T>
  inline void DrawLine(const math::tVector<2, T> &p1, const math::tVector<2, T> &p2);

  /*!
   * Draw Box
   */
  template <typename T>
  void DrawBox(T top_left_x, T top_left_y, T width, T height);

  template <typename T>
  inline void DrawBox(const math::tVector<2, T> &top_left, T width, T height);

  /*!
   * Draw Ellipsoid
   */
  template <typename T>
  void DrawEllipsoid(T center_x, T center_y, T width, T height);

  template <typename T>
  inline void DrawEllipsoid(const math::tVector<2, T> &center_position, T width, T height);

  /*!
   * Draw Polygon
   */
  template <typename TIterator>
  void DrawPolygon(TIterator points_begin, TIterator points_end);

  /*!
   * Draw Spline
   */
  template <typename TIterator>
  void DrawSpline(TIterator points_begin, TIterator points_end);

  /*!
   * Draw Cubic Bezier curve
   */
  template <typename TIterator>
  void DrawCubicBezierCurve(TIterator points_begin, TIterator points_end);

  template <typename T>
  void DrawCubicBezierCurve(const math::tVector<2, T> &p1, const math::tVector<2, T> &p2, const math::tVector<2, T> &p3, const math::tVector<2, T> &p4);

  /*!
   * Start a path (of lines and curves)
   * Path ends when any of the above methods is called
   *
   * Specified position is start of path
   */
  template <typename T>
  void StartPath(T x, T y);

  template <typename T>
  inline void StartPath(const math::tVector<2, T> &p);

  /*!
   * Start a shape (with an edge of lines and curves)
   * Shape edge ends when any of the above methods is called.
   * Edge is closed automatically: start and end points are connected by a line.
   *
   * Specified position is start of edge
   */
  template <typename T>
  void StartShape(T x, T y);

  template <typename T>
  inline void StartShape(const math::tVector<2, T> &p);

  /*!
   * Append a line to the specified point to the current path or shape edge.
   * (Only valid after having started a path or shape) FIXME: Could be realized in a more secure way by using a proxy object of type tPath
   */
  template <typename T>
  void AppendLineSegment(T x, T y);

  template <typename T>
  inline void AppendLineSegment(const math::tVector<2, T> &v);

  /*!
   * Append a quadratic curve to the current path or shape edge.
   * Point 1 is a control points.
   * The curves's destination will be point 2.
   * (Only valid after having started a path or shape)
   */
  template <typename T>
  void AppendQuadraticCurve(T x1, T y1, T x2, T y2);

  template <typename T>
  inline void AppendQuadraticCurve(const math::tVector<2, T> &p1, const math::tVector<2, T> &p2);

  /*!
   * Append a cubic bezier curve to the current path or shape edge.
   * Points 1 and 2 are control points.
   * The curves's destination will be point 3.
   * (Only valid after having started a path or shape)
   */
  template <typename T>
  void AppendCubicCurve(T x1, T y1, T x2, T y2, T x3, T y3);

  template <typename T>
  inline void AppendCubicCurve(const math::tVector<2, T>& p1, const math::tVector<2, T>& p2, const math::tVector<2, T>& p3);

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  bool in_path_mode;

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#include "rrlib/canvas/tCanvas2D.hpp"

#endif
