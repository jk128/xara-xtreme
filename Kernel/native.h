// $Id$
/* @@tag:xara-cn@@ DO NOT MODIFY THIS LINE
================================XARAHEADERSTART===========================
 
               Xara LX, a vector drawing and manipulation program.
                    Copyright (C) 1993-2006 Xara Group Ltd.
       Copyright on certain contributions may be held in joint with their
              respective authors. See AUTHORS file for details.

LICENSE TO USE AND MODIFY SOFTWARE
----------------------------------

This file is part of Xara LX.

Xara LX is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as published
by the Free Software Foundation.

Xara LX and its component source files are distributed in the hope
that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with Xara LX (see the file GPL in the root directory of the
distribution); if not, write to the Free Software Foundation, Inc., 51
Franklin St, Fifth Floor, Boston, MA  02110-1301 USA


ADDITIONAL RIGHTS
-----------------

Conditional upon your continuing compliance with the GNU General Public
License described above, Xara Group Ltd grants to you certain additional
rights. 

The additional rights are to use, modify, and distribute the software
together with the wxWidgets library, the wxXtra library, and the "CDraw"
library and any other such library that any version of Xara LX relased
by Xara Group Ltd requires in order to compile and execute, including
the static linking of that library to XaraLX. In the case of the
"CDraw" library, you may satisfy obligation under the GNU General Public
License to provide source code by providing a binary copy of the library
concerned and a copy of the license accompanying it.

Nothing in this section restricts any of the rights you have under
the GNU General Public License.


SCOPE OF LICENSE
----------------

This license applies to this program (XaraLX) and its constituent source
files only, and does not necessarily apply to other Xara products which may
in part share the same code base, and are subject to their own licensing
terms.

This license does not apply to files in the wxXtra directory, which
are built into a separate library, and are subject to the wxWindows
license contained within that directory in the file "WXXTRA-LICENSE".

This license does not apply to the binary libraries (if any) within
the "libs" directory, which are subject to a separate license contained
within that directory in the file "LIBS-LICENSE".


ARRANGEMENTS FOR CONTRIBUTION OF MODIFICATIONS
----------------------------------------------

Subject to the terms of the GNU Public License (see above), you are
free to do whatever you like with your modifications. However, you may
(at your option) wish contribute them to Xara's source tree. You can
find details of how to do this at:
  http://www.xaraxtreme.org/developers/

Prior to contributing your modifications, you will need to complete our
contributor agreement. This can be found at:
  http://www.xaraxtreme.org/developers/contribute/

Please note that Xara will not accept modifications which modify any of
the text between the start and end of this header (marked
XARAHEADERSTART and XARAHEADEREND).


MARKS
-----

Xara, Xara LX, Xara X, Xara X/Xtreme, Xara Xtreme, the Xtreme and Xara
designs are registered or unregistered trademarks, design-marks, and/or
service marks of Xara Group Ltd. All rights in these marks are reserved.


      Xara Group Ltd, Gaddesden Place, Hemel Hempstead, HP2 6EX, UK.
                        http://www.xara.com/

=================================XARAHEADEREND============================
 */
// The Camelot Native File format for Studio (version 1)

#ifndef INC_NATIVE
#define INC_NATIVE

//#include "camfiltr.h"	// 	BaseCamelotFilter - in camtypes.h [AUTOMATICALLY REMOVED]
#include "bmpprefs.h"	// SelectionType

class WebPrefsDlgParam;

/********************************************************************************************

>	class CamelotNativeFilter : public BaseCamelotFilter

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	25/5/96
	Purpose:	This filter is for the new Native file format which comes after the version
				1.1 ready for version 2.
				The BaseCamelotFilter provides common functionality for both this and the
				web file filter as they are essentially the same thing. 

********************************************************************************************/

class CamelotNativeFilter : public BaseCamelotFilter
{
	// Give my name in memory dumps
	CC_DECLARE_DYNAMIC(CamelotNativeFilter);

public:
	// Construction and initialisation
	CamelotNativeFilter();
	~CamelotNativeFilter();
	BOOL Init();

	virtual BOOL IsWebFilter() { return FALSE; }
	virtual BOOL IsCompactNativeFilter() { return FALSE; }

	// Find out how much compression is required for bitmaps
	virtual INT32 GetBitmapCompression(); 

	// Find out whether the user has requested a preview bitmap
	virtual BOOL GetPreviewBitmapExport(); 

	// Find out whether the user has requested convert text to outlines
	virtual BOOL GetConvertTextToOutlines(); 
	
	// Find out whether the user has requested remove invisible layers
	virtual BOOL GetRemoveInvisibleLayers(); 

	// Find out whether the user has requested to remove unused colours
	virtual BOOL GetRemoveUnusedColours(); 

	virtual INT32 HowCompatibleIsFileType(char* pFileType);

	virtual	double GetSimilarPathTolerance(NodePath* pPath);

	virtual INT32 GetMaxPathLookUp();

	virtual WebPrefsDlgParam* GetCachedExportOptions() { return NULL; }

protected:
	virtual BaseCamelotFilter* CreateNULLFilter(Document* pDocument);
	
	virtual char* GetExportFileType();	// Returns a three char long ASCII string

	virtual BOOL BeginDocumentExport();
	virtual BOOL EndDocumentExport();
	virtual BOOL WriteCurrentAttributes();

	// Resource IDs of the strings that describe this filter.
	UINT32 FilterNameID;
	UINT32 FilterInfoID;

	// The string to display when actually exporting rather than rendering.
	// 0 if we're not an export filter.
	// ExportMsgID is used in ExportRender when we are rendering the bitmap
	UINT32 ExportingMsgID;

public:

};

/********************************************************************************************

>	class CamelotNULLNativeFilter : public CamelotNativeFilter

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	4/6/96
	Purpose:	This is a dummy filter that is used to calculate the size of the file 
				that will be exported.

				It's like a NULL byte sink, where no bytes are actually outputted anywhere

********************************************************************************************/

class CamelotNULLNativeFilter : public CamelotNativeFilter
{
	// Give my name in memory dumps
	CC_DECLARE_DYNAMIC(CamelotNULLNativeFilter);

public:
	// Construction and initialisation
	CamelotNULLNativeFilter(BaseCamelotFilter* pParentFilter,Document* pDocument);

	// WEBSTER - markn 31/1/97
	// Replaced with general system
//	virtual void		AddTextStoryGroupRefToList(TextStory* pStory,NodeGroup* pGroup);
//	virtual NodeGroup*	FindTextStoryGroupRefGroup(TextStory* pStory);
//	virtual TextStory*	FindTextStoryGroupRefTextStory(NodeGroup* pGroup);

  	// WEBSTER - markn 29/1/97
	// Part of the general form of the system used to convert text to outlines in v1.5
	// Node & group references
	virtual void		AddNodeGroupRefToList(Node* pNode,NodeGroup* pGroup);
	virtual NodeGroup*	FindGroupForThisNode(Node* pNode);

	// WEBSTER - markn 11/2/97
	// System for writing tag descriptions to the file (introduced for the TAG_MOULD_BOUNDS record)
	virtual void AddTagDescription(UINT32 Tag, UINT32 ID);

protected:
	CXaraFile* CreateCXaraFile();

private:
	BaseCamelotFilter* pParentFilter;
};


#endif  // INC_NATIVE


