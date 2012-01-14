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
/*!\file    tCanvas.cpp
 *
 * \author  Max Reichardt
 *
 * \date    2012-01-09
 *
 */
//----------------------------------------------------------------------
#include "rrlib/canvas/tCanvas.h"

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
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::canvas;

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------
tCanvas::tCanvas() :
    buffer(),
    stream(&buffer)
{
}


void tCanvas::AppendCommandRaw(tCanvasOpCode opcode, void* buffer, size_t bytes)
{
  stream << opcode;
  if (buffer)
  {
    stream.Write(buffer, bytes);
  }
}

void tCanvas::Clear()
{
  buffer.Clear();
  stream.Reset(&buffer);
}

void tCanvas::Deserialize(rrlib::serialization::tInputStream& is)
{
  is >> buffer;
}

void tCanvas::Serialize(rrlib::serialization::tOutputStream& os) const
{
  os.WriteInt(stream.GetWriteSize());
  os.Write(*buffer.GetBuffer(), 0u, stream.GetWriteSize());
}


