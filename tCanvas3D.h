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
/*!\file    tCanvas3D.h
 *
 * \author  Patrick Fleischmann
 * \author  Max Reichardt
 * \author  Tobias Föhst
 *
 * \date    2013-06-13
 *
 * \brief Contains tCanvas3D
 *
 * \b tCanvas3D
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__canvas__tCanvas3D_h__
#define __rrlib__canvas__tCanvas3D_h__

#include "rrlib/canvas/tCanvas.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/math/tMatrix.h"
#include "rrlib/math/tPose3D.h"

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
//! 3D Canvas to paint to.
/*!
 *  This class represents a 3D canvas that shapes etc. can be drawn to.
 *  Currently, it is mainly intended to serialize such shapes/geometry
 *  for visualization in a remote process (such as the fingui).
 *
 *  Regarding usage, it has some similarities with typical GUI toolkit
 *  canvas classes (such as Java AWT's Graphics3D - which is used for
 *  the actual drawing in fingui).
 *
 *  Geometry is painted to tCanvas3D serially and cannot be removed.
 *  When a tCanvas3D is to be reused, Clear() can be used to remove
 *  everything.
 */
class tCanvas3D : public tCanvas
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  inline tCanvas3D();

  /*!
   * Move constructor
   */
  inline tCanvas3D(tCanvas3D && o);

  /*!
   * Move assignment
   */
  inline tCanvas3D& operator=(tCanvas3D && o);

  /*!
   * Set affine transformation of all following operations
   * Overwrites current transform completely.
   * Should only be used when this is not a problem
   * (=> when this code fragment is never used on top of another transformation)
   */
  template <typename T>
  void SetTransformation(const math::tMatrix<4, 4, T> &transformation);

  inline void SetTransformation(const math::tPose3D &transformation);

  /*!
   * Applies an affine transform to the Canvas' current transformation.
   * (according to the rule last-specified-first-applied)
   */
  template <typename T>
  void Transform(const math::tMatrix<4, 4, T> &transformation);

  inline void Transform(const math::tPose3D &transformation);

  /*!
   * Applies a translation transform to the Canvas' current transformation.
   */
  template <typename T>
  void Translate(T x, T y, T z);

  template <typename T>
  void Translate(const math::tVector<3, T> &vector);

  /*!
   * Applies a rotation transform to the Canvas' current transformation. // FIXME: Better names for arguments
   */
  template <typename T>
  void Rotate(T x, T y, T z);

  /*!
   * Applies a scaling transform to the Canvas' current transformation.
   */
  template <typename T>
  void Scale(T x, T y, T z);

  template <typename T>
  inline void Scale(const math::tVector<3, T> &v);

  /*!
   * Set Z Coordinate for 2D-Shapes painted to canvas
   */
  template <typename T>
  void SetZ(T z);

  /*!
   * Draw Point
   */
  template <typename T>
  void DrawPoint(T x, T y, T z);

  template <typename T>
  inline void DrawPoint(const math::tVector<3, T> &position);

  /*!
   * Draw Line
   */
  template <typename T>
  void DrawLine(T support_x, T support_y, T support_z, T direction_x, T direction_y, T direction_z);

  template <typename T>
  inline void DrawLine(const math::tVector<3, T> &support, const math::tVector<3, T> &direction);

  /*!
   * Draw Line Segment
   */
  template <typename T>
  void DrawLineSegment(T p1_x, T p1_y, T p1_z, T p2_x, T p2_y, T p2_z);

  template <typename T>
  inline void DrawLineSegment(const math::tVector<3, T> &p1, const math::tVector<3, T> &p2);

  /*!
   * Draw Line Strip
   */
  template <typename TIterator>
  void DrawLineStrip(TIterator points_begin, TIterator points_end);

  template <typename TElement, typename ... TVectors>
  void DrawLineStrip(const math::tVector<3, TElement> &p1, const math::tVector<3, TElement> &p2, const TVectors &... rest);

  /*!
   * Draw arrow
   */
  template <typename T>
  void DrawArrow(T start_x, T start_y, T start_z, T end_x, T end_y, T end_z, bool undirected = false);

  template <typename T>
  inline void DrawArrow(const math::tVector<3, T> &start, const math::tVector<3, T> &end, bool undirected = false);

  /*!
   * Draw Box
   */
  template <typename T>
  void DrawBox(T bottom_left_x, T bottom_left_y, T bottom_left_z, T width, T height = -1, T depth = -1);

  template <typename T>
  inline void DrawBox(const math::tVector<3, T> &bottom_left, T width, T height = -1, T depth = -1);

  /*!
   * Draw Ellipsoid
   */
  template <typename T>
  void DrawEllipsoid(T center_x, T center_y, T center_z, T width, T height = -1, T depth = -1);

  template <typename T>
  inline void DrawEllipsoid(const math::tVector<3, T> &center, T width, T height = -1, T depth = -1);

  /*!
   * Draw Bezier curve
   */
  template <typename TIterator>
  void DrawBezierCurve(TIterator points_begin, TIterator points_end);

  template <typename TElement, typename ... TVectors>
  void DrawBezierCurve(const math::tVector<3, TElement> &p1, const math::tVector<3, TElement> &p2, const TVectors &... rest);

  /*!
   * Draw Polygon
   */
  template <typename TIterator>
  void DrawPolygon(TIterator points_begin, TIterator points_end);

  template <typename TElement, typename ... TVectors>
  void DrawPolygon(const math::tVector<3, TElement> &p1, const math::tVector<3, TElement> &p2, const TVectors &... rest);

  /*!
   * Draw 3D Text
   */
  template <typename T, typename S>
  void DrawText(T x, T y, T z, const S& text);

  template <typename T, typename S>
  void DrawText(const math::tVector<3, T> &position, const S &text);

  /*!
   * Draw 2D Text
   */
  template <typename T, typename S>
  void DrawText(T x, T y, const S& text);

  template <typename T, typename S>
  inline void DrawText(const math::tVector<2, T> &position, const S &text);

  /*!
   * Draw Point Cloud
   */
  template <typename TIterator>
  void DrawPointCloud(TIterator points_begin, TIterator points_end);

  template <typename TElement, typename ... TVectors>
  void DrawPointCloud(const math::tVector<3, TElement> &p1, const math::tVector<3, TElement> &p2, const TVectors &... rest);

  /*!
   * Draw Colored Point Cloud
   */
  template <typename TIterator>
  void DrawColoredPointCloud(TIterator points_begin, TIterator points_end);

  template <typename TElement, typename ... TVectors>
  void DrawColoredPointCloud(const math::tVector<6, TElement> &p1, const math::tVector<6, TElement> &p2, const TVectors &... rest);


  /*!
   * Start a path (of lines and curves)
   * Path ends when any of the above methods is called
   *
   * Specified position is start of path
   */
  template <typename T>
  void StartPath(T x, T y, T z);

  template <typename T>
  inline void StartPath(const math::tVector<3, T> &p);

  /*!
   * Start a shape (with an edge of lines and curves)
   * Shape edge ends when any of the above methods is called.
   * Edge is closed automatically: start and end points are connected by a line.
   *
   * Specified position is start of edge
   */
  template <typename T>
  void StartShape(T x, T y, T z);

  template <typename T>
  inline void StartShape(const math::tVector<3, T> &p);

  /*!
   * Append a line to the specified point to the current path or shape edge.
   * (Only valid after having started a path or shape)
   */
  template <typename T>
  void AppendLineSegment(T x, T y, T z);

  template <typename T>
  inline void AppendLineSegment(const math::tVector<3, T> &p);

  /*!
   * Append a quadratic curve to the current path or shape edge.
   * Point 1 is a control points.
   * The curves's destination will be point 2.
   * (Only valid after having started a path or shape)
   */
  template <typename T>
  void AppendQuadraticBezierCurve(T p1_x, T p1_y, T p1_z, T p2_x, T p2_y, T p2_z);

  template <typename T>
  inline void AppendQuadraticBezierCurve(const math::tVector<3, T> &p1, const math::tVector<3, T> &p2);

  /*!
   * Append a cubic bezier curve to the current path or shape edge.
   * Points 1 and 2 are control points.
   * The curves's destination will be point 3.
   * (Only valid after having started a path or shape)
   */
  template <typename T>
  void AppendCubicBezierCurve(T p1_x, T p1_y, T p1_z, T p2_x, T p2_y, T p2_z, T p3_x, T p3_y, T p3_z);

  template <typename T>
  inline void AppendCubicBezierCurve(const math::tVector<3, T>& p1, const math::tVector<3, T>& p2, const math::tVector<3, T>& p3);

  //----------------------------------------------------------------------
  // Private fields and methods
  //----------------------------------------------------------------------
private:


};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#include "rrlib/canvas/tCanvas3D.hpp"

#endif
