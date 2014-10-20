//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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
#include "rrlib/logging/messages.h"

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
  entering_path_mode(false),
  in_path_mode(false),
  default_viewport_offset(0),
  buffer(new rrlib::serialization::tMemoryBuffer()),
  stream(new rrlib::serialization::tOutputStream(*buffer))
{}

tCanvas::tCanvas(tCanvas && o) :
  entering_path_mode(false),
  in_path_mode(false),
  default_viewport_offset(0),
  buffer(),
  stream()
{
  std::swap(entering_path_mode, o.entering_path_mode);
  std::swap(in_path_mode, o.in_path_mode);
  std::swap(default_viewport_offset, o.default_viewport_offset);
  std::swap(buffer, o.buffer);
  std::swap(stream, o.stream);
}

//----------------------------------------------------------------------
// tCanvas2D operator=
//----------------------------------------------------------------------
tCanvas& tCanvas::operator=(tCanvas && o)
{
  std::swap(entering_path_mode, o.entering_path_mode);
  std::swap(in_path_mode, o.in_path_mode);
  std::swap(default_viewport_offset, o.default_viewport_offset);
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
  this->default_viewport_offset = 0;
}

void tCanvas::AppendCanvas(const tCanvas& canvas)
{
  if (this->entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Just started path mode. Command has no effect.");
    return;
  }
  if (canvas.entering_path_mode)
  {
    RRLIB_LOG_PRINT(ERROR, "Provided canvas just started path mode. Command has no effect.");
    return;
  }
  this->in_path_mode = false;
  this->stream->Flush();
  if (canvas.default_viewport_offset && (!this->default_viewport_offset) && (*this->buffer->GetBufferPointer(0) != static_cast<char>(tCanvasOpCode::eDEFAULT_VIEWPORT_OFFSET)))
  {
    this->default_viewport_offset = this->buffer->GetSize() + canvas.default_viewport_offset;
  }
  this->stream->Write(canvas.buffer->GetBufferPointer(0), canvas.stream->GetPosition());
}

rrlib::serialization::tOutputStream& rrlib::canvas::operator << (rrlib::serialization::tOutputStream& stream, const tCanvas& canvas)
{
  canvas.stream->Flush();
  if (!canvas.default_viewport_offset)
  {
    stream << (*canvas.buffer);
  }
  else
  {
    assert(canvas.buffer->GetSize());
    if (*canvas.buffer->GetBufferPointer(0) == static_cast<char>(tCanvasOpCode::eDEFAULT_VIEWPORT_OFFSET))
    {
      // Update default viewport offset
      canvas.buffer->GetBuffer().PutLong(1, canvas.default_viewport_offset);
    }
    else
    {
      // Prepend default viewport offset
      stream.WriteLong(9 + canvas.buffer->GetSize());
      stream << static_cast<uint8_t>(tCanvasOpCode::eDEFAULT_VIEWPORT_OFFSET);
      stream.WriteLong(canvas.default_viewport_offset);
      stream.Write(canvas.buffer->GetBuffer(), 0u, canvas.buffer->GetSize());
    }
  }
  return stream;
}

rrlib::serialization::tInputStream& rrlib::canvas::operator >> (rrlib::serialization::tInputStream& stream, tCanvas& canvas)
{
  stream >> (*canvas.buffer);
  size_t buffer_size = canvas.buffer->GetSize();
  canvas.stream->Reset();
  canvas.stream->Seek(buffer_size);

  // Restore default viewport offset member variable
  if (canvas.buffer->GetSize() && *canvas.buffer->GetBufferPointer(0) == static_cast<char>(tCanvasOpCode::eDEFAULT_VIEWPORT_OFFSET))
  {
    canvas.default_viewport_offset = canvas.buffer->GetBuffer().GetLong(1);
  }

  return stream;
}
