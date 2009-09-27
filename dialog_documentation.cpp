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
  \brief Implementation of class DialogDocumentation.
*/

#include "precompiled.h"

#include "dialog_documentation.h"

static const char*const text=
#include "usage_text.h"
;

DialogDocumentation::DialogDocumentation(QWidget* parent)
  :QDialog(parent)
{
  setWindowTitle("Fracplanet User Manual");
  setMinimumSize(480,320);
  setSizeGripEnabled(true);

  setLayout(new QVBoxLayout());
  
  QTextBrowser*const browser=new QTextBrowser();
  layout()->addWidget(browser);
  browser->setText(text);

  QPushButton*const ok=new QPushButton("OK");
  layout()->addWidget(ok);

  //! \todo: These button settings don't seem to do anything.  Find out what's up.
  ok->setAutoDefault(true);
  ok->setDefault(true);

  connect(
	  ok,SIGNAL(clicked()),
	  this,SLOT(hide())
	  );
}

DialogDocumentation::~DialogDocumentation()
{}
