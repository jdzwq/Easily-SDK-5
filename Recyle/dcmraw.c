/*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the MPL as stated above or under the terms of the GNU
* Library General Public License as published by the Free Software Foundation;
* either version 2 of the License, or any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Library general Public License for more
* details.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dcmraw.h"

typedef enum{
	DICM_PARSE_END = 0,
	DICM_PARSE_TAG = 1,
	DICM_PARSE_VR = 2,
	DICM_PARSE_SIZE = 3,
	DICM_PARSE_DATA = 4,
	DICM_PARSE_SQ_BEGIN = 5,
	DICM_PARSE_SQ_ITEM_BEGIN = 6,
	DICM_PARSE_SQ_ITEM_END = 7,
	DICM_PARSE_SQ_END = 8
};

typedef enum{
	DICM_FORMAT_END = 0,
	DICM_FORMAT_TAG = 1,
	DICM_FORMAT_VR = 2,
	DICM_FORMAT_SIZE = 3,
	DICM_FORMAT_DATA = 4,
	DICM_FORMAT_SQ_BEGIN = 5,
	DICM_FORMAT_SQ_ITEM_BEGIN = 6,
	DICM_FORMAT_SQ_ITEM_END = 7,
	DICM_FORMAT_SQ_END = 8

};

const dicm_vr_t DICM_VR[] = {
	{ ("AE"), 16 },
	{ ("AS"), 4 },
	{ ("AT"), 4 },
	{ ("CS"), 16 },
	{ ("DA"), 8 },
	{ ("DS"), 16 },
	{ ("DT"), 26 },
	{ ("FL"), 4 },
	{ ("FD"), 8 },
	{ ("IS"), 12 },
	{ ("LO"), 64 },
	{ ("LT"), 10240 },
	{ ("OB"), 0 },
	{ ("OW"), 0 },
	{ ("PN"), 64 },
	{ ("SH"), 16 },
	{ ("SL"), 4 },
	{ ("SQ"), 0 },
	{ ("SS"), 2 },
	{ ("ST"), 1024 },
	{ ("TM"), 16 },
	{ ("UI"), 64 },
	{ ("UL"), 4 },
	{ ("US"), 2 }
};


const dicm_item_t DICM_ITEMS[] = {
	{ 0x00020000, ("UL"), ("FileMetaInfoGroupLength") },
	{ 0x00020001, ("OB"), ("FileMetaInfoVersion") },
	{ 0x00020002, ("UI"), ("MediaStorageSOPClassUID") },
	{ 0x00020003, ("UI"), ("MediaStorageSOPInstanceUID") },
	{ 0x00020010, ("UI"), ("TransferSyntaxUID") },
	{ 0x00020012, ("UI"), ("ImplementationClassUID") },
	{ 0x00020013, ("SH"), ("ImplementationVersionName") },
	{ 0x00020016, ("AE"), ("SourceApplicationEntityTitle") },
	{ 0x00020100, (""), ("PrivateInformationCreatorUID") },
	{ 0x00020102, (""), ("PrivateInformation") },

	{ 0x00041130, (""), ("FileSetID") },
	{ 0x00041141, (""), ("FileSetDescriptorFileID") },
	{ 0x00041142, (""), ("SpecificCharacterSetOfFile") },
	{ 0x00041200, (""), ("FirstDirectoryRecordOffset") },
	{ 0x00041202, (""), ("LastDirectoryRecordOffset") },
	{ 0x00041212, (""), ("FileSetConsistencyFlag") },
	{ 0x00041220, (""), ("DirectoryRecordSequence") },
	{ 0x00041400, (""), ("OffsetOfNextDirectoryRecord") },
	{ 0x00041410, (""), ("RecordInUseFlag") },
	{ 0x00041420, (""), ("LowerLevelDirectoryEntityOffset") },
	{ 0x00041430, (""), ("DirectoryRecordType") },
	{ 0x00041432, (""), ("PrivateRecordUID") },
	{ 0x00041500, (""), ("ReferencedFileID") },
	{ 0x00041504, (""), ("MRDRDirectoryRecordOffset") },
	{ 0x00041510, (""), ("ReferencedSOPClassUIDInFile") },
	{ 0x00041511, (""), ("ReferencedSOPInstanceUIDInFile") },
	{ 0x00041512, (""), ("ReferencedTransferSyntaxUIDInFile") },
	{ 0x0004151A, (""), ("ReferencedRelatedSOPClassUIDInFile") },
	{ 0x00041600, (""), ("NumberOfReferences") },

	{ 0x00080000, (""), ("IdentifyingGroupLength") },
	{ 0x00080001, (""), ("LengthToEnd") },
	{ 0x00080005, (""), ("SpecificCharacterSet") },
	{ 0x00080006, (""), ("LanguageCodeSequence") },
	{ 0x00080008, (""), ("ImageType") },
	{ 0x00080010, (""), ("RecognitionCode") },
	{ 0x00080012, (""), ("InstanceCreationDate") },
	{ 0x00080013, (""), ("InstanceCreationTime") },
	{ 0x00080014, (""), ("InstanceCreatorUID") },
	{ 0x00080016, ("UI"), ("SOPClassUID") },
	{ 0x00080018, ("UI"), ("SOPInstanceUID") },
	{ 0x0008001A, (""), ("RelatedGeneralSOPClassUID") },
	{ 0x0008001B, (""), ("OriginalSpecializedSOPClassUID") },
	{ 0x00080020, ("DA"), ("StudyDate") },
	{ 0x00080021, ("DA"), ("SeriesDate") },
	{ 0x00080022, (""), ("AcquisitionDate") },
	{ 0x00080023, ("DA"), ("ContentDate") },
	{ 0x00080024, (""), ("OverlayDate") },
	{ 0x00080025, (""), ("CurveDate") },
	{ 0x0008002A, (""), ("AcquisitionDateTime") },
	{ 0x00080030, ("TM"), ("StudyTime") },
	{ 0x00080031, ("TM"), ("SeriesTime") },
	{ 0x00080032, (""), ("AcquisitionTime") },
	{ 0x00080033, ("TM"), ("ContentTime") },
	{ 0x00080034, (""), ("OverlayTime") },
	{ 0x00080035, (""), ("CurveTime") },
	{ 0x00080040, (""), ("DataSetType") },
	{ 0x00080041, (""), ("DataSetSubtype") },
	{ 0x00080042, (""), ("NuclearMedicineSeriesType") },
	{ 0x00080050, ("SH"), ("AccessionNumber") },
	{ 0x00080052, (""), ("QueryRetrieveLevel") },
	{ 0x00080054, (""), ("RetrieveAETitle") },
	{ 0x00080056, (""), ("InstanceAvailability") },
	{ 0x00080058, (""), ("FailedSOPInstanceUIDList") },
	{ 0x00080060, ("CS"), ("Modality") },
	{ 0x00080061, (""), ("ModalitiesInStudy") },
	{ 0x00080062, (""), ("SOPClassesInStudy") },
	{ 0x00080064, (""), ("ConversionType") },
	{ 0x00080068, (""), ("PresentationIntentType") },
	{ 0x00080070, (""), ("Manufacturer") },
	{ 0x00080080, (""), ("InstitutionName") },
	{ 0x00080081, (""), ("InstitutionAddress") },
	{ 0x00080082, (""), ("InstitutionCodeSequence") },
	{ 0x00080090, (""), ("ReferringPhysicianName") },
	{ 0x00080092, (""), ("ReferringPhysicianAddress") },
	{ 0x00080094, (""), ("ReferringPhysicianTelephoneNumber") },
	{ 0x00080096, (""), ("ReferringPhysicianIDSequence") },
	{ 0x00080100, ("US"), ("CodeValue") },
	{ 0x00080102, (""), ("CodingSchemeDesignator") },
	{ 0x00080103, (""), ("CodingSchemeVersion") },
	{ 0x00080104, (""), ("CodeMeaning") },
	{ 0x00080105, (""), ("MappingResource") },
	{ 0x00080106, (""), ("ContextGroupVersion") },
	{ 0x00080107, (""), ("ContextGroupLocalVersion") },
	{ 0x0008010B, (""), ("ContextGroupExtensionFlag") },
	{ 0x0008010C, (""), ("CodingSchemeUID") },
	{ 0x0008010D, (""), ("ContextGroupExtensionCreatorUID") },
	{ 0x0008010F, (""), ("ContextIdentifier") },
	{ 0x00080110, (""), ("CodingSchemeIDSequence") },
	{ 0x00080112, (""), ("CodingSchemeRegistry") },
	{ 0x00080114, (""), ("CodingSchemeExternalID") },
	{ 0x00080115, (""), ("CodingSchemeName") },
	{ 0x00080116, (""), ("ResponsibleOrganization") },
	{ 0x00080117, (""), ("ContextUID") },
	{ 0x00080201, (""), ("TimezoneOffsetFromUTC") },
	{ 0x00081000, (""), ("NetworkID") },
	{ 0x00081010, (""), ("StationName") },
	{ 0x00081090, (""), ("ManufacturersModelName") },

	{ 0x00100010, ("PN"), ("PatientName") },
	{ 0x00100020, ("LO"), ("PatientID") },
	{ 0x00100021, (""), ("IssuerOfPatientID") },
	{ 0x00100022, (""), ("TypeOfPatientID") },
	{ 0x00100030, ("DA"), ("PatientBirthDate") },
	{ 0x00100032, (""), ("PatientBirthTime") },
	{ 0x00100040, ("CS"), ("PatientSex") },
	{ 0x00101010, ("AS"), ("PatientAge") },
	{ 0x00101020, (""), ("PatientSize") },
	{ 0x00101030, (""), ("PatientWeight") },
	{ 0x00101040, (""), ("PatientAddress") },
	{ 0x001021C0, (""), ("PregnancyStatus") },

	{ 0x00180030, (""), ("Radionuclide") },
	{ 0x00180040, (""), ("CineRate") },
	{ 0x00180050, (""), ("SliceThickness") },
	{ 0x00180060, (""), ("KVP") },
	{ 0x00181100, (""), ("ReconstructionDiameter") },
	{ 0x00181110, (""), ("DistanceSourceToDetector") },
	{ 0x00181111, (""), ("DistanceSourceToPatient") },
	{ 0x00181114, (""), ("EstimatedRadiographicMagnification") },
	{ 0x00181120, (""), ("GantryDetectorTilt") },
	{ 0x00181121, (""), ("GantryDetectorSlew") },
	{ 0x00181130, (""), ("TableHeight") },
	{ 0x00181131, (""), ("TableTraverse") },
	{ 0x00181134, (""), ("TableMotion") },
	{ 0x00181150, (""), ("ExposureTime") },
	{ 0x00181151, (""), ("XRayTubeCurrent") },

	{ 0x0020000D, ("UI"), ("StudyInstanceUID") },
	{ 0x0020000E, ("UI"), ("SeriesInstanceUID") },
	{ 0x00200010, (""), ("StudyID") },
	{ 0x00200011, ("IS"), ("SeriesNumber") },
	{ 0x00200012, (""), ("AcquisitionNumber") },
	{ 0x00200013, ("IS"), ("InstanceNumber") },
	{ 0x00200014, (""), ("IsotopeNumber") },
	{ 0x00200015, (""), ("PhaseNumber") },
	{ 0x00200016, (""), ("IntervalNumber") },
	{ 0x00200017, (""), ("TimeSlotNumber") },
	{ 0x00200018, (""), ("AngleNumber") },
	{ 0x00200019, (""), ("ItemNumber") },
	{ 0x00200020, (""), ("PatientOrientation") },
	{ 0x00200022, (""), ("OverlayNumber") },
	{ 0x00200024, (""), ("CurveNumber") },
	{ 0x00200026, (""), ("LookupTableNumber") },
	{ 0x00200030, (""), ("ImagePosition") },
	{ 0x00200032, (""), ("ImagePositionPatient")},
	{ 0x00200035, (""), ("ImageOrientation") },
	{ 0x00200037, (""), ("ImageOrientationPatient") },
	{ 0x00201000, (""), ("SeriesInStudy") },
	{ 0x00201001, (""), ("AcquisitionsInSeries") },
	{ 0x00201002, (""), ("ImagesInAcquisition") },
	{ 0x00201003, (""), ("ImagesInSeries") },
	{ 0x00201004, (""), ("AcquisitionsInStudy") },
	{ 0x00201005, (""), ("ImagesInStudy") },

	{ 0x00280002, ("US"), ("SamplesPerPixel") },
	{ 0x00280003, (""), ("SamplesPerPixelUsed") },
	{ 0x00280004, ("CS"), ("PhotometricInterpretation") },
	{ 0x00280005, (""), ("ImageDimensions") },
	{ 0x00280006, ("US"), ("PlanarConfiguration") },
	{ 0x00280008, (""), ("NumberOfFrames") },
	{ 0x00280009, (""), ("FrameIncrementPointer") },
	{ 0x0028000A, (""), ("FrameDimensionPointer") },
	{ 0x00280010, ("US"), ("Rows") },
	{ 0x00280011, ("US"), ("Columns") },
	{ 0x00280012, (""), ("Planes") },
	{ 0x00280014, (""), ("UltrasoundColorDataPresent") },
	{ 0x00280030, (""), ("PixelSpacing") },
	{ 0x00280031, (""), ("ZoomFactor") },
	{ 0x00280032, (""), ("ZoomCenter") },
	{ 0x00280034, (""), ("PixelAspectRatio") },

	{ 0x00280100, ("US"), ("BitsAllocated") },
	{ 0x00280101, ("US"), ("BitsStored") },
	{ 0x00280102, ("US"), ("HighBit") },
	{ 0x00280103, ("US"), ("PixelRepresentation") },
	{ 0x00281050, (""), ("WindowCenter") },
	{ 0x00281051, (""), ("WindowWidth") },
	{ 0x00281052, (""), ("RescaleIntercept") },
	{ 0x00281053, (""), ("RescaleSlope") },
	{ 0x00281054, (""), ("RescaleType") },

	{ 0x00700001, (""), ("GraphicAnnotationSequence") },
	{ 0x00700002, (""), ("GraphicLayer") },
	{ 0x00700003, (""), ("BoundingBoxAnnotationUnits") },
	{ 0x00700004, (""), ("AnchorPointAnnotationUnits") },
	{ 0x00700005, (""), ("GraphicAnnotationUnits") },
	{ 0x00700006, (""), ("UnformattedTextValue") },
	{ 0x00700008, (""), ("TextObjectSequence") },
	{ 0x00700009, (""), ("GraphicObjectSequence") },
	{ 0x00700010, (""), ("BoundingBoxTopLeftHandCorner") },
	{ 0x00700011, (""), ("BoundingBoxBottomRightHandCorner") },
	{ 0x00700012, (""), ("BoundingBoxTextHorizJustification") },
	{ 0x00700014, (""), ("AnchorPoint") },
	{ 0x00700015, (""), ("AnchorPointVisibility") },
	{ 0x00700020, (""), ("GraphicDimensions") },
	{ 0x00700021, (""), ("NumberOfGraphicPoints") },
	{ 0x00700022, (""), ("GraphicData") },
	{ 0x00700023, (""), ("GraphicType") },
	{ 0x00700024, (""), ("GraphicFilled") },

	{ 0x7FE00010, ("OW"), ("PixelData") },

	{ 0xFFFEE000, (""), ("StartOfItem") },
	{ 0xFFFEE00D, (""), ("EndOfItems") },
	{ 0xFFFEE0DD, (""), ("EndOfSequence") },
};

const dicm_group_t DICM_GROUPS[] = {
	{ 0x0002, ("FileMeta") },
	{ 0x0004, ("FileSet") },
	{ 0x0008, ("Identifying") },
	{ 0x0010, ("Patient") },
	{ 0x0018, ("Acquisition") },
	{ 0x0020, ("Relationship") },
	{ 0x0028, ("Presentation") },
	{ 0x0032, ("Study") },
	{ 0x0070, ("Annotation") },
	{ 0x7FE0, ("PixelData") },
};

const unsigned long SQTAGS[] = {
	0x00080082, //Institution Code Sequence
	0x00081032, //Procedure Code Sequence
	0x00081084, //Admitting Diagnosis Code Sequence
	0x00081100, //Referenced Results Sequence
	0x00081110, //Referenced Study Sequence
	0x00081111, //Referenced Study Component Sequence
	0x00081115, //Referenced Series Sequence
	0x00081120, //Referenced Patient Sequence
	0x00081125, //Referenced Visit Sequence
	0x00081130, //Referenced Overlay Sequence
	0x00081140, //Referenced Image Sequence
	0x00081145, //Referenced Curve Sequence
	0x00082112, //Source Image Sequence
	0x00100050, //Patient's Insurance Plan Code Sequence
	0x00186011, //Sequence of Ultrasound Regions
	0x00283000, //Modality LUT Sequence
	0x00283010, //VOI LUT Sequence
	0x00321064, //Requested Procedure Code Sequence
	0x00380004, //Referenced Patient Alias Sequence
	0x00380044, //Discharge Diagnosis Code Sequence
	0x20000500, //Referenced Film Box Sequence
	0x20100500, //Referenced Film Session Sequence
	0x20100510, //Referenced Image Box Sequence
	0x20100520, //Referenced Basic Annotation Box Sequence
	0x20200110, //Preformatted Grayscale Image Sequence
	0x20200111, //Preformatted Color Image Sequence
	0x20200130, //Referenced Image Overlay Box Sequence
	0x20200140, //Referenced VOI LUT Box Sequence
	0x20400010, //Referenced Overlay Plane Sequence
	0x20400500, //Referenced Image Box Sequence
	0x21000500, //Referenced Print Job Sequence
	0x40080050, //Referenced Interpretation Sequence
	0x40080111, //Interpretation Approver Sequence
	0x40080117, //Diagnosis Code Sequence
	0x40080118, //Results Distribution List Sequence
};


#define MAKEULONG(l,h)	(unsigned long)((l & 0x0000FFFF) | (h << 16))

static int is_sq_tag(unsigned long tag)
{
	int n = sizeof(SQTAGS) / sizeof(unsigned long);

	for (int i = 0; i < n; i++)
	{
		if (tag == SQTAGS[i])
			return 1;
	}

	return 0;
}

static void _bytes_turn(unsigned char* ba, int n)
{
	unsigned char b;
	int i;

	for (i = 0; i < n / 2; i++)
	{
		b = ba[i];
		ba[i] = ba[n - 1 - i];
		ba[n - 1 - i] = b;
	}
}

static int _find_item_index(unsigned long tag, int l, int h)
{
	int i;

	i = (l + h) / 2;

	if (i < l || i > h)
		return -1;
	if (tag < DICM_ITEMS[i].tag)
		return _find_item_index(tag, l, i - 1);
	else if (tag > DICM_ITEMS[i].tag)
		return _find_item_index(tag, i + 1, h);
	else
		return i;
}

static int find_item_index(unsigned long tag)
{
	int n;

	n = sizeof(DICM_ITEMS) / sizeof(dicm_item_t);

	return _find_item_index(tag, 0, n - 1);
}

const dicm_item_t* dicm_item_info(unsigned long tag)
{
	int i;

	i = find_item_index(tag);

	if (i < 0)
		return NULL;
	else
		return &(DICM_ITEMS[i]);
}

const dicm_group_t* dicm_group_info(unsigned long tag)
{
	unsigned short sw;
	int i, n;

	sw = (tag & 0xFFFF0000) >> 16;

	n = sizeof(DICM_GROUPS) / sizeof(dicm_group_t);

	for (i = 0; i < n; i++)
	{
		if (sw == DICM_GROUPS[i].tag)
			return &(DICM_GROUPS[i]);
		else if (sw < DICM_GROUPS[i].tag)
			break;
	}

	return NULL;
}

dicm_context_t* dicm_alloc(void)
{
	dicm_context_t* ctx;

	ctx = (dicm_context_t*)calloc(1, sizeof(dicm_context_t));
	memset((void*)ctx, 0, sizeof(dicm_context_t));

	ctx->b_Implicit = 0;
	ctx->b_BigEndian = 0;
	ctx->n_CharSet = 0;

	return ctx;
}

void dicm_free(dicm_context_t* ctx)
{
	free(ctx);
}

void dicm_set_read(dicm_context_t* pdcm, void* rp, PF_DICM_READ pf_read)
{
	pdcm->rp = rp;
	pdcm->pf_dicm_read = pf_read;
}

void dicm_set_write(dicm_context_t* pdcm, void* wp, PF_DICM_WRITE pf_write)
{
	pdcm->wp = wp;
	pdcm->pf_dicm_write = pf_write;
}

void dicm_set_error(dicm_context_t* pdcm, void* ep, PF_DICM_ERROR pf_error)
{
	pdcm->ep = ep;
	pdcm->pf_dicm_error = pf_error;
}

void dicm_set_parse(dicm_context_t* pdcm, void* pp, PF_DICM_PARSE_ITEM pf_parse_item, PF_DICM_PARSE_DATA pf_parse_data, PF_DICM_PARSE_SEQUENCE_BEGIN pf_parse_sequence_begin, PF_DICM_PARSE_SEQUENCE_END pf_parse_sequence_end, PF_DICM_PARSE_SEQUENCE_ITEM_BEGIN pf_parse_sequence_item_begin, PF_DICM_PARSE_SEQUENCE_ITEM_END pf_parse_sequence_item_end)
{
	pdcm->pp = pp;
	pdcm->pf_dicm_parse_item = pf_parse_item;
	pdcm->pf_dicm_parse_data = pf_parse_data;
	pdcm->pf_dicm_parse_sequence_begin = pf_parse_sequence_begin;
	pdcm->pf_dicm_parse_sequence_end = pf_parse_sequence_end;
	pdcm->pf_dicm_parse_sequence_item_begin = pf_parse_sequence_item_begin;
	pdcm->pf_dicm_parse_sequence_item_end = pf_parse_sequence_item_end;
}

void dicm_set_format(dicm_context_t* pdcm, void* fp, PF_DICM_FORMAT_ITEM pf_format_item, PF_DICM_FORMAT_DATA pf_format_data, PF_DICM_FORMAT_SEQUENCE_BEGIN pf_format_sequence_begin, PF_DICM_FORMAT_SEQUENCE_END pf_format_sequence_end, PF_DICM_FORMAT_SEQUENCE_ITEM_BEGIN pf_format_sequence_item_begin, PF_DICM_FORMAT_SEQUENCE_ITEM_END pf_format_sequence_item_end)
{
	pdcm->fp = fp;
	pdcm->pf_dicm_format_item = pf_format_item;
	pdcm->pf_dicm_format_data = pf_format_data;
	pdcm->pf_dicm_format_sequence_begin = pf_format_sequence_begin;
	pdcm->pf_dicm_format_sequence_end = pf_format_sequence_end;
	pdcm->pf_dicm_format_sequence_item_begin = pf_format_sequence_item_begin;
	pdcm->pf_dicm_format_sequence_item_end = pf_format_sequence_item_end;
}

int _dicm_parse_head(dicm_context_t* pdcm)
{
	unsigned char head[128] = { 0 };
	unsigned char dicm[5] = { 0 };

	unsigned long n;

	//skip file head
	n = 128;
	(*pdcm->pf_dicm_read)(pdcm->rp, head, &n);
	if (n < 128)
	{
		return 0;
	}

	//dicm head
	n = 4;
	(*pdcm->pf_dicm_read)(pdcm->rp, dicm, &n);
	if (n < 4)
	{
		return 0;
	}

	if (strnicmp(dicm, "DICM", 4) != 0)
	{
		return 0;
	}

	return 1;
}

static void _dec_size(unsigned long* p, int i, unsigned long d)
{
	do{
		if (p[i] != 0xFFFFFFFF)
			p[i] -= d;
	} while (i--);
}

int _dicm_parse_body(dicm_context_t* pdcm)
{
	char errtext[256] = { 0 };

	unsigned short vt[2] = { 0 };
	unsigned char vr[2] = { 0 };
	unsigned long vl = 0;
	unsigned char* vf = NULL;

	unsigned long ul = 0;
	unsigned short us = 0;
	unsigned char rev[4] = { 0 };

	unsigned long n;
	unsigned long tag;
	int b_sq = 0;
	int rt, i, step;
	
	unsigned long size[256] = { 0 };
	int deep = 0;

	size[deep] = 0xFFFFFFFF;
	step = DICM_PARSE_TAG;

	while (step != DICM_PARSE_END)
	{
		switch (step)
		{
		case DICM_PARSE_TAG:
			//element tag
			n = sizeof(short);
			if(!(*pdcm->pf_dicm_read)(pdcm->rp, (unsigned char*)vt, &n))
			{
				goto ERRRET;
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);
			
			if (pdcm->b_BigEndian)
			{
				_bytes_turn((unsigned char*)vt, n);
			}

			n = sizeof(short);
			if(!(*pdcm->pf_dicm_read)(pdcm->rp, (unsigned char*)(vt + 1), &n))
			{
				goto ERRRET;
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);
			
			if (pdcm->b_BigEndian)
			{
				_bytes_turn((unsigned char*)(vt + 1), n);
			}

			tag = MAKEULONG(vt[1], vt[0]);

			if (tag == 0xFFFEE000 || tag == 0xFFFEE00D || tag == 0xFFFEE0DD)
			{
				step = DICM_PARSE_SIZE;
			}
			else
			{
				if (pdcm->b_Implicit && !IS_META_TAG(tag))
				{
					b_sq = is_sq_tag(tag);
					step = DICM_PARSE_SIZE;
				}
				else
				{
					step = DICM_PARSE_VR;
				}
			}

			vr[0] = vr[1] = 0;
			vl = 0;
			vf = NULL;
			break;
		case DICM_PARSE_VR:
			//element vr
			n = sizeof(short);
			if(!(*pdcm->pf_dicm_read)(pdcm->rp, (unsigned char*)vr, &n))
			{
				goto ERRRET;
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);
			
			if (pdcm->b_BigEndian)
			{
				_bytes_turn((unsigned char*)vr, n);
			}

			b_sq = (vr[0] == 'S' && vr[1] == 'Q') ? 1 : 0;

			if (!pdcm->b_Implicit || IS_META_TAG(tag))
			{
				if ((vr[0] == 'O' && vr[1] == 'B') || (vr[0] == 'O' && vr[1] == 'W') || (vr[0] == 'S' && vr[1] == 'Q'))
				{
					//skip 2 bytes
					n = sizeof(short);
					if (!(*pdcm->pf_dicm_read)(pdcm->rp, (unsigned char*)rev, &n))
					{
						goto ERRRET;
					}
					if (!n)
					{
						step = DICM_PARSE_END;
						break;
					}
					_dec_size(size, deep, n);

					step = DICM_PARSE_SIZE;
				}
				else
				{
					//short size
					us = 0;
					n = sizeof(short);
					if (!(*pdcm->pf_dicm_read)(pdcm->rp, (unsigned char*)&us, &n))
					{
						goto ERRRET;
					}
					if (!n)
					{
						step = DICM_PARSE_END;
						break;
					}
					_dec_size(size, deep, n);

					if (pdcm->b_BigEndian)
					{
						_bytes_turn((unsigned char*)&us, n);
					}

					vl = (unsigned long)us;
					size[++deep] = 0xFFFFFFFF;

					if (pdcm->pf_dicm_parse_item)
						rt = (*pdcm->pf_dicm_parse_item)(pdcm->pp, tag, vr, vl);
					else
						rt = DICM_CONTINUE;

					if (rt != DICM_CONTINUE)
					{
						goto ERRRET;
					}

					step = DICM_PARSE_DATA;
				}
			}
			else
			{
				step = DICM_PARSE_SIZE;
			}
			break;
		case DICM_PARSE_SIZE:
			//element size
			ul = 0;
			n = sizeof(long);
			if (!(*pdcm->pf_dicm_read)(pdcm->rp, (unsigned char*)&ul, &n))
			{
				goto ERRRET;
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);

			if (pdcm->b_BigEndian)
			{
				_bytes_turn((unsigned char*)&ul, n);
			}

			vl = ul;
			if (tag != 0xFFFEE00D && tag != 0xFFFEE0DD)
			{
				if (b_sq || tag == 0xFFFEE000)
					size[++deep] = vl;
				else
					size[++deep] = 0xFFFFFFFF;
			}

			if (tag == 0xFFFEE000)
			{
				step = DICM_PARSE_SQ_ITEM_BEGIN;
				break;
			}
			else if (tag == 0xFFFEE00D)
			{
				step = DICM_PARSE_SQ_ITEM_END;
				break;
			}
			else if (tag == 0xFFFEE0DD)
			{
				step = DICM_PARSE_SQ_END;
				break;
			}
			else
			{
				if (pdcm->pf_dicm_parse_item)
					rt = (*pdcm->pf_dicm_parse_item)(pdcm->pp, tag, vr, vl);
				else
					rt = DICM_CONTINUE;

				if (rt != DICM_CONTINUE)
				{
					goto ERRRET;
				}

				if (b_sq)
				{
					step = DICM_PARSE_SQ_BEGIN;
				}
				else
				{
					step = DICM_PARSE_DATA;
				}
			}
			break;
		case DICM_PARSE_DATA:
			//element value
			if (vl > 0x7FFFFFFF)
			{
				goto ERRRET;
			}

			vf = (unsigned char*)calloc(vl + 2, sizeof(char));
			n = vl;
			if(!(*pdcm->pf_dicm_read)(pdcm->rp, (unsigned char*)vf, &n))
			{
				goto ERRRET;
			}
			_dec_size(size, deep, n);

			if (vr[0] == 'O' && vr[1] == 'W')
			{
				if (pdcm->b_BigEndian)
				{
					for (i = 0; i < (int)(n / 2); i += 2)
					{
						_bytes_turn(vf + i, 2);
					}
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 4);
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'D') //8
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 8);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 2);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 4);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 2);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 4);
				}
			}

			if (tag == 0x00020010)
			{
				if (strcmp(vf, "1.2.840.10008.1.2.1") == 0)
				{
					pdcm->b_Implicit = 0;
					pdcm->b_BigEndian = 0;
				}
				else if (strcmp(vf, "1.2.840.10008.1.2.2") == 0)
				{
					pdcm->b_Implicit = 0;
					pdcm->b_BigEndian = 1;
				}
				else if (strcmp(vf, "1.2.840.10008.1.2") == 0)
				{
					pdcm->b_Implicit = 1;
					pdcm->b_BigEndian = 0;
				}
			}
			else if (tag == 0x00080005)
			{
				if (strnicmp(vf, "ISO_IR 192", 10) == 0)
					pdcm->n_CharSet = DICM_CHARSET_UTF;
				else if (strnicmp(vf, "GB18030", 7) == 0)
					pdcm->n_CharSet = DICM_CHARSET_GBK;
				else
					pdcm->n_CharSet = DICM_CHARSET_ENG;
			}

			if (pdcm->pf_dicm_parse_data)
				rt = (*pdcm->pf_dicm_parse_data)(pdcm->pp, tag, vr, vf, vl);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				goto ERRRET;
			}

			free(vf);
			vf = NULL;

			deep--;

			if (!size[deep])
				step = DICM_PARSE_SQ_ITEM_END;
			else
				step = DICM_PARSE_TAG;
			break;
		case DICM_PARSE_SQ_BEGIN:
			if (pdcm->pf_dicm_parse_sequence_begin)
				rt = (*pdcm->pf_dicm_parse_sequence_begin)(pdcm->pp, tag, vl);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				goto ERRRET;
			}

			step = DICM_PARSE_TAG;
			break;
		case DICM_PARSE_SQ_ITEM_BEGIN:
			if (pdcm->pf_dicm_parse_sequence_item_begin)
				rt = (*pdcm->pf_dicm_parse_sequence_item_begin)(pdcm->pp, tag, vl);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				goto ERRRET;
			}

			step = DICM_PARSE_TAG;
			break;
		case DICM_PARSE_SQ_ITEM_END:
			if (pdcm->pf_dicm_parse_sequence_item_end)
				rt = (*pdcm->pf_dicm_parse_sequence_item_end)(pdcm->pp, tag);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				goto ERRRET;
			}

			deep--;

			if (size[deep])
				step = DICM_PARSE_TAG;
			else
				step = DICM_PARSE_SQ_END;
			break;
		case DICM_PARSE_SQ_END:
			if (pdcm->pf_dicm_parse_sequence_end)
				rt = (*pdcm->pf_dicm_parse_sequence_end)(pdcm->pp, tag);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				goto ERRRET;
			}

			deep--;

			step = DICM_PARSE_TAG;
			break;
		}
	}

	return 1;

ERRRET:

	if (vf)
		free(vf);

	if (pdcm->pf_dicm_error)
	{
		sprintf(errtext, "0x%04X%04X", vt[0], vt[1]);
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return 0;
}

int dicm_parse(dicm_context_t* pdcm)
{
	if(!_dicm_parse_head(pdcm))
		return 0;

	return _dicm_parse_body(pdcm);
}

/*************************************************************************************************************/
int _dicm_format_head(dicm_context_t* pdcm)
{
	unsigned char head[128] = { 0 };
	unsigned char dicm[5] = { 0 };

	unsigned long n;

	n = 128;
	(*pdcm->pf_dicm_write)(pdcm->wp, head, &n);
	if (n < 128)
	{
		return 0;
	}

	a_xscpy((schar_t*)dicm, "DICM");
	n = 4;
	(*pdcm->pf_dicm_write)(pdcm->wp, dicm, &n);
	if (n < 4)
	{
		return 0;
	}

	return 1;

ONERROR:

	if (pdcm->pf_dicm_error)
	{
		get_last_error(errcode, errtext, ERR_LEN);
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return 0;
}

int _dicm_format_body(dicm_context_t* pdcm)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	unsigned long tag;
	unsigned short vt[2] = { 0 };
	unsigned char vr[3] = { 0 };
	unsigned char* vf = NULL;
	unsigned long vl = 0;
	int b_sq = 0;

	unsigned long n;
	int i, rt;
	int step;

	unsigned char ub[2];
	unsigned long ul;
	unsigned short us;

	unsigned long size[256] = { 0 };
	int deep = 0;

	TRY_CATCH;

	size[deep] = 0xFFFFFFFF;
	step = DICM_FORMAT_TAG;

	while (step != DICM_FORMAT_END)
	{
		switch (step)
		{
		case DICM_FORMAT_TAG:
			if (pdcm->pf_dicm_format_item)
				rt = (*pdcm->pf_dicm_format_item)(pdcm->fp, &tag, vr, &vl);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				step = DICM_FORMAT_END;
				break;
			}

			vt[0] = GETHSWORD(tag);
			vt[1] = GETLSWORD(tag);

			n = sizeof(short);
			(*pdcm->pf_dicm_write)(pdcm->wp, (unsigned char*)vt, &n);
			if (!n)
			{
				raise_user_error(("dcm_format"), ("write dicm item tag failed"));
			}
			_dec_size(size, deep, n);

			n = sizeof(short);
			(*pdcm->pf_dicm_write)(pdcm->wp, (unsigned char*)(vt + 1), &n);
			if (!n)
			{
				raise_user_error(("dcm_format"), ("write dicm item tag failed"));
			}
			_dec_size(size, deep, n);

			if (tag == 0xFFFEE000 || tag == 0xFFFEE00D || tag == 0xFFFEE0DD)
			{
				step = DICM_FORMAT_SIZE;
			}
			else
			{
				if (pdcm->b_Implicit && !IS_META_TAG(tag))
				{
					b_sq = is_sq_tag(tag);
					step = DICM_FORMAT_SIZE;
				}
				else
				{
					step = DICM_FORMAT_VR;
				}
			}
			break;
		case DICM_FORMAT_VR:
			memcpy((void*)ub, (void*)vr, 2);
			n = sizeof(short);
			if (pdcm->b_BigEndian)
			{
				_bytes_turn((unsigned char*)ub, n);
			}
			(*pdcm->pf_dicm_write)(pdcm->wp, ub, &n);
			if (!n)
			{
				raise_user_error(("dcm_format"), ("write dicm item vr failed"));
			}
			_dec_size(size, deep, n);

			b_sq = (vr[0] == 'S' && vr[1] == 'Q') ? 1 : 0;

			if ((vr[0] == 'O' && vr[1] == 'B') || (vr[0] == 'O' && vr[1] == 'W') || (vr[0] == 'S' && vr[1] == 'Q'))
			{
				us = 0;
				n = sizeof(short);
				(*pdcm->pf_dicm_write)(pdcm->wp, (unsigned char*)&us, &n);
				if (!n)
				{
					raise_user_error(("dcm_format"), ("write dicm item length failed"));
				}
				_dec_size(size, deep, n);

				step = DICM_FORMAT_SIZE;
			}
			else /*VR with short size*/
			{
				us = (unsigned short)vl;
				n = sizeof(short);
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)&us, n);
				}
				(*pdcm->pf_dicm_write)(pdcm->wp, (unsigned char*)&us, &n);
				if (!n)
				{
					raise_user_error(("dcm_format"), ("write dicm item length failed"));
				}
				_dec_size(size, deep, n);

				size[++deep] = 0xFFFFFFFF;

				step = DICM_FORMAT_DATA;
			}
			break;
		case DICM_FORMAT_SIZE:
			ul = vl;
			n = sizeof(long);
			if (pdcm->b_BigEndian)
			{
				_bytes_turn((unsigned char*)&ul, n);
			}
			(*pdcm->pf_dicm_write)(pdcm->wp, (unsigned char*)&ul, &n);
			if (!n)
			{
				raise_user_error(("dcm_format"), ("write dicm item length failed"));
			}
			_dec_size(size, deep, n);

			if (tag != 0xFFFEE00D && tag != 0xFFFEE0DD)
			{
				if ((vr[0] == 'S' && vr[1] == 'Q') || tag == 0xFFFEE000)
					size[++deep] = vl;
				else
					size[++deep] = 0xFFFFFFFF;
			}

			if (tag == 0xFFFEE000)
			{
				step = DICM_FORMAT_SQ_ITEM_BEGIN;
				break;
			}
			else if (tag == 0xFFFEE00D)
			{
				step = DICM_FORMAT_SQ_ITEM_END;
				break;
			}
			else if (tag == 0xFFFEE0DD)
			{
				step = DICM_FORMAT_SQ_END;
				break;
			}
			else
			{
				if (b_sq)
					step = DICM_FORMAT_SQ_BEGIN;
				else
					step = DICM_FORMAT_DATA;
			}
			break;
		case DICM_FORMAT_DATA:
			if (vl > MAX_LONG)
			{
				raise_user_error(("dcm_parse"), ("parse dicm breaked"));
			}

			vf = (unsigned char*)xmem_alloc(vl + 2);

			if (pdcm->pf_dicm_format_data)
				rt = (*pdcm->pf_dicm_format_data)(pdcm->fp, &tag, vr, vf, &vl);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				raise_user_error(("dcm_format"), ("format dicm item breaked"));
			}

			if (vr[0] == 'O' && vr[1] == 'W')
			{
				if (pdcm->b_BigEndian)
				{
					for (i = 0; i < (int)(vl / 2); i += 2)
					{
						_bytes_turn(vf + i, 2);
					}
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 4);
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'D') //8
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 8);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 2);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 4);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 2);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					_bytes_turn((unsigned char*)vf, 4);
				}
			}

			n = vl;
			(*pdcm->pf_dicm_write)(pdcm->wp, vf, &n);
			if (n < vl)
			{
				raise_user_error(("dcm_format"), ("write dicm data failed"));
			}
			_dec_size(size, deep, n);

			xmem_free(vf);
			vf = NULL;

			deep--;

			if (!size[deep])
				step = DICM_FORMAT_SQ_ITEM_END;
			else
				step = DICM_FORMAT_TAG;
			break;
		case DICM_FORMAT_SQ_BEGIN:
			if (pdcm->pf_dicm_format_sequence_begin)
				rt = (*pdcm->pf_dicm_format_sequence_begin)(pdcm->fp, &tag);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				raise_user_error(("dcm_parse"), ("format dicm sequence breaked"));
			}
			step = DICM_FORMAT_TAG;
			break;
		case DICM_FORMAT_SQ_ITEM_BEGIN:
			if (pdcm->pf_dicm_format_sequence_item_begin)
				rt = (*pdcm->pf_dicm_format_sequence_item_begin)(pdcm->fp, &tag);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				raise_user_error(("dcm_parse"), ("format dicm sequence breaked"));
			}
			step = DICM_FORMAT_TAG;
			break;
		case DICM_FORMAT_SQ_ITEM_END:
			if (pdcm->pf_dicm_format_sequence_item_end)
				rt = (*pdcm->pf_dicm_format_sequence_item_end)(pdcm->fp, &tag);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				raise_user_error(("dcm_parse"), ("format dicm sequence breaked"));
			}

			deep--;

			if (size[deep])
				step = DICM_FORMAT_TAG;
			else
				step = DICM_FORMAT_SQ_END;
			break;
		case DICM_FORMAT_SQ_END:
			if (pdcm->pf_dicm_format_sequence_end)
				rt = (*pdcm->pf_dicm_format_sequence_end)(pdcm->fp, &tag);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				raise_user_error(("dcm_parse"), ("format dicm sequence breaked"));
			}

			deep--;

			step = DICM_FORMAT_TAG;
			break;
		}
	}

	END_CATCH;

	return 1;

ONERROR:

	if (vf)
		xmem_free(vf);

	if (pdcm->pf_dicm_error)
	{
		get_last_error(errcode, errtext, ERR_LEN);
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return 0;
}

int dicm_format(dicm_context_t* pdcm)
{
	if (!_dicm_format_head(pdcm))
		return 0;

	return _dicm_format_body(pdcm);
}

int dicm_parse_point(const tchar_t* token, int len, xpoint_t* ppt, int max)
{
	int i,n;

	if (len < 0)
		len = xslen(token);

	if (is_null(token) || !len)
		return 0;

	i = 0;
	while (*token && len && i < max)
	{
		n = 0;
		while (*token != _T('/') && *token != _T('\\') && *token != _T('\0') && n < len)
		{
			token++;
			n++;
		}
		if (ppt)
		{
			ppt[i].x = xsntol(token - n, n);
		}

		if (*token == _T('/') || *token == _T('\\'))
		{
			token++;
			n++;
		}
		len -= n;

		n = 0;
		while (*token != _T('/') && *token != _T('\\') && *token != _T('\0') && n < len)
		{
			token++;
			n++;
		}
		if (ppt)
		{
			ppt[i].y = xsntol(token - n, n);
		}

		if (*token == _T('/') || *token == _T('\\'))
		{
			token++;
			n++;
		}
		len -= n;

		i++;
	}

	return i;
}

int dicm_format_point(const xpoint_t* ppt, int count, tchar_t* buf, int max)
{
	int i, n, total = 0;;

	for (i = 0; i < count; i++)
	{
		n = xsprintf(((buf)? (buf + total) : NULL), ("%d/%d/"), ppt[i].x, ppt[i].y);

		if (total + n > max)
			break;

		total += n;
	}

	if (total)
	{
		buf[total - 1] = _T('\0'); //last /
		total--;
	}

	return total;
}