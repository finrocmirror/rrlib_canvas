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
 * \author  Tobias Föhst
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

//----------------------------------------------------------------------
// tCanvas constructors
//----------------------------------------------------------------------
tCanvas::tCanvas() :
  buffer(new rrlib::serialization::tMemoryBuffer()),
  stream(new rrlib::serialization::tOutputStream(*buffer))
{}

tCanvas::tCanvas(tCanvas && o) :
  buffer(),
  stream()
{
  std::swap(buffer, o.buffer);
  std::swap(stream, o.stream);
}

//----------------------------------------------------------------------
// tCanvas2D operator=
//----------------------------------------------------------------------
tCanvas& tCanvas::operator=(tCanvas && o)
{
  std::swap(buffer, o.buffer);
  std::swap(stream, o.stream);
  return *this;
}

//----------------------------------------------------------------------
// tCanvas AppendCommandRaw
//----------------------------------------------------------------------
void tCanvas::AppendCommandRaw(tCanvasOpCode opcode, void* buffer, size_t bytes)
{
  (*this->stream) << opcode;
  if (buffer)
  {
    this->stream->Write(buffer, bytes);
  }
}

//----------------------------------------------------------------------
// tCanvas Clear
//----------------------------------------------------------------------
void tCanvas::Clear()
{
  this->buffer->Clear();
  this->stream->Reset(*this->buffer);
}

rrlib::serialization::tOutputStream& rrlib::canvas::operator << (rrlib::serialization::tOutputStream& stream, const tCanvas& canvas)
{
  stream.WriteLong(canvas.stream->GetWriteSize());
  stream.Write(canvas.buffer->GetBuffer(), 0u, canvas.stream->GetWriteSize());
  return stream;
}

rrlib::serialization::tInputStream& rrlib::canvas::operator >> (rrlib::serialization::tInputStream& stream, tCanvas& canvas)
{
  stream >> (*canvas.buffer);
  return stream;
}



