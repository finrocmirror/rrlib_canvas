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
 * \author  Tobias Föhst
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
 * [vector] is 2 coordinates in 2D and 3 coordinates in 3D mode.
 * K is 2 in 2D and 3 in 3D mode.
 * Values are encoded as float or double depending on canvas mode.
 */
enum tCanvasOpCode
{
  // ####### tCanvas-supported opcodes ########

  // Transformation operations
  eSET_TRANSFORMATION,            // [(K+1)x(K+1) matrix]
  eTRANSFORM,                     // [(K+1)x(K+1) matrix]
  eTRANSLATE,                     // [vector]
  eROTATE,                        // [yaw]
  eSCALE,                         // [vector]
  eRESET_TRANSFORMATION,          // []

  // Canvas, Draw & encoding mode
  eSET_COLOR,                     // [RGB: 3 bytes]
  eSET_EDGE_COLOR,                // [RGB: 3 bytes]
  eSET_FILL_COLOR,                // [RGB: 3 bytes]
  eSET_FILL,                      // [bool]
  eSET_ALPHA,                     // [1 byte]

  // Geometry primitives
  eDRAW_POINT,                    // [vector]
  eDRAW_LINE,                     // [vector][vector]
  eDRAW_LINE_SEGMENT,             // [vector][vector]
  eDRAW_ARROW,                    // [bool][vector][vector]
  eDRAW_BOX,                      // [vector][size1]...[sizeN]
  eDRAW_ELLIPSOID,                // [vector][diameter1]...[diameterN]
  eDRAW_BEZIER_CURVE,             // [degree: N][vector1]...[vectorN+1]
  eDRAW_POLYGON,                  // [number of values: N][vector1]...[vectorN]
  eDRAW_SPLINE,                   // [number of values: N][tension-parameter][vector1]...[vectorN]
  eDRAW_STRING,                   // [vector][null-terminated chars]

  // Custom path/shape
  ePATH_START,                    // [point]
  ePATH_END_OPEN,                 // [point]
  ePATH_END_CLOSED,               // [point]
  ePATH_LINE,                     // [point]
  ePATH_QUADRATIC_BEZIER_CURVE,   // [point][point]
  ePATH_CUBIC_BEZIER_CURVE,       // [point][point][point]

  // ####### tCanvas2D-only opcodes ########

  // Canvas, Draw & encoding mode
  eSET_Z,                         // [value]
  eSET_EXTRUSION,                 // [value]

  // ####### tCanvas3D-only opcodes ########
  eDRAW_COLORED_POINT_CLOUD,      // [number of values: N][6d vector1]...[6d vectorN]
  eDRAW_POINT_CLOUD,              // [number of values: N][vector1]...[vectorN]
  eDRAW_LINE_STRIP                // [number of values: N][vector1]...[vectorN]
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
