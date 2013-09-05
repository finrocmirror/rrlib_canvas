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
/*!\file    tCanvas3D.hpp
 *
 * \author  Patrick Fleischmann
 * \author  Max Reichardt
 * \author  Tobias Föhst
 *
 * \date    2013-06-13
 *
 */
//----------------------------------------------------------------------
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
// tCanvas3D constructors
//----------------------------------------------------------------------
tCanvas3D::tCanvas3D()
{
}

tCanvas3D::tCanvas3D(tCanvas3D && o)
{
  tCanvas(std::forward<tCanvas>(o)),
          std::swap(entering_path_mode, o.entering_path_mode);
  std::swap(in_path_mode, o.in_path_mode);
}

//----------------------------------------------------------------------
// tCanvas3D operator=
//----------------------------------------------------------------------
tCanvas3D& tCanvas3D::operator=(tCanvas3D && o)
{
  tCanvas::operator=(std::forward<tCanvas>(o));
  std::swap(entering_path_mode, o.entering_path_mode);
  std::swap(in_path_mode, o.in_path_mode);
  return *this;
}

//----------------------------------------------------------------------
// tCanvas3D SetTransformation
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::SetTransformation(const math::tMatrix<4, 4, T> &transformation)
{
  T values[] =
  {
    transformation[0][0], transformation[0][1], transformation[0][2], transformation[0][3],
    transformation[1][0], transformation[1][1], transformation[1][2], transformation[1][3],
    transformation[2][0], transformation[2][1], transformation[2][2], transformation[2][3],
    transformation[3][0], transformation[3][1], transformation[3][2], transformation[3][3]
  };
  this->AppendCommand(eSET_TRANSFORMATION, values, 16);
}

inline void tCanvas3D::SetTransformation(const math::tPose3D &transformation)
{
  this->SetTransformation(transformation.GetTransformationMatrix());
}

//----------------------------------------------------------------------
// tCanvas3D Transform
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::Transform(const math::tMatrix<4, 4, T> &transformation)
{
  T values[] =
  {
    transformation[0][0], transformation[0][1], transformation[0][2], transformation[0][3],
    transformation[1][0], transformation[1][1], transformation[1][2], transformation[1][3],
    transformation[2][0], transformation[2][1], transformation[2][2], transformation[2][3],
    transformation[3][0], transformation[3][1], transformation[3][2], transformation[3][3]
  };
  this->AppendCommand(eTRANSFORM, values, 16);
}

inline void tCanvas3D::Transform(const math::tPose3D &transformation)
{
  this->Transform(transformation.GetTransformationMatrix());
}

//----------------------------------------------------------------------
// tCanvas3D Translate
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::Translate(T x, T y, T z)
{
  T values[] = { x, y, z };
  this->AppendCommand(eTRANSLATE, values, 3);
}

template<typename T>
void tCanvas3D::Translate(const math::tVector<3, T> &vector)
{
  this->Translate(vector.X(), vector.Y(), vector.Z());
}

//----------------------------------------------------------------------
// tCanvas3D Rotate
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::Rotate(T x, T y, T z)
{
  T values[] = { x, y, z };
  this->AppendCommand(eROTATE, values, 3);
}

//----------------------------------------------------------------------
// tCanvas3D Scale
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::Scale(T x, T y, T z)
{
  T values[] = { x, y, z };
  this->AppendCommand(eSCALE, values, 3);
}

template<typename T>
void tCanvas3D::Scale(const math::tVector<3, T> &vector)
{
  this->Scale(vector.X(), vector.Y(), vector.Z());
}

//----------------------------------------------------------------------
// tCanvas3D SetZ
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::SetZ(T z)
{
  this->AppendCommand(eSET_Z, &z, 1);
}

//----------------------------------------------------------------------
// tCanvas3D DrawPoint
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::DrawPoint(T x, T y, T z)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { x, y, z };
  this->AppendCommand(eDRAW_POINT, values, 3);
}

template<typename T>
void tCanvas3D::DrawPoint(const math::tVector<3, T> &position)
{
  this->DrawPoint(position.X(), position.Y(), position.Z());
}

//----------------------------------------------------------------------
// tCanvas3D DrawLine
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::DrawLine(T support_x, T support_y, T support_z, T direction_x, T direction_y, T direction_z)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { support_x, support_y, support_z, direction_x, direction_y, direction_z };
  this->AppendCommand(eDRAW_LINE, values, 6);
}

template<typename T>
void tCanvas3D::DrawLine(const math::tVector<3, T> &support, const math::tVector<3, T> &direction)
{
  this->DrawLine(support.X(), support.Y(), support.Z(), direction.X(), direction.Y(), direction.Z());
}

//----------------------------------------------------------------------
// tCanvas3D DrawLineSegment
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::DrawLineSegment(T p1_x, T p1_y, T p1_z, T p2_x, T p2_y, T p2_z)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { p1_x, p1_y, p1_z, p2_x, p2_y, p2_z };
  this->AppendCommand(eDRAW_LINE_SEGMENT, values, 6);
}

template<typename T>
void tCanvas3D::DrawLineSegment(const math::tVector<3, T> &p1, const math::tVector<3, T> &p2)
{
  this->DrawLineSegment(p1.X(), p1.Y(), p1.Z(), p2.X(), p2.Y(), p2.Z());
}

//----------------------------------------------------------------------
// tCanvas3D DrawLineStrip
//----------------------------------------------------------------------
template<typename TIterator>
void tCanvas3D::DrawLineStrip(TIterator points_begin, TIterator points_end)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  this->AppendCommandRaw(eDRAW_LINE_STRIP);
  this->Stream().WriteInt(std::distance(points_begin, points_end));
  this->AppendData(points_begin, points_end);
}

template<typename TElement, typename ... TVectors>
void tCanvas3D::DrawLineStrip(const math::tVector<3, TElement> &p1, const math::tVector<3, TElement> &p2, const TVectors &... rest)
{
  typedef math::tVector<3, TElement> tVector;
  const size_t number_of_points = 2 + sizeof...(rest);

  tVector buffer[number_of_points];
  tVector *p = buffer;
  util::ProcessVariadicValues([&p](const tVector & value)
  {
    *p++ = value;
  },
  p1, p2, rest...);

  this->DrawLineStrip(buffer, buffer + number_of_points);
}

//----------------------------------------------------------------------
// tCanvas3D DrawArrow
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::DrawArrow(T p1_x, T p1_y, T p1_z, T p2_x, T p2_y, T p2_z, bool undirected)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { p1_x, p1_y, p1_z, p2_x, p2_y, p2_z };
  this->AppendCommandRaw(eDRAW_ARROW);
  this->Stream().WriteBoolean(undirected);
  this->AppendData(values, values + 6);
}

template<typename T>
void tCanvas3D::DrawArrow(const math::tVector<3, T> &p1, const math::tVector<3, T> &p2, bool undirected)
{
  this->DrawArrow(p1.X(), p1.Y(), p1.Z(), p2.X(), p2.Y(), p2.Z(), undirected);
}

//----------------------------------------------------------------------
// tCanvas3D DrawBox
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::DrawBox(T bottom_left_x, T bottom_left_y, T bottom_left_z, T width, T height, T depth)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { bottom_left_x, bottom_left_y, bottom_left_z, width, height, depth };
  this->AppendCommand(eDRAW_BOX, values, 6);
}

template<typename T>
void tCanvas3D::DrawBox(const math::tVector<3, T> &bottom_left, T width, T height, T depth)
{
  this->DrawBox(bottom_left.X(), bottom_left.Y(), bottom_left.Z(), width, height, depth);
}

//----------------------------------------------------------------------
// tCanvas3D DrawEllipsoid
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::DrawEllipsoid(T center_x, T center_y, T center_z, T width, T height, T depth)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  T values[] = { center_x, center_y, center_z, width, height, depth };
  this->AppendCommand(eDRAW_ELLIPSOID, values, 6);
}

template<typename T>
void tCanvas3D::DrawEllipsoid(const math::tVector<3, T> &center, T width, T height, T depth)
{
  this->DrawEllipsoid(center.X(), center.Y(), center.Z(), width, height, depth);
}

//----------------------------------------------------------------------
// tCanvas3D DrawBezierCurve
//----------------------------------------------------------------------
template<typename TIterator>
void tCanvas3D::DrawBezierCurve(TIterator points_begin, TIterator points_end)
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

template<typename TElement, typename ... TVectors>
void tCanvas3D::DrawBezierCurve(const math::tVector<3, TElement> &p1, const math::tVector<3, TElement> &p2, const TVectors &... rest)
{
  typedef math::tVector<3, TElement> tVector;
  const size_t number_of_points = 2 + sizeof...(rest);

  tVector buffer[number_of_points];
  tVector *p = buffer;
  util::ProcessVariadicValues([&p](const tVector & value)
  {
    *p++ = value;
  },
  p1, p2, rest...);

  this->DrawBezierCurve(buffer, buffer + number_of_points);
}

//----------------------------------------------------------------------
// tCanvas3D DrawPolygon
//----------------------------------------------------------------------
template<typename TIterator>
void tCanvas3D::DrawPolygon(TIterator points_begin, TIterator points_end)
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

template<typename TElement, typename ... TVectors>
void tCanvas3D::DrawPolygon(const math::tVector<3, TElement> &p1, const math::tVector<3, TElement> &p2, const TVectors &... rest)
{
  typedef math::tVector<3, TElement> tVector;
  const size_t number_of_points = 2 + sizeof...(rest);

  tVector buffer[number_of_points];
  tVector *p = buffer;
  util::ProcessVariadicValues([&p](const tVector & value)
  {
    *p++ = value;
  },
  p1, p2, rest...);

  this->DrawPolygon(buffer, buffer + number_of_points);
}

//----------------------------------------------------------------------
// tCanvas3D DrawText
//--------------------------------------------------------------------
template<typename T, typename S>
void tCanvas3D::DrawText(T x, T y, T z, const S& text)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  this->AppendCommandRaw(eDRAW_STRING);
  this->Stream().WriteBoolean(false);
  T values[] = { x, y, z };
  AppendData(std::begin(values), std::end(values));
  this->Stream().WriteString(text);
}

template<typename T, typename S>
void tCanvas3D::DrawText(const math::tVector<3, T> &position, const S &text)
{
  this->DrawText(position.X(), position.Y(), position.Z(), text);
}

template<typename T, typename S>
void tCanvas3D::DrawText(T x, T y, const S& text)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  this->AppendCommandRaw(eDRAW_STRING);
  this->Stream().WriteBoolean(true);
  T values[] = { x, y };
  AppendData(std::begin(values), std::end(values));
  this->Stream().WriteString(text);
}
template<typename T, typename S>
void tCanvas3D::DrawText(const math::tVector<2, T> &position, const S &text)
{
  this->DrawText(position.X(), position.Y(), text);
}

//----------------------------------------------------------------------
// tCanvas3D DrawPointCloud
//----------------------------------------------------------------------
template<typename TIterator>
void tCanvas3D::DrawPointCloud(TIterator points_begin, TIterator points_end)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  this->AppendCommandRaw(eDRAW_POINT_CLOUD);
  this->Stream().WriteInt(std::distance(points_begin, points_end));
  this->AppendData(points_begin, points_end);
}

template<typename TElement, typename ... TVectors>
void tCanvas3D::DrawPointCloud(const math::tVector<3, TElement> &p1, const math::tVector<3, TElement> &p2, const TVectors &... rest)
{
  typedef math::tVector<3, TElement> tVector;
  const size_t number_of_points = 2 + sizeof...(rest);

  tVector buffer[number_of_points];
  tVector *p = buffer;
  util::ProcessVariadicValues([&p](const tVector & value)
  {
    *p++ = value;
  },
  p1, p2, rest...);

  this->DrawPointCloud(buffer, buffer + number_of_points);
}

//----------------------------------------------------------------------
// tCanvas3D DrawPointCloud
//----------------------------------------------------------------------
template<typename TIterator>
void tCanvas3D::DrawColoredPointCloud(TIterator points_begin, TIterator points_end)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  this->AppendCommandRaw(eDRAW_COLORED_POINT_CLOUD);
  this->Stream().WriteInt(std::distance(points_begin, points_end));
  this->AppendData(points_begin, points_end);
}

template<typename TElement, typename ... TVectors>
void tCanvas3D::DrawColoredPointCloud(const math::tVector<6, TElement> &p1, const math::tVector<6, TElement> &p2, const TVectors &... rest)
{
  typedef math::tVector<6, TElement> tVector;
  const size_t number_of_points = 2 + sizeof...(rest);

  tVector buffer[number_of_points];
  tVector *p = buffer;
  util::ProcessVariadicValues([&p](const tVector & value)
  {
    *p++ = value;
  },
  p1, p2, rest...);

  this->DrawColoredPointCloud(buffer, buffer + number_of_points);
}

//----------------------------------------------------------------------
// tCanvas3D StartPath
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::StartPath(T x, T y, T z)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  T values[] = { x, y, z };
  this->AppendCommand(ePATH_START, values, 3);
  this->Stream().WriteBoolean(false);
  this->entering_path_mode = true;
  this->in_path_mode = true;
}

template<typename T>
void tCanvas3D::StartPath(const math::tVector<3, T> &p)
{
  this->StartPath(p.X(), p.Y(), p.Z());
}

//----------------------------------------------------------------------
// tCanvas3D StartShape
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::StartShape(T x, T y, T z)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  T values[] = { x, y, z };
  this->AppendCommand(ePATH_START, values, 3);
  this->Stream().WriteBoolean(true);
  this->entering_path_mode = true;
  this->in_path_mode = true;
}

template<typename T>
void tCanvas3D::StartShape(const math::tVector<3, T> &p)
{
  this->StartShape(p.X(), p.Y(), p.Z());
}

//----------------------------------------------------------------------
// tCanvas3D AppendLineSegment
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::AppendLineSegment(T x, T y, T z)
{
  if (!this->in_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Not in path mode. Command has no effect.");
    return;
  }
  this->entering_path_mode = false;
  T values[] = { x, y, z };
  this->AppendCommand(ePATH_LINE, values, 3);
}

template<typename T>
void tCanvas3D::AppendLineSegment(const math::tVector<3, T>& v)
{
  this->AppendLineSegment(v.X(), v.Y(), v.Z());
}

//----------------------------------------------------------------------
// tCanvas3D AppendQuadraticBezierCurve
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::AppendQuadraticBezierCurve(T x1, T y1, T z1, T x2, T y2, T z2)
{
  if (!this->in_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Not in path mode. Command has no effect.");
    return;
  }
  this->entering_path_mode = false;
  T values[] = { x1, y1, z1, x2, y2, z2 };
  this->AppendCommand(ePATH_QUADRATIC_BEZIER_CURVE, values, 6);
}

template<typename T>
void tCanvas3D::AppendQuadraticBezierCurve(const math::tVector<3, T> &p1, const math::tVector<3, T> &p2)
{
  this->AppendQuadraticBezierCurve(p1.X(), p1.Y(), p1.Z(), p2.X(), p2.Y(), p2.Z());
}

//----------------------------------------------------------------------
// tCanvas3D AppendCubicBezierCurve
//----------------------------------------------------------------------
template<typename T>
void tCanvas3D::AppendCubicBezierCurve(T x1, T y1, T z1, T x2, T y2, T z2, T x3, T y3, T z3)
{
  if (!this->in_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Not in path mode. Command has no effect.");
    return;
  }
  this->entering_path_mode = false;
  T values[] = { x1, y1, z1, x2, y2, z2, x3, y3, z3 };
  this->AppendCommand(ePATH_CUBIC_BEZIER_CURVE, values, 9);
}

template<typename T>
void tCanvas3D::AppendCubicBezierCurve(const math::tVector<3, T>& p1, const math::tVector<3, T>& p2, const math::tVector<3, T>& p3)
{
  this->AppendCubicBezierCurve(p1.X(), p1.Y(), p1.Z(), p2.X(), p3.Z(), p2.Y(), p3.X(), p3.Y(), p3.Z());
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
