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
/*!\file    definitions.h
 *
 * \author  Max Reichardt
 *
 * \date    2012-01-10
 *
 * \brief Contains op codes for tCanvas
 *
 * \b tCanvas
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__canvas__definitions_h__
#define __rrlib__canvas__definitions_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

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

/*!
 * OpCodes when serializing canvas.
 *
 * [vector] and [point] are 2 coordinates in 2D and 3 coordinates in 3D mode.
 * Values are encoded as float or double depending on canvas mode.
 */
enum tCanvasOpCode
{
  // ####### tCanvas-supported opcodes ########

  // Transformation operations
  eSET_TRANSFORM_2D,  // [m00, m10, m01, m11, m02, m12]
  eTRANSFORM_2D,      // [m00, m10, m01, m11, m02, m12]
  eTRANSLATE_2D,      // [2D-vector]
  eTRANSLATE_3D,      // [3D-vector]
  eROTATE_2D,         // [yaw]
  eSCALE_2D,          // [2D-vector]
  eRESET_TRANSFORM,   // []

  // Canvas, Draw & encoding mode
  eSET_COLOR,         // [RGB: 3 bytes]
  eSET_EDGE_COLOR,    // [RGB: 3 bytes]
  eSET_FILL_COLOR,    // [RGB: 3 bytes]
  eSET_FILL,          // [bool]
  eSET_Z,             // [value]

  // Geometry primitives
  eDRAW_POINT_2D,              // [2D-vector]
  eDRAW_LINE_2D,               // [2D-point][2D-point]
  eDRAW_RECTANGLE,             // [2D-point][width][height]
  eDRAW_ELLIPSE,               // [2D-point][width][height]
  eDRAW_POLYGON_2D,            // [number of values][2D-vector1]...[2D-vectorN]
  eDRAW_SPLINE_2D,             // [number of values][2D-vector1]...[2D-vectorN]  (bezier spline)
  eDRAW_STRING_2D,             // [2D-point][null-terminated chars]
  eDRAW_CUBIC_BEZIER_CURVE_2D, // [2D-point][2D-point][2D-point][2D-point]

  // Custom path/shape
  ePATH_2D_START,       // [2D-point][bool shape?]
  ePATH_2D_LINE,        // [2D-point]
  ePATH_2D_QUAD_CURVE,  // [2D-point][2D-point]
  ePATH_2D_CUBIC_CURVE, // [2D-point][2D-point][2D-point]

  // ####### tCanvas2D-only opcodes ########


  // ####### tCanvas3D-only opcodes ########
  eSET_TRANSFORM_3D, // [m00, m10, m20, m01, m11, m21, m02, m12, m22, m03, m13, m23]
  eTRANSFORM_3D,     // [m00, m10, m20, m01, m11, m21, m02, m12, m22, m03, m13, m23]
  eROTATE_3D,        // [roll, pitch, yaw]
  eSCALE_3D,         // [3D-vector]

  eSET_EXTRUSION,    // {value]
  eDRAW_POINT_3D,    // [3D-vector]
  eDRAW_LINE_3D,     // [3D-point][3D-point]
  eDRAW_POLYGON_3D   // [number of values][3D-vector1]...[3D-vectorN]
};

enum tNumberTypeEnum
{
  eFLOAT,
  eDOUBLE,
  eZEROES,
  eINT8,
  eUINT8,
  eINT16,
  eUINT16,
  eINT32,
  eUINT32,
  eINT64,
  eUINT64
};

template <typename T>
struct tNumberType {};

template <> struct tNumberType<float>
{
  const static tNumberTypeEnum value = eFLOAT;
};
template <> struct tNumberType<double>
{
  const static tNumberTypeEnum value = eDOUBLE;
};
template <> struct tNumberType<int8_t>
{
  const static tNumberTypeEnum value = eINT8;
};
template <> struct tNumberType<uint8_t>
{
  const static tNumberTypeEnum value = eUINT8;
};
template <> struct tNumberType<int16_t>
{
  const static tNumberTypeEnum value = eINT16;
};
template <> struct tNumberType<uint16_t>
{
  const static tNumberTypeEnum value = eUINT16;
};
template <> struct tNumberType<int32_t>
{
  const static tNumberTypeEnum value = eINT32;
};
template <> struct tNumberType<uint32_t>
{
  const static tNumberTypeEnum value = eUINT32;
};
template <> struct tNumberType<int64_t>
{
  const static tNumberTypeEnum value = eINT64;
};
template <> struct tNumberType<uint64_t>
{
  const static tNumberTypeEnum value = eUINT64;
};

}
}


#endif
