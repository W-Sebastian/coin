/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SbName SbName.h Inventor/SbName.h
  \brief The SbName class stores strings by reference.
  \ingroup base

  The class is used by Coin for storing keywords, names and other
  strings. They are stored in a manner where identical strings are
  guaranteed to map to the same memory address (as returned by the
  SbName::getString() method).

  The main advantage of storing identical strings to the same memory
  address is that it simplifies comparison operations, and
  particularly when working with string data as keys in other data
  structures, like e.g. in hash (dictionary) tables.

  Apart from that, mapping identical strings to the same memory
  address can also save on memory resources, and provide run-time
  optimizations. String comparisons for SbName objects are very
  efficient, for instance.


  There is an aspect of using SbName instances that it is important to
  be aware of: since strings are stored \e permanently, using SbName
  instances in code where there is continually changing strings or the
  continual addition of new unique strings will in the end swamp
  memory resources. So where possible, use SbString instances instead.

  \sa SbString
*/

// *************************************************************************

#include <ctype.h>
#include <string.h>

#include <Inventor/SbName.h>

#include <Inventor/C/base/namemap.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/SbString.h>

// *************************************************************************

/*!
  This is the default constructor.
*/
SbName::SbName(void)
{
  this->permaaddress = cc_namemap_get_address("");
}

/*!
  Constructor. Adds the \a namestring string to the name table.
*/
SbName::SbName(const char * namestring)
{
  this->permaaddress = cc_namemap_get_address(namestring);
}

/*!
  Constructor. Adds \a str to the name table.
*/
SbName::SbName(const SbString & str)
{
  this->permaaddress = cc_namemap_get_address(str.getString());
}

/*!
  Copy constructor.
*/
SbName::SbName(const SbName & name)
{
  this->permaaddress = name.permaaddress;
}

/*!
  The destructor.
*/
SbName::~SbName()
{
  // No unreferences of memory resources happens here, because strings
  // will be stored permanently for the remaining life of the process.
  //
  // This is how the string mapping feature of SbName is *supposed* to
  // work. The strings should be stored _permanently_, so the return
  // value from SbName::getString() will be valid even after all its
  // SbName-instances have been destructed.
}

/*!
  This method returns pointer to character array for the name.

  The returned memory pointer for the character string is guaranteed
  to be valid for the remaining life time of the process, even after
  all SbName instances referencing the string has been destructed.
*/
const char *
SbName::getString(void) const
{
  return this->permaaddress;
}

/*!
  This method returns the number of characters in the name.
*/
int
SbName::getLength(void) const
{
  // FIXME: shouldn't we cache this value for subsequent faster
  // execution? 20010909 mortene.
  //
  // UPDATE 20030606 mortene: this can easily be done by storing an
  // extra value in the memory chunk allocated inside namemap.c, right
  // before the string itself.
  return strlen(this->permaaddress);
}

/*!
  This method checks if the \a c character is a valid identifier start
  character for a name.

  \sa SbBool SbName::isIdentChar(const char c)

*/
SbBool
SbName::isIdentStartChar(const char c)
{
  // There is an important reason why the cast below is necessary:
  // isdigit() et al takes an "int" as input argument. A _signed_ char
  // value for any character above the 127-value ASCII character set
  // will be promoted to a negative integer, which can cause the
  // function to make an array reference that's out of bounds.
  //
  // FIXME: this needs to be fixed other places isdigit() is used,
  // aswell as for other is*() function. 20021124 mortene.
  const unsigned char uc = (unsigned char)c;

  if (isdigit(uc)) return FALSE;
  return SbName::isIdentChar(c);
}

/*!
  This method checks if the \a c character is a valid character for a
  name.

  \sa SbBool SbName::isIdentStartChar(const char c)
*/
SbBool
SbName::isIdentChar(const char c)
{
  // FIXME: isalnum() takes the current locale into account. This can
  // lead to "interesting" artifacts. We very likely need to audit and
  // fix our isalnum() calls in the Coin sourcecode to behave in the
  // exact manner that we expect them to. 20020319 mortene.
  return (isalnum(c) || c == '_');
}

/*!
  Returns \c TRUE if the given character is valid for use as the first
  character of a name for an object derived from a class inheriting
  SoBase.

  SoBase derived objects needs to be named in a manner which will not
  clash with the special characters reserved as tokens in the syntax
  rules of Open Inventor and VRML files.

  This method is not part of the original Open Inventor API.

  \sa isBaseNameChar()
*/
SbBool
SbName::isBaseNameStartChar(const char c)
{
  // FIXME: isalpha() takes the current locale into account. This can
  // lead to "interesting" artifacts. We very likely need to audit and
  // fix our isalpha() calls in the Coin sourcecode to behave in the
  // exact manner that we expect them to. 20020319 mortene.
  if (c == '_' || (coin_isascii(c) && isalpha(c))) return TRUE;
  return FALSE;
}

/*!
  Returns \c TRUE if the given character is valid for use in naming
  object instances of classes derived from SoBase.

  SoBase derived objects needs to be named in a manner which will not
  clash with the special characters reserved as tokens in the syntax
  rules of Open Inventor and VRML files.

  This method is not part of the original Open Inventor API.

  \sa isBaseNameStartChar()
*/
SbBool
SbName::isBaseNameChar(const char c)
{
  static const char invalid[] = "\"\'+.\\{}";
  if (c <= 0x20 || c >= 0x7f || strchr(invalid, c)) return FALSE;
  return TRUE;
}

/*!
  This unary operator results in \c FALSE if the SbName object is
  non-empty and \c TRUE if the SbName object is empty.  An empty name
  contains a null-length string.
*/
int
SbName::operator!(void) const
{
  return this->permaaddress[0] == '\0';
}

/*!
  This operator checks for equality and returns \c TRUE if so, and \c
  FALSE otherwise.
*/
int
operator==(const SbName & lhs, const char * rhs)
{
  // Due to the nature of permanent unique mappings of same strings to
  // same address in the name hash, we can simple compare pointer
  // addresses.
  return lhs.permaaddress == rhs;
}

/*!
  This operator checks for equality and returns \c TRUE if so, and \c
  FALSE otherwise.
*/
int
operator==(const char * lhs, const SbName & rhs)
{
  // Due to the nature of permanent unique mappings of same strings to
  // same address in the name hash, we can simple compare pointer
  // addresses.
  return rhs.permaaddress == lhs;
}

/*!
  This operator checks for equality and returns \c TRUE if so, and \c
  FALSE otherwise.
*/
int
operator==(const SbName & lhs, const SbName & rhs)
{
  // Due to the nature of permanent unique mappings of same strings to
  // same address in the name hash, we can simple compare pointer
  // addresses.
  return lhs.permaaddress == rhs.permaaddress;
}

/*!
  This operator checks for inequality and returns \c TRUE if so, and
  \c FALSE if the names are equal.
*/
int
operator!=(const SbName & lhs, const char * rhs)
{
  return !(lhs == rhs);
}

/*!
  This operator checks for inequality and returns \c TRUE if so, and
  \c FALSE if the names are equal.
*/
int
operator!=(const char * lhs, const SbName & rhs)
{
  return !(lhs == rhs);
}

/*!
  This operator checks for inequality and returns \c TRUE if so, and
  \c FALSE if the names are equal.
*/
int
operator!=(const SbName & lhs, const SbName & rhs)
{
  return !(lhs == rhs);
}

/*!
  This operator returns a pointer to the character array for the name
  string.  It is intended for implicit use.  Use SbName::getString()
  explicitly instead of this operator - it might be removed later.

  \sa const char * SbName::getString(void)
*/
SbName::operator const char * (void) const
{
  return this->permaaddress;
}
