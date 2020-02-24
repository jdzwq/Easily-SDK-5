/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dcm.c | dicom implement file

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
	{ _T("AE"), 16 },
	{ _T("AS"), 4 },
	{ _T("AT"), 4 },
	{ _T("CS"), 16 },
	{ _T("DA"), 8 },
	{ _T("DS"), 16 },
	{ _T("DT"), 26 },
	{ _T("FL"), 4 },
	{ _T("FD"), 8 },
	{ _T("IS"), 12 },
	{ _T("LO"), 64 },
	{ _T("LT"), 10240 },
	{ _T("OB"), 0 },
	{ _T("OW"), 0 },
	{ _T("PN"), 64 },
	{ _T("SH"), 16 },
	{ _T("SL"), 4 },
	{ _T("SQ"), 0 },
	{ _T("SS"), 2 },
	{ _T("ST"), 1024 },
	{ _T("TM"), 16 },
	{ _T("UI"), 64 },
	{ _T("UL"), 4 },
	{ _T("US"), 2 }
};


const dicm_item_t DICM_ITEMS[] = {
	{ 0x00020000, _T("UL"), _T("FileMetaInfoGroupLength") },
	{ 0x00020001, _T("OB"), _T("FileMetaInfoVersion") },
	{ 0x00020002, _T("UI"), _T("MediaStorageSOPClassUID") },
	{ 0x00020003, _T("UI"), _T("MediaStorageSOPInstanceUID") },
	{ 0x00020010, _T("UI"), _T("TransferSyntaxUID") },
	{ 0x00020012, _T("UI"), _T("ImplementationClassUID") },
	{ 0x00020013, _T("SH"), _T("ImplementationVersionName") },
	{ 0x00020016, _T("AE"), _T("SourceApplicationEntityTitle") },
	{ 0x00020100, _T(""), _T("PrivateInformationCreatorUID") },
	{ 0x00020102, _T(""), _T("PrivateInformation") },

	{ 0x00041130, _T(""), _T("FileSetID") },
	{ 0x00041141, _T(""), _T("FileSetDescriptorFileID") },
	{ 0x00041142, _T(""), _T("SpecificCharacterSetOfFile") },
	{ 0x00041200, _T(""), _T("FirstDirectoryRecordOffset") },
	{ 0x00041202, _T(""), _T("LastDirectoryRecordOffset") },
	{ 0x00041212, _T(""), _T("FileSetConsistencyFlag") },
	{ 0x00041220, _T(""), _T("DirectoryRecordSequence") },
	{ 0x00041400, _T(""), _T("OffsetOfNextDirectoryRecord") },
	{ 0x00041410, _T(""), _T("RecordInUseFlag") },
	{ 0x00041420, _T(""), _T("LowerLevelDirectoryEntityOffset") },
	{ 0x00041430, _T(""), _T("DirectoryRecordType") },
	{ 0x00041432, _T(""), _T("PrivateRecordUID") },
	{ 0x00041500, _T(""), _T("ReferencedFileID") },
	{ 0x00041504, _T(""), _T("MRDRDirectoryRecordOffset") },
	{ 0x00041510, _T(""), _T("ReferencedSOPClassUIDInFile") },
	{ 0x00041511, _T(""), _T("ReferencedSOPInstanceUIDInFile") },
	{ 0x00041512, _T(""), _T("ReferencedTransferSyntaxUIDInFile") },
	{ 0x0004151A, _T(""), _T("ReferencedRelatedSOPClassUIDInFile") },
	{ 0x00041600, _T(""), _T("NumberOfReferences") },

	{ 0x00080000, _T(""), _T("IdentifyingGroupLength") },
	{ 0x00080001, _T(""), _T("LengthToEnd") },
	{ 0x00080005, _T(""), _T("SpecificCharacterSet") },
	{ 0x00080006, _T(""), _T("LanguageCodeSequence") },
	{ 0x00080008, _T(""), _T("ImageType") },
	{ 0x00080010, _T(""), _T("RecognitionCode") },
	{ 0x00080012, _T(""), _T("InstanceCreationDate") },
	{ 0x00080013, _T(""), _T("InstanceCreationTime") },
	{ 0x00080014, _T(""), _T("InstanceCreatorUID") },
	{ 0x00080016, _T("UI"), _T("SOPClassUID") },
	{ 0x00080018, _T("UI"), _T("SOPInstanceUID") },
	{ 0x0008001A, _T(""), _T("RelatedGeneralSOPClassUID") },
	{ 0x0008001B, _T(""), _T("OriginalSpecializedSOPClassUID") },
	{ 0x00080020, _T("DA"), _T("StudyDate") },
	{ 0x00080021, _T("DA"), _T("SeriesDate") },
	{ 0x00080022, _T(""), _T("AcquisitionDate") },
	{ 0x00080023, _T("DA"), _T("ContentDate") },
	{ 0x00080024, _T(""), _T("OverlayDate") },
	{ 0x00080025, _T(""), _T("CurveDate") },
	{ 0x0008002A, _T(""), _T("AcquisitionDateTime") },
	{ 0x00080030, _T("TM"), _T("StudyTime") },
	{ 0x00080031, _T("TM"), _T("SeriesTime") },
	{ 0x00080032, _T(""), _T("AcquisitionTime") },
	{ 0x00080033, _T("TM"), _T("ContentTime") },
	{ 0x00080034, _T(""), _T("OverlayTime") },
	{ 0x00080035, _T(""), _T("CurveTime") },
	{ 0x00080040, _T(""), _T("DataSetType") },
	{ 0x00080041, _T(""), _T("DataSetSubtype") },
	{ 0x00080042, _T(""), _T("NuclearMedicineSeriesType") },
	{ 0x00080050, _T("SH"), _T("AccessionNumber") },
	{ 0x00080052, _T(""), _T("QueryRetrieveLevel") },
	{ 0x00080054, _T(""), _T("RetrieveAETitle") },
	{ 0x00080056, _T(""), _T("InstanceAvailability") },
	{ 0x00080058, _T(""), _T("FailedSOPInstanceUIDList") },
	{ 0x00080060, _T("CS"), _T("Modality") },
	{ 0x00080061, _T(""), _T("ModalitiesInStudy") },
	{ 0x00080062, _T(""), _T("SOPClassesInStudy") },
	{ 0x00080064, _T(""), _T("ConversionType") },
	{ 0x00080068, _T(""), _T("PresentationIntentType") },
	{ 0x00080070, _T(""), _T("Manufacturer") },
	{ 0x00080080, _T(""), _T("InstitutionName") },
	{ 0x00080081, _T(""), _T("InstitutionAddress") },
	{ 0x00080082, _T(""), _T("InstitutionCodeSequence") },
	{ 0x00080090, _T(""), _T("ReferringPhysicianName") },
	{ 0x00080092, _T(""), _T("ReferringPhysicianAddress") },
	{ 0x00080094, _T(""), _T("ReferringPhysicianTelephoneNumber") },
	{ 0x00080096, _T(""), _T("ReferringPhysicianIDSequence") },
	{ 0x00080100, _T("US"), _T("CodeValue") },
	{ 0x00080102, _T(""), _T("CodingSchemeDesignator") },
	{ 0x00080103, _T(""), _T("CodingSchemeVersion") },
	{ 0x00080104, _T(""), _T("CodeMeaning") },
	{ 0x00080105, _T(""), _T("MappingResource") },
	{ 0x00080106, _T(""), _T("ContextGroupVersion") },
	{ 0x00080107, _T(""), _T("ContextGroupLocalVersion") },
	{ 0x0008010B, _T(""), _T("ContextGroupExtensionFlag") },
	{ 0x0008010C, _T(""), _T("CodingSchemeUID") },
	{ 0x0008010D, _T(""), _T("ContextGroupExtensionCreatorUID") },
	{ 0x0008010F, _T(""), _T("ContextIdentifier") },
	{ 0x00080110, _T(""), _T("CodingSchemeIDSequence") },
	{ 0x00080112, _T(""), _T("CodingSchemeRegistry") },
	{ 0x00080114, _T(""), _T("CodingSchemeExternalID") },
	{ 0x00080115, _T(""), _T("CodingSchemeName") },
	{ 0x00080116, _T(""), _T("ResponsibleOrganization") },
	{ 0x00080117, _T(""), _T("ContextUID") },
	{ 0x00080201, _T(""), _T("TimezoneOffsetFromUTC") },
	{ 0x00081000, _T(""), _T("NetworkID") },
	{ 0x00081010, _T(""), _T("StationName") },
	{ 0x00081090, _T(""), _T("ManufacturersModelName") },

	{ 0x00100010, _T("PN"), _T("PatientName") },
	{ 0x00100020, _T("LO"), _T("PatientID") },
	{ 0x00100021, _T(""), _T("IssuerOfPatientID") },
	{ 0x00100022, _T(""), _T("TypeOfPatientID") },
	{ 0x00100030, _T("DA"), _T("PatientBirthDate") },
	{ 0x00100032, _T(""), _T("PatientBirthTime") },
	{ 0x00100040, _T("CS"), _T("PatientSex") },
	{ 0x00101010, _T("AS"), _T("PatientAge") },
	{ 0x00101020, _T(""), _T("PatientSize") },
	{ 0x00101030, _T(""), _T("PatientWeight") },
	{ 0x00101040, _T(""), _T("PatientAddress") },
	{ 0x001021C0, _T(""), _T("PregnancyStatus") },

	{ 0x00180030, _T(""), _T("Radionuclide") },
	{ 0x00180040, _T(""), _T("CineRate") },
	{ 0x00180050, _T(""), _T("SliceThickness") },
	{ 0x00180060, _T(""), _T("KVP") },
	{ 0x00181100, _T(""), _T("ReconstructionDiameter") },
	{ 0x00181110, _T(""), _T("DistanceSourceToDetector") },
	{ 0x00181111, _T(""), _T("DistanceSourceToPatient") },
	{ 0x00181114, _T(""), _T("EstimatedRadiographicMagnification") },
	{ 0x00181120, _T(""), _T("GantryDetectorTilt") },
	{ 0x00181121, _T(""), _T("GantryDetectorSlew") },
	{ 0x00181130, _T(""), _T("TableHeight") },
	{ 0x00181131, _T(""), _T("TableTraverse") },
	{ 0x00181134, _T(""), _T("TableMotion") },
	{ 0x00181150, _T(""), _T("ExposureTime") },
	{ 0x00181151, _T(""), _T("XRayTubeCurrent") },

	{ 0x0020000D, _T("UI"), _T("StudyInstanceUID") },
	{ 0x0020000E, _T("UI"), _T("SeriesInstanceUID") },
	{ 0x00200010, _T(""), _T("StudyID") },
	{ 0x00200011, _T("IS"), _T("SeriesNumber") },
	{ 0x00200012, _T(""), _T("AcquisitionNumber") },
	{ 0x00200013, _T("IS"), _T("InstanceNumber") },
	{ 0x00200014, _T(""), _T("IsotopeNumber") },
	{ 0x00200015, _T(""), _T("PhaseNumber") },
	{ 0x00200016, _T(""), _T("IntervalNumber") },
	{ 0x00200017, _T(""), _T("TimeSlotNumber") },
	{ 0x00200018, _T(""), _T("AngleNumber") },
	{ 0x00200019, _T(""), _T("ItemNumber") },
	{ 0x00200020, _T(""), _T("PatientOrientation") },
	{ 0x00200022, _T(""), _T("OverlayNumber") },
	{ 0x00200024, _T(""), _T("CurveNumber") },
	{ 0x00200026, _T(""), _T("LookupTableNumber") },
	{ 0x00200030, _T(""), _T("ImagePosition") },
	{ 0x00200032, _T(""), _T("ImagePositionPatient")},
	{ 0x00200035, _T(""), _T("ImageOrientation") },
	{ 0x00200037, _T(""), _T("ImageOrientationPatient") },
	{ 0x00201000, _T(""), _T("SeriesInStudy") },
	{ 0x00201001, _T(""), _T("AcquisitionsInSeries") },
	{ 0x00201002, _T(""), _T("ImagesInAcquisition") },
	{ 0x00201003, _T(""), _T("ImagesInSeries") },
	{ 0x00201004, _T(""), _T("AcquisitionsInStudy") },
	{ 0x00201005, _T(""), _T("ImagesInStudy") },

	{ 0x00280002, _T("US"), _T("SamplesPerPixel") },
	{ 0x00280003, _T(""), _T("SamplesPerPixelUsed") },
	{ 0x00280004, _T("CS"), _T("PhotometricInterpretation") },
	{ 0x00280005, _T(""), _T("ImageDimensions") },
	{ 0x00280006, _T("US"), _T("PlanarConfiguration") },
	{ 0x00280008, _T(""), _T("NumberOfFrames") },
	{ 0x00280009, _T(""), _T("FrameIncrementPointer") },
	{ 0x0028000A, _T(""), _T("FrameDimensionPointer") },
	{ 0x00280010, _T("US"), _T("Rows") },
	{ 0x00280011, _T("US"), _T("Columns") },
	{ 0x00280012, _T(""), _T("Planes") },
	{ 0x00280014, _T(""), _T("UltrasoundColorDataPresent") },
	{ 0x00280030, _T(""), _T("PixelSpacing") },
	{ 0x00280031, _T(""), _T("ZoomFactor") },
	{ 0x00280032, _T(""), _T("ZoomCenter") },
	{ 0x00280034, _T(""), _T("PixelAspectRatio") },

	{ 0x00280100, _T("US"), _T("BitsAllocated") },
	{ 0x00280101, _T("US"), _T("BitsStored") },
	{ 0x00280102, _T("US"), _T("HighBit") },
	{ 0x00280103, _T("US"), _T("PixelRepresentation") },
	{ 0x00281050, _T(""), _T("WindowCenter") },
	{ 0x00281051, _T(""), _T("WindowWidth") },
	{ 0x00281052, _T(""), _T("RescaleIntercept") },
	{ 0x00281053, _T(""), _T("RescaleSlope") },
	{ 0x00281054, _T(""), _T("RescaleType") },

	{ 0x00700001, _T(""), _T("GraphicAnnotationSequence") },
	{ 0x00700002, _T(""), _T("GraphicLayer") },
	{ 0x00700003, _T(""), _T("BoundingBoxAnnotationUnits") },
	{ 0x00700004, _T(""), _T("AnchorPointAnnotationUnits") },
	{ 0x00700005, _T(""), _T("GraphicAnnotationUnits") },
	{ 0x00700006, _T(""), _T("UnformattedTextValue") },
	{ 0x00700008, _T(""), _T("TextObjectSequence") },
	{ 0x00700009, _T(""), _T("GraphicObjectSequence") },
	{ 0x00700010, _T(""), _T("BoundingBoxTopLeftHandCorner") },
	{ 0x00700011, _T(""), _T("BoundingBoxBottomRightHandCorner") },
	{ 0x00700012, _T(""), _T("BoundingBoxTextHorizJustification") },
	{ 0x00700014, _T(""), _T("AnchorPoint") },
	{ 0x00700015, _T(""), _T("AnchorPointVisibility") },
	{ 0x00700020, _T(""), _T("GraphicDimensions") },
	{ 0x00700021, _T(""), _T("NumberOfGraphicPoints") },
	{ 0x00700022, _T(""), _T("GraphicData") },
	{ 0x00700023, _T(""), _T("GraphicType") },
	{ 0x00700024, _T(""), _T("GraphicFilled") },

	{ 0x7FE00010, _T("OW"), _T("PixelData") },

	{ 0xFFFEE000, _T(""), _T("StartOfItem") },
	{ 0xFFFEE00D, _T(""), _T("EndOfItems") },
	{ 0xFFFEE0DD, _T(""), _T("EndOfSequence") },
};

const dicm_group_t DICM_GROUPS[] = {
	{ 0x0002, _T("FileMeta") },
	{ 0x0004, _T("FileSet") },
	{ 0x0008, _T("Identifying") },
	{ 0x0010, _T("Patient") },
	{ 0x0018, _T("Acquisition") },
	{ 0x0020, _T("Relationship") },
	{ 0x0028, _T("Presentation") },
	{ 0x0032, _T("Study") },
	{ 0x0070, _T("Annotation") },
	{ 0x7FE0, _T("PixelData") },
};

const dword_t SQTAGS[] = {
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



static bool_t is_sq_tag(dword_t tag)
{
	int n = sizeof(SQTAGS) / sizeof(dword_t);

	for (int i = 0; i < n; i++)
	{
		if (tag == SQTAGS[i])
			return 1;
	}

	return 0;
}

static int _find_item_index(dword_t tag, int l, int h)
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

static int find_item_index(dword_t tag)
{
	int n;

	n = sizeof(DICM_ITEMS) / sizeof(dicm_item_t);

	return _find_item_index(tag, 0, n - 1);
}

const dicm_item_t* dicm_item_info(dword_t tag)
{
	int i;

	i = find_item_index(tag);

	if (i < 0)
		return NULL;
	else
		return &(DICM_ITEMS[i]);
}

const dicm_group_t* dicm_group_info(dword_t tag)
{
	sword_t sw;
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

	ctx = (dicm_context_t*)xmem_alloc(sizeof(dicm_context_t));

	ctx->b_Implicit = 0;
	ctx->b_BigEndian = 0;
	ctx->n_CharSet = 0;

	return ctx;
}

void dicm_free(dicm_context_t* ctx)
{
	xmem_free(ctx);
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

bool_t _dicm_parse_head(dicm_context_t* pdcm)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	unsigned char head[128] = { 0 };
	unsigned char dicm[5] = { 0 };

	dword_t n;

	TRY_CATCH;

	//skip file head
	n = 128;
	(*pdcm->pf_dicm_read)(pdcm->rp, head, &n);
	if (n < 128)
	{
		raise_user_error(_T("dcm_parse"), _T("not dicm file"));
	}

	//dicm head
	n = 4;
	(*pdcm->pf_dicm_read)(pdcm->rp, dicm, &n);
	if (n < 4)
	{
		raise_user_error(_T("dcm_parse"), _T("not dicm file"));
	}

	if (a_xsnicmp((schar_t*)dicm, "DICM", 4) != 0)
	{
		raise_user_error(_T("dcm_parse"), _T("read DICM flag falied"));
	}

	END_CATCH;

	return 1;

ONERROR:

	if (pdcm->pf_dicm_error)
	{
		get_last_error(errcode, errtext, ERR_LEN);
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return 0;
}

static void _dec_size(dword_t* p, int i, dword_t d)
{
	do{
		if (p[i] != 0xFFFFFFFF)
			p[i] -= d;
	} while (i--);
}

bool_t _dicm_parse_body(dicm_context_t* pdcm)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	unsigned short vt[2] = { 0 };
	unsigned char vr[2] = { 0 };
	unsigned long vl = 0;
	unsigned char* vf = NULL;

	unsigned long ul = 0;
	unsigned short us = 0;
	unsigned char rev[4] = { 0 };

	dword_t n;
	dword_t tag;
	bool_t b_sq = 0;
	int rt, i, step;
	
	dword_t size[256] = { 0 };
	int deep = 0;

	TRY_CATCH;

	size[deep] = 0xFFFFFFFF;
	step = DICM_PARSE_TAG;

	while (step != DICM_PARSE_END)
	{
		switch (step)
		{
		case DICM_PARSE_TAG:
			//element tag
			n = sizeof(short);
			if(!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)vt, &n))
			{
				raise_user_error(_T("dcm_parse"), _T("read dicm tag failed"));
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);
			
			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)vt, n);
			}

			n = sizeof(short);
			if(!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)(vt + 1), &n))
			{
				raise_user_error(_T("dcm_parse"), _T("read dicm tag failed"));
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);
			
			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)(vt + 1), n);
			}

			tag = MAKEDWORD(vt[1], vt[0]);

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
			if(!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)vr, &n))
			{
				raise_user_error(_T("dcm_parse"), _T("read dicm vr failed"));
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);
			
			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)vr, n);
			}

			b_sq = (vr[0] == 'S' && vr[1] == 'Q') ? 1 : 0;

			if (!pdcm->b_Implicit || IS_META_TAG(tag))
			{
				if ((vr[0] == 'O' && vr[1] == 'B') || (vr[0] == 'O' && vr[1] == 'W') || (vr[0] == 'S' && vr[1] == 'Q'))
				{
					//skip 2 bytes
					n = sizeof(short);
					if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)rev, &n))
					{
						raise_user_error(_T("dcm_parse"), _T("read dicm size failed"));
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
					if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)&us, &n))
					{
						raise_user_error(_T("dcm_parse"), _T("read dicm size failed"));
					}
					if (!n)
					{
						step = DICM_PARSE_END;
						break;
					}
					_dec_size(size, deep, n);

					if (pdcm->b_BigEndian)
					{
						bytes_turn((byte_t*)&us, n);
					}

					vl = (dword_t)us;
					size[++deep] = 0xFFFFFFFF;

					if (pdcm->pf_dicm_parse_item)
						rt = (*pdcm->pf_dicm_parse_item)(pdcm->pp, tag, vr, vl);
					else
						rt = DICM_CONTINUE;

					if (rt != DICM_CONTINUE)
					{
						raise_user_error(_T("dcm_parse"), _T("parse dicm breaked"));
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
			if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)&ul, &n))
			{
				raise_user_error(_T("dcm_parse"), _T("read dicm size failed"));
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);

			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&ul, n);
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
					raise_user_error(_T("dcm_parse"), _T("parse dicm breaked"));
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
			if (vl > MAX_LONG)
			{
				raise_user_error(_T("dcm_parse"), _T("parse dicm breaked"));
			}

			vf = (unsigned char*)xmem_alloc(vl + 2);
			n = vl;
			if(!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)vf, &n))
			{
				raise_user_error(_T("dcm_parse"), _T("read dicm value failed"));
			}
			_dec_size(size, deep, n);

			if (vr[0] == 'O' && vr[1] == 'W')
			{
				if (pdcm->b_BigEndian)
				{
					for (i = 0; i < (int)(n / 2); i += 2)
					{
						bytes_turn(vf + i, 2);
					}
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'D') //8
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 8);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}

			if (tag == 0x00020010)
			{
				if (a_xscmp((schar_t*)vf, "1.2.840.10008.1.2.1") == 0)
				{
					pdcm->b_Implicit = 0;
					pdcm->b_BigEndian = 0;
				}
				else if (a_xscmp((schar_t*)vf, "1.2.840.10008.1.2.2") == 0)
				{
					pdcm->b_Implicit = 0;
					pdcm->b_BigEndian = 1;
				}
				else if (a_xscmp((schar_t*)vf, "1.2.840.10008.1.2") == 0)
				{
					pdcm->b_Implicit = 1;
					pdcm->b_BigEndian = 0;
				}
			}
			else if (tag == 0x00080005)
			{
				if (a_xsnicmp((schar_t*)vf, "ISO_IR 192", 10) == 0)
					pdcm->n_CharSet = DICM_CHARSET_UTF;
				else if (a_xsnicmp((schar_t*)vf, "GB18030", 7) == 0)
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
				raise_user_error(_T("dcm_parse"), _T("parse dicm breaked"));
			}

			xmem_free(vf);
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
				raise_user_error(_T("dcm_parse"), _T("parse dicm breaked"));
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
				raise_user_error(_T("dcm_parse"), _T("parse dicm breaked"));
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
				raise_user_error(_T("dcm_parse"), _T("parse dicm breaked"));
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
				raise_user_error(_T("dcm_parse"), _T("parse dicm breaked"));
			}

			deep--;

			step = DICM_PARSE_TAG;
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

bool_t dicm_parse(dicm_context_t* pdcm)
{
	if(!_dicm_parse_head(pdcm))
		return 0;

	return _dicm_parse_body(pdcm);
}

/*************************************************************************************************************/
bool_t _dicm_format_head(dicm_context_t* pdcm)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	unsigned char head[128] = { 0 };
	unsigned char dicm[5] = { 0 };

	dword_t n;

	TRY_CATCH;

	n = 128;
	(*pdcm->pf_dicm_write)(pdcm->wp, head, &n);
	if (n < 128)
	{
		raise_user_error(_T("dcm_format"), _T("write dicm head failed"));
	}

	a_xscpy((schar_t*)dicm, "DICM");
	n = 4;
	(*pdcm->pf_dicm_write)(pdcm->wp, dicm, &n);
	if (n < 4)
	{
		raise_user_error(_T("dcm_format"), _T("write dicm flat failed"));
	}

	END_CATCH;

	return 1;

ONERROR:

	if (pdcm->pf_dicm_error)
	{
		get_last_error(errcode, errtext, ERR_LEN);
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return 0;
}

bool_t _dicm_format_body(dicm_context_t* pdcm)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	dword_t tag;
	sword_t vt[2] = { 0 };
	byte_t vr[3] = { 0 };
	byte_t* vf = NULL;
	dword_t vl = 0;
	bool_t b_sq = 0;

	dword_t n;
	int i, rt;
	int step;

	byte_t ub[2];
	dword_t ul;
	sword_t us;

	dword_t size[256] = { 0 };
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
			(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)vt, &n);
			if (!n)
			{
				raise_user_error(_T("dcm_format"), _T("write dicm item tag failed"));
			}
			_dec_size(size, deep, n);

			n = sizeof(short);
			(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)(vt + 1), &n);
			if (!n)
			{
				raise_user_error(_T("dcm_format"), _T("write dicm item tag failed"));
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
				bytes_turn((byte_t*)ub, n);
			}
			(*pdcm->pf_dicm_write)(pdcm->wp, ub, &n);
			if (!n)
			{
				raise_user_error(_T("dcm_format"), _T("write dicm item vr failed"));
			}
			_dec_size(size, deep, n);

			b_sq = (vr[0] == 'S' && vr[1] == 'Q') ? 1 : 0;

			if ((vr[0] == 'O' && vr[1] == 'B') || (vr[0] == 'O' && vr[1] == 'W') || (vr[0] == 'S' && vr[1] == 'Q'))
			{
				us = 0;
				n = sizeof(short);
				(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&us, &n);
				if (!n)
				{
					raise_user_error(_T("dcm_format"), _T("write dicm item length failed"));
				}
				_dec_size(size, deep, n);

				step = DICM_FORMAT_SIZE;
			}
			else /*VR with short size*/
			{
				us = (sword_t)vl;
				n = sizeof(short);
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)&us, n);
				}
				(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&us, &n);
				if (!n)
				{
					raise_user_error(_T("dcm_format"), _T("write dicm item length failed"));
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
				bytes_turn((byte_t*)&ul, n);
			}
			(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&ul, &n);
			if (!n)
			{
				raise_user_error(_T("dcm_format"), _T("write dicm item length failed"));
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
				raise_user_error(_T("dcm_parse"), _T("parse dicm breaked"));
			}

			vf = (byte_t*)xmem_alloc(vl + 2);

			if (pdcm->pf_dicm_format_data)
				rt = (*pdcm->pf_dicm_format_data)(pdcm->fp, &tag, vr, vf, &vl);
			else
				rt = DICM_CONTINUE;

			if (rt != DICM_CONTINUE)
			{
				raise_user_error(_T("dcm_format"), _T("format dicm item breaked"));
			}

			if (vr[0] == 'O' && vr[1] == 'W')
			{
				if (pdcm->b_BigEndian)
				{
					for (i = 0; i < (int)(vl / 2); i += 2)
					{
						bytes_turn(vf + i, 2);
					}
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'D') //8
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 8);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}

			n = vl;
			(*pdcm->pf_dicm_write)(pdcm->wp, vf, &n);
			if (n < vl)
			{
				raise_user_error(_T("dcm_format"), _T("write dicm data failed"));
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
				raise_user_error(_T("dcm_parse"), _T("format dicm sequence breaked"));
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
				raise_user_error(_T("dcm_parse"), _T("format dicm sequence breaked"));
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
				raise_user_error(_T("dcm_parse"), _T("format dicm sequence breaked"));
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
				raise_user_error(_T("dcm_parse"), _T("format dicm sequence breaked"));
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

bool_t dicm_format(dicm_context_t* pdcm)
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
		n = xsprintf(((buf)? (buf + total) : NULL), _T("%d/%d/"), ppt[i].x, ppt[i].y);

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