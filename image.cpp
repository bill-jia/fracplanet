// Source file for fracplanet
// Copyright (C) 2006 Tim Day
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

/*! \file
  \brief Implementation for templated Image class.
*/

#include "precompiled.h"

#include "image.h"

#include "progress.h"
#include "rgb.h"

template <typename T> void Raster<T>::fill(const T& v)
{
  if (contiguous())
    {
      std::fill(contiguous_begin(),contiguous_end(),v);
    }
  else 
    {
      for (RowIterator row=row_begin();row!=row_end();++row)
	std::fill((*row).begin(),(*row).end(),v);
    }
}

template <typename T> const typename Raster<T>::ScalarType Raster<T>::maximum_scalar_pixel_value() const
{
  ScalarType m(scalar(*_data));
  for (ConstRowIterator row=row_begin();row!=row_end();++row)
    for (const T* it=row->begin();it!=row->end();++it)
      {
	const ScalarType v(scalar(*it));
	if (v>m) m=v;
      }
  return m;
}

template <> bool Raster<uchar>::write_pgmfile(const std::string& filename,Progress* target) const
{
  ProgressScope progress(height(),"Writing PGM image:\n"+filename,target);
  std::ofstream out(filename.c_str(),std::ios::binary);
  out << "P5" << std::endl;
  out << width() << " " << height() << std::endl;
  out << "255" << std::endl;
  for (ConstRowIterator row=row_begin();row!=row_end();++row)
    {
      progress.step();
      out.write(reinterpret_cast<const char*>(&(*(row->begin()))),row->size());
    }
  out.close();
  return out;
}

template <> bool Raster<ushort>::write_pgmfile(const std::string& filename,Progress* target) const
{
  ProgressScope progress(height(),"Writing PGM image:\n"+filename,target);
  std::ofstream out(filename.c_str(),std::ios::binary);
  out << "P5" << std::endl;
  out << width() << " " << height() << std::endl;
  const ushort m=maximum_scalar_pixel_value();
  out << m << std::endl;
  for (ConstRowIterator row=row_begin();row!=row_end();++row)
    {
      progress.step();
      for (const ushort* it=row->begin();it!=row->end();++it)
	{
	  const uchar p[2]={((*it)>>8),(*it)};
	  if (m>=256)
	    {
	      // PGM spec is most significant byte first
	      out.write(reinterpret_cast<const char*>(p),2);
	    }
	  else
	    {
	      assert(p[0]==0);
	      out.write(reinterpret_cast<const char*>(p+1),1);
	    }
	}
    }
  out.close();
  return out;
}

template <> bool Raster<ByteRGBA>::write_ppmfile(const std::string& filename,Progress* target) const
{
  ProgressScope progress(height(),"Writing PPM image:\n"+filename,target);
  std::ofstream out(filename.c_str(),std::ios::binary);
  out << "P6" << std::endl;
  out << width() << " " << height() << std::endl;
  out << "255" << std::endl;
  for (ConstRowIterator row=row_begin();row!=row_end();++row)
    {
      progress.step();
      for (const ByteRGBA* it=row->begin();it!=row->end();++it)
	out.write(reinterpret_cast<const char*>(&((*it).r)),3);
    }
  out.close();
  return out;
}


template class Raster<uchar>;
template class Image<uchar>;

template class Raster<ushort>;
template class Image<ushort>;

template class Raster<ByteRGBA>;
template class Image<ByteRGBA>;

