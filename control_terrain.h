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
#ifndef _control_terrain_h_
#define _control_terrain_h_

#include <qhbox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qgrid.h>
#include <qtooltip.h>
#include <qradiobutton.h>
#include <qhbuttongroup.h>
#include <qslider.h>
#include <qcolordialog.h>
#include <qiconset.h>

#include "useful.h"
#include "parameters_terrain.h"

#include <iostream>

class FracplanetMain;

//! Encapsulates controls for setting terrain generation parameters
/*! \todo: Way too much stuff in this classes .h file. Shift it to the .cpp!
 */
class ControlTerrain : public QVBox
{
 private:
  Q_OBJECT;
 protected:

  //! Pointer to the parameters we control.
  ParametersTerrain*const parameters;

  //! Requests to regenerate terrain are forwarded to the top level GUI widget.
  FracplanetMain* regenerate_target;

  QGrid* grid; 

  QHButtonGroup* object_type_button_group;
  QRadioButton* object_type_planet_button;
  QRadioButton* object_type_terrain_button;
  
  QLabel* subdivisions_seed_label;
  QSpinBox* subdivisions_seed_spinbox;
  QLabel* subdivisions_label;
  QSpinBox* subdivisions_spinbox;
  QLabel* subdivisions_unperturbed_label;
  QSpinBox* subdivisions_unperturbed_spinbox;

  QLabel* variation_vertical_label;
  QSpinBox* variation_vertical_spinbox;
  QLabel* variation_horizontal_label;
  QSpinBox* variation_horizontal_spinbox;

  QLabel* base_height_label;
  QSpinBox* base_height_spinbox;

  QLabel* power_law_label;
  QSpinBox* power_law_spinbox;

  QLabel* snowline_equator_label;
  QSpinBox* snowline_equator_spinbox;
  QLabel* snowline_pole_label;
  QSpinBox* snowline_pole_spinbox;
  QLabel* snowline_power_law_label;
  QSpinBox* snowline_power_law_spinbox;
  QLabel* snowline_slope_effect_label;
  QSpinBox* snowline_slope_effect_spinbox;
  QLabel* snowline_glacier_effect_label;
  QSpinBox* snowline_glacier_effect_spinbox;
  
  QLabel* rivers_label;
  QSpinBox* rivers_spinbox;
  QLabel* rivers_seed_label;
  QSpinBox* rivers_seed_spinbox;
  QLabel* lake_becomes_sea_label;
  QSpinBox* lake_becomes_sea_spinbox;

  QLabel* colour_label;
  QGrid* colour_grid;
  QPushButton* colour_ocean_button;
  QPushButton* colour_shoreline_button;
  QPushButton* colour_low_button;
  QPushButton* colour_river_button;
  QPushButton* colour_snow_button;
  QPushButton* colour_high_button;

  QPushButton* regenerate_button;
  QPushButton* regenerate_with_new_seed_button;
  QPushButton* regenerate_rivers_with_new_seed_button;

  QVBox* padding;

  //! Utility function to build a small Qt icon of the specified colour.
  static QIconSet build_icon_of_colour(const FloatRGB& col);

 public:
  ControlTerrain(QWidget* parent,FracplanetMain* tgt,ParametersTerrain* param);
  virtual ~ControlTerrain()
    {}

  public slots:

  void setObjectType(int id)
    {
      switch (id)
	{
	case 0: parameters->object_type=ParametersTerrain::ObjectTypePlanet;break;
	case 1: parameters->object_type=ParametersTerrain::ObjectTypeTerrain;break;
	}
    }
  void setSubdivisionsSeed(int v)
    {
      parameters->subdivisions_seed=v;
    }
  void setSubdivisions(int v)
    {
      parameters->subdivisions=v;
    }
  void setSubdivisionsUnperturbed(int v)
    {
      parameters->subdivisions_unperturbed=v;
    }
  void setVariationVertical(int v)
    {
      parameters->variation.z=v/1024.0;
    }
  void setVariationHorizontal(int v)
    {
      parameters->variation.x=v/1024.0;
      parameters->variation.y=v/1024.0;
    }
  void setBaseHeight(int v)
    {
      parameters->base_height=v/100.0;
    }
  void setPowerLaw(int v)
    {
      parameters->power_law=v/100.0;
    }
  void setSnowlineEquator(int v)
    {
      parameters->snowline_equator=v/100.0;
    }
  void setSnowlinePole(int v)
    {
      parameters->snowline_pole=v/100.0;
    }
  void setSnowlinePowerLaw(int v)
    {
      parameters->snowline_power_law=v/100.0;
    }
  void setSnowlineSlopeEffect(int v)
    {
      parameters->snowline_slope_effect=v/100.0;
    }
  void setSnowlineGlacierEffect(int v)
    {
      parameters->snowline_glacier_effect=v/100.0;
    }
  void setRivers(int v)
    {
      parameters->rivers=v;
    }
  void setRiversSeed(int v)
    {
      parameters->rivers_seed=v;
    }
  void setLakeBecomesSea(int v)
    {
      parameters->lake_becomes_sea=v/100.0;
    }
  void pickColourOcean()
    {
      emit pickColour(colour_ocean_button,parameters->colour_ocean);
    }
  void pickColourShoreline()
    {
      emit pickColour(colour_shoreline_button,parameters->colour_shoreline);
    }
  void pickColourLow()
    {
      emit pickColour(colour_low_button,parameters->colour_low);
    }
  void pickColourRiver()
    {
      emit pickColour(colour_river_button,parameters->colour_river);
    }
  void pickColourSnow()
    {
      emit pickColour(colour_snow_button,parameters->colour_snow);
    }
  void pickColourHigh()
    {
      emit pickColour(colour_high_button,parameters->colour_high);
    }

  //! Use Qt's colour-picking dialog to replace the referenced colour
  void pickColour(QPushButton* button,FloatRGB& colour)
    {
      const ByteRGB col_old(colour);
      QColor qcol_old(col_old.r,col_old.g,col_old.b);
      QColor qcol_new=QColorDialog::getColor(qcol_old,this);
      if (qcol_new.isValid())
	{
	  colour=FloatRGB(ByteRGB(qcol_new.red(),qcol_new.green(),qcol_new.blue()));
	  
	  QPixmap pmap(16,16);
	  pmap.fill(qcol_new);
	  button->setIconSet(QIconSet(pmap));
	}
    }

  void regenerate_rivers_with_new_seed();
  void regenerate_with_new_seed();
};



#endif