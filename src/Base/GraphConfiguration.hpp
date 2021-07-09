// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/

#ifndef WAC_GraphConfiguration
#define WAC_GraphConfiguration

/////////////////////////////////////////////////////////////////////////
//                                                                     //
// GraphConfiguration                                                  //
//                                                                     //
// Class used to describe a graph configuration                        //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TString.h"
#include "TAttMarker.h"
#include "TAttLine.h"
#include <vector>

using namespace std;
using std::vector;

//!
//! Class encapsulates the plotting properties or attributes of a graph or histograms.
//! This class is designed to work with other WAC graphics classes to set the plotting
//! attributes of graphs (TGraph), histograms (TH1*), and data graphs (DataGraph) displaying
//! data values, statistical error bars, and systematic error bars.
//!
//! Lines are used to draw connections between points as well as  (statistical) error bars.
//! Markers are used to represent the data point position/value
//! Systs are used to represent the systematic error bars or bounds.
//!
//! All data members of this class are public to enable getting/setting operations.
//!
class GraphConfiguration
{
public:

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  int npx;
  int npy;
  
  int lineColor;
  int lineStyle;
  int lineWidth;
  
  int markerColor;
  int markerStyle;
  float markerSize;

  int systsColor;
  int systsStyle;
  float systsWidth;


  int   nXDivisions;
  float xTitleSize;
  float xTitleOffset;
  float xLabelSize;
  float xLabelOffset;
  TString xTitle;
  
  int   nYDivisions;
  float yTitleSize;
  float yTitleOffset;
  float yLabelSize;
  float yLabelOffset;
  TString yTitle;
  
  int   nZDivisions;
  float zTitleSize;
  float zTitleOffset;
  float zLabelSize;
  float zLabelOffset;
  TString zTitle;
  
  TString plotOption;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Member functrions
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  GraphConfiguration(int d=1, int type=1);
  GraphConfiguration(const GraphConfiguration & source);
  virtual ~GraphConfiguration(){}
  GraphConfiguration & operator=(const GraphConfiguration & other);

  static vector<GraphConfiguration*> createConfigurationPalette(unsigned int n, int dimension);

  ClassDef(GraphConfiguration,0)
};

#endif
