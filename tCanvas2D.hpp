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
/*!\file    tCanvas2D.hpp
 *
 * \author  Max Reichardt
 * \author  Tobias Föhst
 *
 * \date    2012-01-13
 *
 */
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/logging/messages.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

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
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tCanvas2D constructors
//----------------------------------------------------------------------
tCanvas2D::tCanvas2D()
  : in_path_mode(false)
{}

//----------------------------------------------------------------------
// tCanvas2D SetTransformation
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::SetTransformation(const math::tMatrix<3, 3, T> &t)
{
  T values[] = { t[0][0], t[1][0], t[0][1], t[1][1], t[0][2], t[1][2] };
  this->AppendCommand(eSET_TRANSFORMATION, values, 6);
}

inline void tCanvas2D::SetTransformation(const math::tPose2D &transformation)
{
  this->SetTransformation(transformation.GetTransformationMatrix());
}

//----------------------------------------------------------------------
// tCanvas2D Transform
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::Transform(const math::tMatrix<3, 3, T> &t)
{
  T values[] = { t[0][0], t[1][0], t[0][1], t[1][1], t[0][2], t[1][2] };
  this->AppendCommand(eTRANSFORM, values, 6);
}

inline void tCanvas2D::Transform(const math::tPose2D &transformation)
{
  this->Transform(transformation.GetTransformationMatrix());
}

//----------------------------------------------------------------------
// tCanvas2D Translate
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::Translate(T x, T y)
{
  T values[] = { x, y };
  this->AppendCommand(eTRANSLATE, values, 2);
}

template <typename T>
void tCanvas2D::Translate(const math::tVector<2, T> &v)
{
  this->AppendCommand(eTRANSLATE, reinterpret_cast<const T *>(&v), 2);
}

//----------------------------------------------------------------------
// tCanvas2D Rotate
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::Rotate(T x)
{
  AppendCommand(eROTATE, &x, 1);
}

//----------------------------------------------------------------------
// tCanvas2D Scale
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::Scale(T x, T y)
{
  T values[] = { x, y };
  this->AppendCommand(eSCALE, values, 2);
}

template <typename T>
void tCanvas2D::Scale(const math::tVector<2, T> &v)
{
  this->AppendCommand(eSCALE, reinterpret_cast<const T *>(&v), 2);
}

//----------------------------------------------------------------------
// tCanvas2D SetZ
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::SetZ(T z)
{
  this->AppendCommand(eSET_Z, &z, 1);
}

//----------------------------------------------------------------------
// tCanvas2D DrawPoint
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::DrawPoint(T x, T y)
{
  this->in_path_mode = false;
  T values[] = { x, y };
  this->AppendCommand(eDRAW_POINT, values, 2);
}

template <typename T>
void tCanvas2D::DrawPoint(const math::tVector<2, T> &v)
{
  this->AppendCommand(eDRAW_POINT, reinterpret_cast<const T *>(&v), 2);
}

//----------------------------------------------------------------------
// tCanvas2D DrawLine
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::DrawLine(T x1, T y1, T x2, T y2)
{
  this->in_path_mode = false;
  T values[] = { x1, y1, x2, y2 };
  this->AppendCommand(eDRAW_LINE, values, 4);
}

template <typename T>
void tCanvas2D::DrawLine(const math::tVector<2, T> &p1, const math::tVector<2, T> &p2)
{
  this->DrawLine(p1.X(), p1.Y(), p2.X(), p2.Y());
}

//----------------------------------------------------------------------
// tCanvas2D DrawLineSegment
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::DrawLineSegment(T x1, T y1, T x2, T y2)
{
  this->in_path_mode = false;
  T values[] = { x1, y1, x2, y2 };
  this->AppendCommand(eDRAW_LINE_SEGMENT, values, 4);
}

template <typename T>
void tCanvas2D::DrawLineSegment(const math::tVector<2, T> &p1, const math::tVector<2, T> &p2)
{
  this->DrawLine(p1.X(), p1.Y(), p2.X(), p2.Y());
}

//----------------------------------------------------------------------
// tCanvas2D DrawBox
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::DrawBox(T top_left_x, T top_left_y, T width, T height)
{
  this->in_path_mode = false;
  T values[] = { top_left_x, top_left_y, width, height };
  this->AppendCommand(eDRAW_BOX, values, 4);
}

template <typename T>
void tCanvas2D::DrawBox(const math::tVector<2, T> &top_left, T width, T height)
{
  this->DrawBox(top_left.X(), top_left.Y(), width, height);
}

//----------------------------------------------------------------------
// tCanvas2D DrawEllipsoid
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::DrawEllipsoid(T center_x, T center_y, T width, T height)
{
  this->in_path_mode = false;
  T values[] = { center_x - width / 2, center_y - height / 2, width, height };
  this->AppendCommand(eDRAW_ELLIPSOID, values, 4);
}

template <typename T>
void tCanvas2D::DrawEllipsoid(const math::tVector<2, T> &center_position, T width, T height)
{
  this->DrawEllipsoid(center_position.X(), center_position.Y(), width, height);
}

//----------------------------------------------------------------------
// tCanvas2D DrawPolygon
//----------------------------------------------------------------------
template <typename TIterator>
void tCanvas2D::DrawPolygon(TIterator points_begin, TIterator points_end)
{
  this->in_path_mode = false;
  this->AppendCommandRaw(eDRAW_POLYGON);
  this->Stream().WriteShort(std::distance(points_begin, points_end));
  this->AppendData(points_begin, points_end);
}

//----------------------------------------------------------------------
// tCanvas2D DrawSpline
//----------------------------------------------------------------------
template <typename TIterator>
void tCanvas2D::DrawSpline(TIterator points_begin, TIterator points_end, float tension)
{
  this->in_path_mode = false;
  this->AppendCommandRaw(eDRAW_SPLINE);
  this->Stream().WriteFloat(tension);
  this->Stream().WriteShort(std::distance(points_begin, points_end));
  this->AppendData(points_begin, points_end);
}

//----------------------------------------------------------------------
// tCanvas2D DrawCubicBezierCurve
//----------------------------------------------------------------------
template <typename TIterator>
void tCanvas2D::DrawCubicBezierCurve(TIterator points_begin, TIterator points_end)
{
  this->in_path_mode = false;
  assert(std::distance(points_begin, points_end) == 4);
  this->AppendCommandRaw(eDRAW_CUBIC_BEZIER_CURVE);
  this->AppendData(points_begin, points_end);
}

template <typename T>
void tCanvas2D::DrawCubicBezierCurve(const math::tVector<2, T> &p1, const math::tVector<2, T> &p2, const math::tVector<2, T> &p3, const math::tVector<2, T> &p4)
{
  const math::tVector<2, T> buffer[] = { p1, p2, p3, p4 };
  this->DrawCubicBezierCurve(buffer, buffer + 4);
}

//----------------------------------------------------------------------
// tCanvas2D StartPath
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::StartPath(T x, T y)
{
  T values[] = { x, y };
  this->AppendCommand(ePATH_START, values, 2);
  this->Stream().WriteBoolean(false);
  this->in_path_mode = true;
}

template <typename T>
void tCanvas2D::StartPath(const math::tVector<2, T> &p)
{
  this->StartPath(p.X(), p.Y());
}

//----------------------------------------------------------------------
// tCanvas2D StartShape
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::StartShape(T x, T y)
{
  T values[] = { x, y };
  this->AppendCommand(ePATH_START, values, 2);
  this->Stream().WriteBoolean(true);
  this->in_path_mode = true;
}

template <typename T>
void tCanvas2D::StartShape(const math::tVector<2, T> &p)
{
  this->StartShape(p.X(), p.Y());
}

//----------------------------------------------------------------------
// tCanvas2D AppendLineSegment
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::AppendLineSegment(T x, T y)
{
  if (!this->in_path_mode)
  {
    RRLIB_LOG_PRINT(logging::eLL_ERROR, "Not in path mode. Command has no effect.");
    return;
  }
  T values[] = { x, y };
  this->AppendCommand(ePATH_LINE, values, 2);
}

template <typename T>
void tCanvas2D::AppendLineSegment(const math::tVector<2, T>& v)
{
  this->AppendLineSegment(v.X(), v.Y());
}

//----------------------------------------------------------------------
// tCanvas2D AppendQuadraticBezierCurve
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::AppendQuadraticBezierCurve(T x1, T y1, T x2, T y2)
{
  if (!this->in_path_mode)
  {
    RRLIB_LOG_PRINT(logging::eLL_ERROR, "Not in path mode. Command has no effect.");
    return;
  }
  T values[] = { x1, y1, x2, y2 };
  this->AppendCommand(ePATH_QUADRATIC_BEZIER_CURVE, values, 4);
}

template <typename T>
void tCanvas2D::AppendQuadraticBezierCurve(const math::tVector<2, T> &p1, const math::tVector<2, T> &p2)
{
  this->AppendQuadraticBezierCurve(p1.X(), p1.Y(), p2.X(), p2.Y());
}

//----------------------------------------------------------------------
// tCanvas2D AppendCubicBezierCurve
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::AppendCubicBezierCurve(T x1, T y1, T x2, T y2, T x3, T y3)
{
  if (!this->in_path_mode)
  {
    RRLIB_LOG_PRINT(logging::eLL_ERROR, "Not in path mode. Command has no effect.");
    return;
  }
  T values[] = { x1, y1, x2, y2, x3, y3 };
  this->AppendCommand(ePATH_CUBIC_BEZIER_CURVE, values, 6);
}

template <typename T>
void tCanvas2D::AppendCubicBezierCurve(const math::tVector<2, T>& p1, const math::tVector<2, T>& p2, const math::tVector<2, T>& p3)
{
  this->AppendCubicBezierCurve(p1.X(), p1.Y(), p2.X(), p2.Y(), p3.X(), p3.Y());
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
