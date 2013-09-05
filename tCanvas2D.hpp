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
#include "rrlib/util/variadic_templates.h"

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
{}

tCanvas2D::tCanvas2D(tCanvas2D && o)
{
  tCanvas(std::forward<tCanvas>(o)),
          std::swap(entering_path_mode, o.entering_path_mode);
  std::swap(in_path_mode, o.in_path_mode);
}

//----------------------------------------------------------------------
// tCanvas2D operator=
//----------------------------------------------------------------------
tCanvas2D& tCanvas2D::operator=(tCanvas2D && o)
{
  tCanvas::operator=(std::forward<tCanvas>(o));
  std::swap(entering_path_mode, o.entering_path_mode);
  std::swap(in_path_mode, o.in_path_mode);
  return *this;
}

//----------------------------------------------------------------------
// tCanvas2D SetTransformation
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::SetTransformation(const math::tMatrix<3, 3, T> &transformation)
{
  // FIXME: Why this order? Check with 3D version and use one matrix order for all
  T values[] =
  {
    transformation[0][0], transformation[1][0],
    transformation[0][1], transformation[1][1],
    transformation[0][2], transformation[1][2]
  };
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
void tCanvas2D::Transform(const math::tMatrix<3, 3, T> &transformation)
{
  T values[] =
  {
    transformation[0][0], transformation[1][0],
    transformation[0][1], transformation[1][1],
    transformation[0][2], transformation[1][2]
  };
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
void tCanvas2D::Translate(const math::tVector<2, T> &vector)
{
  this->Translate(vector.X(), vector.Y());
}

//----------------------------------------------------------------------
// tCanvas2D Rotate
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::Rotate(T angle)
{
  AppendCommand(eROTATE, &angle, 1);
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
void tCanvas2D::Scale(const math::tVector<2, T> &factor)
{
  this->Scale(factor.X(), factor.Y());
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
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { x, y };
  this->AppendCommand(eDRAW_POINT, values, 2);
}

template <typename T>
void tCanvas2D::DrawPoint(const math::tVector<2, T> &position)
{
  this->DrawPoint(position.X(), position.Y());
}

//----------------------------------------------------------------------
// tCanvas2D DrawLine
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::DrawLine(T support_x, T support_y, T direction_x, T direction_y)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { support_x, support_y, direction_x, direction_y };
  this->AppendCommand(eDRAW_LINE, values, 4);
}

template <typename T>
void tCanvas2D::DrawLine(const math::tVector<2, T> &support, const math::tVector<2, T> &direction)
{
  this->DrawLine(support.X(), support.Y(), direction.X(), direction.Y());
}

//----------------------------------------------------------------------
// tCanvas2D DrawLineSegment
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::DrawLineSegment(T p1_x, T p1_y, T p2_x, T p2_y)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { p1_x, p1_y, p2_x, p2_y };
  this->AppendCommand(eDRAW_LINE_SEGMENT, values, 4);
}

template <typename T>
void tCanvas2D::DrawLineSegment(const math::tVector<2, T> &p1, const math::tVector<2, T> &p2)
{
  this->DrawLineSegment(p1.X(), p1.Y(), p2.X(), p2.Y());
}

//----------------------------------------------------------------------
// tCanvas2D DrawLineStrip
//----------------------------------------------------------------------
template<typename TIterator>
void tCanvas2D::DrawLineStrip(TIterator points_begin, TIterator points_end)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  this->AppendCommandRaw(eDRAW_LINE_STRIP);
  this->Stream().WriteShort(std::distance(points_begin, points_end));
  this->AppendData(points_begin, points_end);
}

template<typename TElement, typename ... TVectors>
void tCanvas2D::DrawLineStrip(const math::tVector<2, TElement> &p1, const math::tVector<2, TElement> &p2, const TVectors &... rest)
{
  typedef math::tVector<2, TElement> tVector;
  const size_t number_of_points = 2 + sizeof...(rest);

  tVector buffer[number_of_points];
  tVector *p = buffer;
  util::ProcessVariadicValues([p](const tVector & value) mutable
  { *p = value; ++p;}, p1, p2, rest...);

  this->DrawLineStrip(buffer, buffer + number_of_points);
}

//----------------------------------------------------------------------
// tCanvas2D DrawArrow
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::DrawArrow(T start_x, T start_y, T end_x, T end_y, bool undirected)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { start_x, start_y, end_x, end_y };
  this->AppendCommandRaw(eDRAW_ARROW);
  this->Stream().WriteBoolean(undirected);
  this->AppendData(values, values + 4);
}

template <typename T>
void tCanvas2D::DrawArrow(const math::tVector<2, T> &start, const math::tVector<2, T> &end, bool undirected)
{
  this->DrawArrow(start.X(), start.Y(), end.X(), end.Y(), undirected);
}

//----------------------------------------------------------------------
// tCanvas2D DrawBox
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::DrawBox(T bottom_left_x, T bottom_left_y, T width, T height)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { bottom_left_x, bottom_left_y, width, height };
  this->AppendCommand(eDRAW_BOX, values, 4);
}

template <typename T>
void tCanvas2D::DrawBox(const math::tVector<2, T> &bottom_left, T width, T height)
{
  this->DrawBox(bottom_left.X(), bottom_left.Y(), width, height);
}

//----------------------------------------------------------------------
// tCanvas2D DrawEllipsoid
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::DrawEllipsoid(T center_x, T center_y, T width, T height)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  if (height == -1)
  {
    height = width;
  }
  T values[] = { center_x - width / 2, center_y - height / 2, width, height };
  this->AppendCommand(eDRAW_ELLIPSOID, values, 4);
}

template <typename T>
void tCanvas2D::DrawEllipsoid(const math::tVector<2, T> &center, T width, T height)
{
  this->DrawEllipsoid(center.X(), center.Y(), width, height);
}

//----------------------------------------------------------------------
// tCanvas2D DrawBezierCurve
//----------------------------------------------------------------------
template <typename TIterator>
void tCanvas2D::DrawBezierCurve(TIterator points_begin, TIterator points_end)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  assert(std::distance(points_begin, points_end) > 1);
  this->AppendCommandRaw(eDRAW_BEZIER_CURVE);
  this->Stream().WriteShort(std::distance(points_begin, points_end) - 1);
  this->AppendData(points_begin, points_end);
}

template <typename TElement, typename ... TVectors>
void tCanvas2D::DrawBezierCurve(const math::tVector<2, TElement> &p1, const math::tVector<2, TElement> &p2, const TVectors &... rest)
{
  typedef math::tVector<2, TElement> tVector;
  const size_t number_of_points = 2 + sizeof...(rest);

  tVector buffer[number_of_points];
  tVector *p = buffer;
  util::ProcessVariadicValues([p](const tVector & value) mutable { *p = value; ++p; }, p1, p2, rest...);

  this->DrawBezierCurve(buffer, buffer + number_of_points);
}

//----------------------------------------------------------------------
// tCanvas2D DrawPolygon
//----------------------------------------------------------------------
template <typename TIterator>
void tCanvas2D::DrawPolygon(TIterator points_begin, TIterator points_end)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  this->AppendCommandRaw(eDRAW_POLYGON);
  this->Stream().WriteShort(std::distance(points_begin, points_end));
  this->AppendData(points_begin, points_end);
}

template <typename TElement, typename ... TVectors>
void tCanvas2D::DrawPolygon(const math::tVector<2, TElement> &p1, const math::tVector<2, TElement> &p2, const TVectors &... rest)
{
  typedef math::tVector<2, TElement> tVector;
  const size_t number_of_points = 2 + sizeof...(rest);

  tVector buffer[number_of_points];
  tVector *p = buffer;
  util::ProcessVariadicValues([p](const tVector & value) mutable { *p = value; ++p; }, p1, p2, rest...);

  this->DrawPolygon(buffer, buffer + number_of_points);
}

//----------------------------------------------------------------------
// tCanvas2D DrawSpline
//----------------------------------------------------------------------
template <typename TIterator>
void tCanvas2D::DrawSpline(TIterator points_begin, TIterator points_end, float tension)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  this->AppendCommandRaw(eDRAW_SPLINE);
  this->Stream().WriteFloat(tension);
  this->Stream().WriteShort(std::distance(points_begin, points_end));
  this->AppendData(points_begin, points_end);
}

//----------------------------------------------------------------------
// tCanvas2D StartPath
//----------------------------------------------------------------------
template <typename T>
void tCanvas2D::StartPath(T x, T y)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  T values[] = { x, y };
  this->AppendCommand(ePATH_START, values, 2);
  this->Stream().WriteBoolean(false);
  this->entering_path_mode = true;
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
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  T values[] = { x, y };
  this->AppendCommand(ePATH_START, values, 2);
  this->Stream().WriteBoolean(true);
  this->entering_path_mode = true;
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
    RRLIB_LOG_PRINT(ERROR, "Not in path mode. Command has no effect.");
    return;
  }
  this->entering_path_mode = false;
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
void tCanvas2D::AppendQuadraticBezierCurve(T p1_x, T p1_y, T p2_x, T p2_y)
{
  if (!this->in_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Not in path mode. Command has no effect.");
    return;
  }
  this->entering_path_mode = false;
  T values[] = { p1_x, p1_y, p2_x, p2_y };
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
void tCanvas2D::AppendCubicBezierCurve(T p1_x, T p1_y, T p2_x, T p2_y, T p3_x, T p3_y)
{
  if (!this->in_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Not in path mode. Command has no effect.");
    return;
  }
  this->entering_path_mode = false;
  T values[] = { p1_x, p1_y, p2_x, p2_y, p3_x, p3_y };
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
