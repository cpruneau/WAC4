// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class GenericEventAnalyzer
 \ingroup WAC

 Utility class used to define graph parameters
 */

#include "GraphConfiguration.hpp"

ClassImp(GraphConfiguration);


GraphConfiguration::GraphConfiguration(int dim, int type)
:
npx(100),
npy(100),
lineColor(1),
lineStyle(1),
lineWidth(2),
markerColor(1),
markerStyle(20),
markerSize(1.1),
systsColor(1),
systsStyle(3040),
systsWidth(1),
nXDivisions(5),
xTitleSize(0.06),
xTitleOffset(1.1),
xLabelSize(0.05),
xLabelOffset(0.01),
xTitle("x"),
nYDivisions(5),
yTitleSize(0.06),
yTitleOffset(1.2),
yLabelSize(0.05),
yLabelOffset(0.01),
yTitle("y"),
nZDivisions(5),
zTitleSize(0.06),
zTitleOffset(1.5),
zLabelSize(0.05),
zLabelOffset(0.01),
zTitle("z"),
plotOption("E0")
{
  if (dim==1 || dim==0)
    {
    lineWidth  = 2;
    markerSize = 1.25;
    systsStyle = 3003;
    systsWidth = 1;
    switch (type)
      {
        default:

        case  0: lineColor = kBlack;  lineStyle = kSolid; markerColor = kBlack;   markerStyle = kFullSquare;    systsColor = kBlack;break;
        case  1: lineColor = kRed;    lineStyle = kSolid; markerColor = kRed;     markerStyle = kFullCircle;    systsColor = kRed;break;
        case  2: lineColor = kGreen;  lineStyle = kSolid; markerColor = kGreen;   markerStyle = kFullSquare;    systsColor = kGreen;break;
        case  3: lineColor = kMagenta;lineStyle = kSolid; markerColor = kMagenta; markerStyle = kFullDiamond;   systsColor = kMagenta;break;
        case  4: lineColor = kBlue;   lineStyle = kSolid; markerColor = kBlue;    markerStyle = kOpenSquare;    systsColor = kBlue;break;
        case  5: lineColor = kYellow; lineStyle = kSolid; markerColor = kYellow;  markerStyle = kOpenCircle;    systsColor = kYellow;break;
        case  6: lineColor = kPink;   lineStyle = kSolid; markerColor = kPink ;   markerStyle = kOpenSquare;    systsColor = kPink;break;
        case  7: lineColor = kBlue+2; lineStyle = kSolid; markerColor = kBlue+2;  markerStyle = kOpenDiamond;   systsColor = kBlue+2;break;

        case  8: lineColor = kBlack;  lineStyle = kDashed; markerColor = kBlack;   markerStyle = kFullSquare;    systsColor = kBlack;break;
        case  9: lineColor = kRed;    lineStyle = kDashed; markerColor = kRed;     markerStyle = kFullCircle;    systsColor = kRed;break;
        case  10: lineColor = kGreen;  lineStyle = kDashed; markerColor = kGreen;   markerStyle = kFullSquare;    systsColor = kGreen;break;
        case  11: lineColor = kMagenta;lineStyle = kDashed; markerColor = kMagenta; markerStyle = kFullDiamond;   systsColor = kMagenta;break;
        case  12: lineColor = kBlue;   lineStyle = kDashed; markerColor = kBlue;    markerStyle = kOpenSquare;    systsColor = kBlue;break;
        case  13: lineColor = kYellow; lineStyle = kDashed; markerColor = kYellow;  markerStyle = kOpenCircle;    systsColor = kYellow;break;
        case  14: lineColor = kPink;   lineStyle = kDashed; markerColor = kPink ;   markerStyle = kOpenSquare;    systsColor = kPink;break;
        case  15: lineColor = kBlue+2; lineStyle = kDashed; markerColor = kBlue+2;  markerStyle = kOpenDiamond;   systsColor = kBlue+2;break;

        case  20:  lineColor = kBlack;  lineStyle = 1;  lineWidth = 4;  markerColor = kBlack;  markerStyle = 20; markerSize  = 1.5; systsColor = kBlack; break;
      }
    }
  else if (dim==2)
    {
    nYDivisions  = 4;
    xTitleSize   = 0.05;
    xTitleOffset = 1.3;
    xLabelSize   = 0.05;
    xLabelOffset = 0.002;
    nYDivisions  = 4;
    yTitleSize   = 0.05;
    yTitleOffset = 1.5;
    yLabelSize   = 0.05;
    yLabelOffset = 0.002;
    nZDivisions  = 4;
    zTitleSize   = 0.05;
    zTitleOffset = 1.7;
    zLabelSize   = 0.05;
    zLabelOffset = 0.005;
    plotOption   = "SURF3";
    }
}

GraphConfiguration::GraphConfiguration(const GraphConfiguration & source)
:
npx(source.npx),
npy(source.npy),
lineColor(source.lineColor),
lineStyle(source.lineStyle),
lineWidth(source.lineWidth),
markerColor(source.markerColor),
markerStyle(source.markerStyle),
markerSize(source.markerSize),
systsColor(source.systsColor),
systsStyle(source.systsStyle),
systsWidth(source.systsWidth),
nXDivisions(source.nXDivisions),
xTitleSize(source.xTitleSize),
xTitleOffset(source.xTitleOffset),
xLabelSize(source.xLabelSize),
xLabelOffset(source.xLabelOffset),
xTitle(source.xTitle),
nYDivisions(source.nYDivisions),
yTitleSize(source.yTitleSize),
yTitleOffset(source.yTitleOffset),
yLabelSize(source.yLabelSize),
yLabelOffset(source.yLabelOffset),
yTitle(source.yTitle),
nZDivisions(source.nZDivisions),
zTitleSize(source.zTitleSize),
zTitleOffset(source.zTitleOffset),
zLabelSize(source.zLabelSize),
zLabelOffset(source.zLabelOffset),
zTitle(source.zTitle),
plotOption(source.plotOption)
{

}

GraphConfiguration & GraphConfiguration::operator=(const GraphConfiguration & source)
{
  if (this!=&source)
    {
    npx = source.npx;
    npy = source.npy;
    lineColor = source.lineColor;
    lineStyle = source.lineStyle;
    lineWidth = source.lineWidth;
    markerColor = source.markerColor;
    markerStyle = source.markerStyle;
    markerSize = source.markerSize;
    systsColor = source.systsColor;
    systsStyle = source.systsStyle;
    systsWidth = source.systsWidth;
    nXDivisions = source.nXDivisions;
    xTitleSize = source.xTitleSize;
    xTitleOffset = source.xTitleOffset;
    xLabelSize = source.xLabelSize;
    xLabelOffset = source.xLabelOffset;
    xTitle = source.xTitle;
    nYDivisions = source.nYDivisions;
    yTitleSize = source.yTitleSize;
    yTitleOffset = source.yTitleOffset;
    yLabelSize = source.yLabelSize;
    yLabelOffset = source.yLabelOffset;
    yTitle = source.yTitle;
    nZDivisions = source.nZDivisions;
    zTitleSize = source.zTitleSize;
    zTitleOffset = source.zTitleOffset;
    zLabelSize = source.zLabelSize;
    zLabelOffset = source.zLabelOffset;
    zTitle = source.zTitle;
    plotOption = source.plotOption;
    }
  return *this;
}



vector<GraphConfiguration*> GraphConfiguration::createConfigurationPalette(unsigned int n, int dimension)
{
  vector<GraphConfiguration*> configs;

  for (unsigned int k=0; k<n; k++)
    {
    GraphConfiguration * c = new GraphConfiguration(1, k);
    configs.push_back( c );
    }
  return configs;
}
