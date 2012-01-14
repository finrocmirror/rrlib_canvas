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
 * \author  Max Reichardt
 *
 * \date    2012-01-13
 *
 * \brief Contains tCanvas3D
 *
 * \b tCanvas3D
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__canvas__tCanvas3D_h__
#define __rrlib__canvas__tCanvas3D_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/canvas/tCanvas.h"

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
 *  This class represents a (very innovative ;-) ) 3D canvas
 *  that objects etc. can be added to.
 *  Currently, it is mainly intended to serialize such objects
 *  for visualization in a remote process (such as the fingui).
 *
 *  Geometry is painted to tCanvas3D serially and cannot be removed.
 *  When a tCanvas3D is to be reused, Clear() can be used to remove
 *  everything.
 */
class tCanvas3D : public tCanvas
{
//----------------------------------------------------------------------
// Public methods and typedefs (no fields/variables)
//----------------------------------------------------------------------
public:

  tCanvas3D() {}

  virtual ~tCanvas3D() {}

  /*!
   * Applies a rotation transform to the Canvas' current transformation.
   */
  template <typename T>
  void Rotate(T x)
  {
    tCanvas::Rotate(x);
  }
  template <typename T>
  void Rotate(const rrlib::math::tVector<3, T>& r)
  {
    T values[] = { r[0], r[1], r[2] };
    AppendCommand(eROTATE_3D, values, 3);
  }

  /*!
   * Applies a scaling transform to the Canvas' current transformation.
   */
  template <typename T>
  void Scale(T x, T y)
  {
    tCanvas::Scale(x, y);
  }
  template <typename T>
  void Scale(T x, T y, T z)
  {
    T values[] = { x, y, z };
    AppendCommand(eSCALE_3D, values, 3);
  }
  template <typename T>
  void Scale(const rrlib::math::tVector<2, T>& v)
  {
    tCanvas::Scale(v);
  }
  template <typename T>
  void Scale(const rrlib::math::tVector<3, T>& v)
  {
    AppendCommand(eSCALE_3D, reinterpret_cast<T*>(&v), 3);
  }


  /*!
   * Draw Line
   */
  template <typename T>
  void DrawLine(const rrlib::math::tVector<2, T>& p1, const rrlib::math::tVector<2, T>& p2)
  {
    tCanvas::DrawLine(p1, p2);
  }
  template <typename T>
  void DrawLine(const rrlib::math::tVector<3, T>& p1, const rrlib::math::tVector<3, T>& p2)
  {
    T values[] = { p1[0], p1[1], p1[2], p2[0], p2[1], p2[2] };
    AppendCommand(eDRAW_LINE_3D, values, 6);
  }

  /*!
   * Draw 3D Polygon
   */
  template <typename T>
  void DrawPolygon(const std::vector<rrlib::math::tVector<2, T>>& points)
  {
    tCanvas::DrawPolygon(points);
  }
  template <typename T>
  void DrawPolygon(const rrlib::math::tVector<2, T>* points, size_t point_count)
  {
    tCanvas::DrawPolygon(points, point_count);
  }
  template <typename T>
  void DrawPolygon(const std::vector<rrlib::math::tVector<3, T>>& points)
  {
    DrawPolygon(&(points[0]), points.size());
  }
  template <typename T>
  void DrawPolygon(const rrlib::math::tVector<3, T>* points, size_t point_count)
  {
    stream.WriteShort(point_count);
    AppendCommand(eDRAW_POLYGON_3D, points, point_count * 3);
  }


//----------------------------------------------------------------------
// Protected methods (no fields/variables)
//----------------------------------------------------------------------
protected:

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

#endif
