/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoGetPrimitiveCountAction SoGetPrimitiveCountAction.h Inventor/actions/SoGetPrimitiveCountAction.h
  \brief The SoGetPrimitiveCountAction class counts the primitives in a scene.
  \ingroup actions

  Apply this action to a scene if you need to know the number of
  primitives present.
*/

#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/SbName.h>
#include <Inventor/actions/SoSubActionP.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/nodes/SoNode.h>


SO_ACTION_SOURCE(SoGetPrimitiveCountAction);


// Override from parent class.
void
SoGetPrimitiveCountAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoGetPrimitiveCountAction, SoAction);
}


/*!
  Constructor.
*/
SoGetPrimitiveCountAction::SoGetPrimitiveCountAction(void)
{
  SO_ACTION_CONSTRUCTOR(SoGetPrimitiveCountAction);

  SO_ACTION_ADD_METHOD_INTERNAL(SoNode, SoNode::getPrimitiveCountS);

  SoGetPrimitiveCountAction::methods->setUp();

  this->textastris = TRUE;
  this->approx = FALSE;
  this->nonvertexastris = TRUE;
}

/*!
  The destructor.
*/
SoGetPrimitiveCountAction::~SoGetPrimitiveCountAction()
{
}

/*!
  Returns number of triangles in graph.
*/
int
SoGetPrimitiveCountAction::getTriangleCount(void) const
{
  return this->numtris;
}

/*!
  Returns number of lines in graph.
*/
int
SoGetPrimitiveCountAction::getLineCount(void) const
{
  return this->numlines;
}

/*!
  Returns number of points in graph.
*/
int
SoGetPrimitiveCountAction::getPointCount(void) const
{
  return this->numpoints;
}

/*!
  Returns number of texts in the graph.
*/
int
SoGetPrimitiveCountAction::getTextCount(void) const
{
  return this->numtexts;
}

/*!
  Returns the number of images in the graph.
*/
int
SoGetPrimitiveCountAction::getImageCount(void) const
{
  return this->numimages;
}

/*!
  Returns whether there are any primitives in graph or not.
*/
SbBool
SoGetPrimitiveCountAction::containsNoPrimitives(void)
{
  return
    this->numtris == 0 &&
    this->numlines == 0 &&
    this->numpoints == 0 &&
    this->numtexts == 0 &&
    this->numimages == 0;
}

/*!
  Returns whether there are non-triangular primitives in graph.
*/
SbBool
SoGetPrimitiveCountAction::containsNonTriangleShapes(void)
{
  return
    this->numlines != 0 ||
    this->numpoints != 0 ||
    this->numtexts != 0 ||
    this->numimages != 0;
}

/*!
  Sets whether SoText3 nodes are counted as the triangles of the
  fonts in the text strings or the text itself. The default is to
  count as triangles.

  \sa is3DTextCountedAsTriangles()
*/
void
SoGetPrimitiveCountAction::setCount3DTextAsTriangles(const SbBool flag)
{
  this->textastris = flag;
}

/*!
  Returns whether SoText3 nodes is counted as triangles or text.

  \sa is3DTextCountedAsTriangles()
*/
SbBool
SoGetPrimitiveCountAction::is3DTextCountedAsTriangles(void)
{
  return this->textastris;
}

/*!
  Returns whether shapes can use an approximate value when counting
  primitives. This is faster than doing an accurate count.  The
  default is to not approximate.

  \sa setCanApproximate()
*/
SbBool
SoGetPrimitiveCountAction::canApproximateCount(void)
{
  return this->approx;
}

/*!
  Sets whether shapes can do an approximate count.
  \sa canApproximateCount()
*/
void
SoGetPrimitiveCountAction::setCanApproximate(const SbBool flag)
{
  this->approx = flag;
}

/*!
  Set up the decimation parameters for the traversal.

  On-the-fly decimation is supported in Coin yet, so this call will
  not have any effect until this feature has been implemented.
*/
void
SoGetPrimitiveCountAction::setDecimationValue(SoDecimationTypeElement::Type type,
                                              float percentage)
{
  this->decimationtype = type;
  this->decimationpercentage = percentage;
}

/*!
  Returns decimation type used during the traversal count.
 */
SoDecimationTypeElement::Type
SoGetPrimitiveCountAction::getDecimationType(void)
{
  return this->decimationtype;
}

/*!
  Returns decimation percentage used during the traversal count.
 */
float
SoGetPrimitiveCountAction::getDecimationPercentage(void)
{
  return this->decimationpercentage;
}

/*!
  Adds \a num triangles to total count. Used by node instances in the
  scene graph during traversal.
*/
void
SoGetPrimitiveCountAction::addNumTriangles(const int num)
{
  this->numtris += num;
}

/*!
  Adds \a num lines to total count. Used by node instances in the
  scene graph during traversal.
*/
void
SoGetPrimitiveCountAction::addNumLines(const int num)
{
  this->numlines += num;
}

/*!
  Adds \a num points to total count. Used by node instances in the
  scene graph during traversal.
*/
void
SoGetPrimitiveCountAction::addNumPoints(const int num)
{
  this->numpoints = num;
}

/*!
  Adds \a num texts to total count. Used by node instances in the
  scene graph during traversal.
*/
void
SoGetPrimitiveCountAction::addNumText(const int num)
{
  this->numtexts += num;
}

/*!
  Adds \a num texture image maps to total count. Used by node
  instances in the scene graph during traversal.
*/
void
SoGetPrimitiveCountAction::addNumImage(const int num)
{
  this->numimages += num;
}

/*!
  Adds a single triangle to the total count. Used by node instances in
  the scene graph during traversal.
*/
void
SoGetPrimitiveCountAction::incNumTriangles(void)
{
  this->numtris++;
}

/*!
  Adds a single line to the total count. Used by node instances in the
  scene graph during traversal.
*/
void
SoGetPrimitiveCountAction::incNumLines(void)
{
  this->numlines++;
}

/*!
  Adds a single point to the total count. Used by node instances in
  the scene graph during traversal.
*/
void
SoGetPrimitiveCountAction::incNumPoints(void)
{
  this->numpoints++;
}

/*!
  Adds a single text to the total count. Used by node instances in the
  scene graph during traversal.
*/
void
SoGetPrimitiveCountAction::incNumText(void)
{
  this->numtexts++;
}

/*!
  Adds a single texture image map to the total count. Used by node
  instances in the scene graph during traversal.
*/
void
SoGetPrimitiveCountAction::incNumImage(void)
{
  this->numimages++;
}


/*!
  Overloaded to reset all counters to zero before traversal starts.
*/
void
SoGetPrimitiveCountAction::beginTraversal(SoNode * node)
{
  this->numtris = 0;
  this->numlines = 0;
  this->numpoints = 0;
  this->numtexts = 0;
  this->numimages = 0;

  // FIXME: set decimationtype and decimationpercentage elements.
  // 20000306 mortene.

  this->traverse(node);
}
