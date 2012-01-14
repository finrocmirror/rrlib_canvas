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
/*!\file    tCanvas2D.h
 *
 * \author  Max Reichardt
 *
 * \date    2012-01-13
 *
 * \brief Contains tCanvas2D
 *
 * \b tCanvas2D
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__canvas__tCanvas2D_h__
#define __rrlib__canvas__tCanvas2D_h__

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
//! 2D Canvas to paint to.
/*!
 *  This class represents a 2D canvas that shapes etc. can be drawn to.
 *  Currently, it is mainly intended to serialize such shapes/geometry
 *  for visualization in a remote process (such as the fingui).
 *
 *  Regarding usage, it has some similarities with typical GUI toolkit
 *  canvas classes (such as Java AWT's Graphics2D - which is used for
 *  the actual drawing in fingui).
 *
 *  Geometry is painted to tCanvas2D serially and cannot be removed.
 *  When a tCanvas2D is to be reused, Clear() can be used to remove
 *  everything.
 */
class tCanvas2D : public tCanvas
{
//----------------------------------------------------------------------
// Public methods and typedefs (no fields/variables)
//----------------------------------------------------------------------
public:

  tCanvas2D() {}

  virtual ~tCanvas2D() {}

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
