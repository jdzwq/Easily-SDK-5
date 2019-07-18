/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmctx.c | dicom implement file

	@devnote 张文权 2018.01 - 2018.12	v1.0
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

#include "dicmtag.h"

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
	{ 0x00000000, _T("UL"), _T("GroupLength") },
	{ 0x00000002, _T("UI"), _T("AffectedSOPClassUID") },
	{ 0x00000100, _T("US"), _T("CommandField") },
	{ 0x00000110, _T("US"), _T("MessageID") },
	{ 0x00000120, _T("US"), _T("MessageIDBeingRespondedTo") },
	{ 0x00000600, _T("AE"), _T("MoveDestination") },
	{ 0x00000700, _T("US"), _T("Priority") },
	{ 0x00000800, _T("US"), _T("DataSetType") },
	{ 0x00000900, _T("US"), _T("Status") },
	{ 0x00001020, _T("US"), _T("NumberofRemainingSubOperations") },
	{ 0x00001021, _T("US"), _T("NumberofCompletedSubOperations") },
	{ 0x00001022, _T("US"), _T("NumberofFailedSubOperations") },
	{ 0x00001023, _T("US"), _T("NumberofWarningSubOperations") },

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

const dword_t DICM_SQTAGS[] = {
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

//Secondary Capture Image Storage
//SOPClassUID 1.2.840.10008.5.1.4.1.1.7
dword_t DICM_CAPTAGS[DICM_CAPTAGS_SIZE] = {
	0x00020000, //FileMetaInfoGroupLength VX = "0x00020000" VR = "UL"
	0x00020001, //FileMetaInfoVersion VX = "0x00020001" VR = "OB"
	0x00020002, //MediaStorageSOPClassUID VX = "0x00020002" VR = "UI"
	0x00020003, //MediaStorageSOPInstanceUID VX = "0x00020003" VR = "UI"
	0x00020010, //TransferSyntaxUID VX = "0x00020010" VR = "UI"
	0x00020012, //ImplementationClassUID VX = "0x00020012" VR = "UI"
	0x00020013, //ImplementationVersionName VX = "0x00020013" VR = "SH"
	0x00020016, //SourceApplicationEntityTitle VX = "0x00020016" VR = "AE"

	0x00080016, //SOPClassUID VX = "0x00080016" VR = "UI"
	0x00080018, //SOPInstanceUID VX = "0x00080018" VR = "UI"
	0x00080020, //StudyDate VX = "0x00080020" VR = "DA",
	0x00080021, //SeriesDate VX = "0x00080021" VR = "DA"
	0x00080023, //ContentDate VX = "0x00080023" VR = "DA"
	0x00080030, //StudyTime VX = "0x00080030" VR = "TM"
	0x00080031, //SeriesTime VX = "0x00080031" VR = "TM"
	0x00080033, //ContentTime VX = "0x00080033" VR = "TM"
	0x00080050, //AccessionNumber VX = "0x00080050" VR = "SH"
	0x00080060, //Modality VX = "0x00080060" VR = "CS"

	0x00100010, //PatientName VX = "0x00100010" VR = "PN"
	0x00100020, //PatientID VX = "0x00100020" VR = "LO"
	0x00100030, //PatientBirthDate VX = "0x00100030" VR = "DA"
	0x00100040, //PatientSex VX = "0x00100040" VR = "CS"
	0x00101010, //PatientAge VX = "0x00101010" VR = "AS"

	0x0020000D, //StudyInstanceUID VX = "0x0020000D" VR = "UI"
	0x0020000E, //SeriesInstanceUID VX = "0x0020000E" VR = "UI"
	0x00200011, //SeriesNumber VX = "0x00200011" VR = "IS"
	0x00200013, //InstanceNumber VX = "0x00200013" VR = "IS"

	0x00280002, //SamplesPerPixel VX = "0x00280002" VR = "US"
	0x00280004, //PhotometricInterpretation VX = "0x00280004" VR = "CS"
	0x00280006, //PlanarConfiguration VX = "0x00280006" VR = "US"
	0x00280010, //Rows VX = "0x00280010" VR = "US"
	0x00280011, //Columns VX = "0x00280011" VR = "US"
	0x00280100, //BitsAllocated VX = "0x00280100" VR = "US"
	0x00280101, //BitsStored VX = "0x00280101" VR = "US"
	0x00280102, //HighBit VX = "0x00280102" VR = "US"
	0x00280103, //PixelRepresentation VX = "0x00280103" VR = "US"

	0x7FE00010 //PixelData VX = "0x7FE00010" VR = "OW"
};

/************************************************************************************************/

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

/*******************************************************************************************/

bool_t dicm_is_sequence(dword_t tag)
{
	int n = sizeof(DICM_SQTAGS) / sizeof(dword_t);

	for (int i = 0; i < n; i++)
	{
		if (tag == DICM_SQTAGS[i])
			return 1;
	}

	return 0;
}

dword_t dicm_vr_size(const tchar_t* pvr)
{
	int i, n;

	n = sizeof(DICM_VR) / sizeof(dicm_vr_t);
	for (i = 0; i < n; i++)
	{
		if (pvr[0] == DICM_VR[i].vr[0] && pvr[1] == DICM_VR[i].vr[1])
			return DICM_VR[i].vl;
	}
	return 0;
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

bool_t sop_is_implicit(const byte_t* syn)
{
	if (a_xsncmp(syn, "1.2.840.10008.1.2.1", a_xslen("1.2.840.10008.1.2.1")) == 0)
	{
		return 0;
	}
	else if (a_xsncmp(syn, "1.2.840.10008.1.2.2", a_xslen("1.2.840.10008.1.2.2")) == 0)
	{
		return 0;
	}
	else if (a_xsncmp(syn, "1.2.840.10008.1.2", a_xslen("1.2.840.10008.1.2")) == 0)
	{
		return 1;
	}

	return 0;
}

bool_t sop_is_bigendian(const byte_t* syn)
{
	if (a_xsncmp(syn, "1.2.840.10008.1.2.2", a_xslen("1.2.840.10008.1.2.2")) == 0)
	{
		return 1;
	}
	else if (a_xsncmp(syn, "1.2.840.10008.1.2.1", a_xslen("1.2.840.10008.1.2.1")) == 0)
	{
		return 0;
	}
	else if (a_xsncmp(syn, "1.2.840.10008.1.2", a_xslen("1.2.840.10008.1.2")) == 0)
	{
		return 0;
	}

	return 0;
}