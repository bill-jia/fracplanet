// Source file for fracplanet
// Copyright (C) 2002 Tim Day
/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef _triangle_mesh_h_
#define _triangle_mesh_h_

#include "useful.h"
#include "random.h"

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

#include "progress.h"
#include "vertex.h"
#include "triangle.h"
#include "triangle_edge.h"
#include "geometry.h"

#include "parameters_terrain.h"
#include "parameters_save.h"

//! Contains vertices and triangles of a triangle mesh.
/*! Not as general-purpose as it might be due to constraints imposed by OpenGL.
  In particular, Triangle can have no attributes (e.g normal, colour) if a single OpenGL call is to be made to draw all triangles,
  so this information is entirely associated with Vertex.
  Two colours can be associated with each vertex (required for fracplanet application to obtain sharp coastlines)
  , and it it a requirement for subclasses to sort triangles so that all those before _triangle_switch_colour use vertex colour index 0,
  , and those afterwards vertex colour index 0.
 */
class TriangleMesh
{
protected:
  std::vector<Vertex> _vertex;
  std::vector<Triangle> _triangle; 
  uint _triangle_switch_colour;
  
  Vertex& vertex(uint i)
    {
      return _vertex[i];
    }

  Triangle& triangle(uint i)
    {
      return _triangle[i];
    }

  virtual const Geometry& geometry() const
    =0;

  Progress*const _progress;
  
  void progress_start(uint steps,const std::string& info) const;
  void progress_step(uint step) const;
  void progress_complete(const std::string& info) const;

public:
  TriangleMesh(Progress* progress)
    :_progress(progress)
    {}

  TriangleMesh(const TriangleMesh& mesh)
    :_vertex(mesh._vertex)
    ,_triangle(mesh._triangle)
    ,_triangle_switch_colour(0)
    ,_progress(mesh._progress)
    {}
  
  virtual ~TriangleMesh()
    {}

  void add_vertex(const Vertex& v)
    {
      _vertex.push_back(v);
    }

  void add_triangle(const Triangle& t)
    {
      _triangle.push_back(t);
    }

  const Vertex& vertex(uint i) const
    {
      return _vertex[i];
    }

  const Triangle& triangle(uint i) const
    {
      return _triangle[i];
    }

  const float vertex_height(uint i) const
    {
      return geometry().height(vertex(i).position());
    }
  const void set_vertex_height(uint i,float h)
    {
      XYZ p(vertex(i).position());
      geometry().set_height(p,h);   
      vertex(i).position(p);
    }

  const float triangle_height_min(uint i) const
    {
      const Triangle& t=triangle(i);
      return minimum
	(
	 vertex_height(t.vertex(0)),
	 vertex_height(t.vertex(1)),
	 vertex_height(t.vertex(2))
	 );
    }
  const float triangle_height_max(uint i) const
    {
      const Triangle& t=triangle(i);
      return maximum
	(
	 vertex_height(t.vertex(0)),
	 vertex_height(t.vertex(1)),
	 vertex_height(t.vertex(2))
	 );
    }
  const float triangle_height_average(uint i) const
    {
      const Triangle& t=triangle(i);
      return 
	(
	 vertex_height(t.vertex(0))
	 +vertex_height(t.vertex(1))
	 +vertex_height(t.vertex(2))
	 )/3.0;
    }

  //! Compute and return the normal to a triangle
  const XYZ triangle_normal(uint i) const;

  const uint which_colour_for_triangle(uint t) const
    {
      return (t<_triangle_switch_colour ? 0 : 1);
    }

  //! Returns number of vertices in mesh.
  const uint vertices() const
    {
      return _vertex.size();
    }
  //! Returns number of triangles in mesh.
  const uint triangles() const
    {
      return _triangle.size();
    }

  //! Returns number of triangles in mesh indexing colour-zero of vertices.
  const uint triangles_of_colour0() const
    {
      return _triangle_switch_colour;
    }

  //! Returns number of triangles in mesh indexing colour-one of vertices.
  const uint triangles_of_colour1() const
    {
      return triangles()-_triangle_switch_colour;
    }

  //! (Re-)computes vertex normals.
  void compute_vertex_normals();

  //! Perform a single subdivision pass with perturbations up to the specified size
  void subdivide(const XYZ& variation);
  
  //! Perform a number of subdivisions, possibly some unperturbed ("flat"), and halving the perturbation variation each iteration.
  void subdivide(uint subdivisions,uint flat_subdivisions,const XYZ& variation);

  //! Dump the mesh to files in a form suitable for use by POVRay.
  void write_povray(const std::string& fname_base,const std::string& header,bool exclude_alternate_colour) const;
};

//! A single triangle lying in the z-plane.
class TriangleMeshFlatTriangle : virtual public TriangleMesh
{
 protected:
  GeometryFlat _geometry;
 public:
  TriangleMeshFlatTriangle(float z,uint seed,Progress* progress);
  virtual ~TriangleMeshFlatTriangle()
    {}  

  virtual const Geometry& geometry() const
    {
      return _geometry;
    }
};

//! An icosahedron.
class TriangleMeshIcosahedron : virtual public TriangleMesh
{
 protected:
  GeometrySpherical _geometry;
 public:
  TriangleMeshIcosahedron(float radius,uint seed,Progress* progress);
  virtual ~TriangleMeshIcosahedron()
    {}

  virtual const Geometry& geometry() const
    {
      return _geometry;
    }
};

//! A subdivided icosahedron.
class TriangleMeshSubdividedIcosahedron : public TriangleMeshIcosahedron
{
 public:
  TriangleMeshSubdividedIcosahedron(float radius,uint subdivisions,uint flat_subdivisions,uint seed,const XYZ& variation,Progress* progress);
  virtual ~TriangleMeshSubdividedIcosahedron()
    {}
};

#endif