/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom attribute document

	@module	dcmattr.h | dicom attribute interface file

***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/


#ifndef _DCMATTR_H
#define	_DCMATTR_H

#define SOP_CR		"1.2.840.10008.5.1.4.1.1.1" //Computed Radiography Image Storage
#define SOP_CT		"1.2.840.10008.5.1.4.1.1.2" //CT Image Storage
#define SOP_UM		"1.2.840.10008.5.1.4.1.1.3" //Ultrasound Multi-frame Image Storage
#define SOP_MR		"1.2.840.10008.5.1.4.1.1.4" //MR Image Storage
#define SOP_NM		"1.2.840.10008.5.1.4.1.1.5" //Nuclear Medicine Image Storage
#define SOP_UL		"1.2.840.10008.5.1.4.1.1.6" //Ultrasound Image Storage
#define SOP_CAP		"1.2.840.10008.5.1.4.1.1.7" //Secondary Capture Image Storage
#define SOP_OVE		"1.2.840.10008.5.1.4.1.1.8" //Stand-alone Overlay Storage
#define SOP_CUR		"1.2.840.10008.5.1.4.1.1.9" //Stand-alone Curve Storage
#define SOP_MOD		"1.2.840.10008.5.1.4.1.1.10" //Stand-alone Modality LUT Storage
#define SOP_VOI		"1.2.840.10008.5.1.4.1.1.11" //Stand-alone VOI LUT Storage


//ISO_IR 100, ISO_IR 192 (UTF8), GB18030

#define DICM_ANNO_CIRCLE		_T("CIRCLE")
#define DICM_ANNO_TEXT			_T("TEXT")
#define DICM_ANNO_RANGE			_T("RANGE")
#define DICM_ANNO_ANGLE			_T("ANGLE")

#endif	/* _DCMATTR_H */

