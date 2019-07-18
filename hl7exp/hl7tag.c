/***********************************************************************
	Easily HL7 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc hl7 document

	@module	hl7tag.c | hl7 implement file

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

#include "hl7tag.h"

const hl7_vr_t HL7_VR[] = {
	{ _T("ST"), 0 }, //String
	{ _T("TX"), 0 }, //Text Data
	{ _T("FT"), 0 }, //Formated Text
	{ _T("CQ"), 0xFFFFFFFF }, //Composite quantity with units; <quantity (NM)>^<units (CE)>
	{ _T("MO"), 0xFFFFFFFF }, //Money; <quantity (NM)>^<denomination (ID)>
	{ _T("NM"), 0 }, //Numeric
	{ _T("SI"), 0 }, //Sequence ID
	{ _T("SN"), 0 }, //Structured Numeric
	{ _T("ID"), 0 }, //Coded values for HL7 tables
	{ _T("IS"), 0 }, //Coded value for user-defined tables
	{ _T("HD"), 0xFFFFFFFF }, //Hierarchic designator; <namespace ID (IS)>^<universal ID (ST)>^<universal ID type (ID)>
	{ _T("EI"), 0xFFFFFFFF }, //Entity identifier; <entity identifier (ST)>^<namespace ID (IS)>^<universal ID (ST)>^<universal ID type(ID)>
	{ _T("RP"), 0xFFFFFFFF }, //Reference Pointer; <pointer (ST) >^< application ID (HD)>^<type of data (ID)>^<subtype (ID)>
	{ _T("PL"), 0xFFFFFFFF }, //Person location; <point of care (IS )>^<room (IS )>^<bed (IS)>^<facility (HD)>^< location status (IS )>^<person location type (IS)>^<building (IS )>^<floor (IS )>^<location description (ST)>
	{ _T("PT"), 0xFFFFFFFF }, //Processing type; <processing ID (ID)> ^ <processing mode (ID)>
	{ _T("DT"), 0 }, //Date; YYYY[MM[DD]]
	{ _T("TM"), 0 }, //Time; HH[MM[SS[.S[S[S[S]]]]]][+/- ZZZZ]
	{ _T("TS"), 0 }, //Time stamp; YYYY[MM[DD[HHMM[SS[.S[S[ S[S]]]]]]]][+/-ZZZZ] ^ <degree of precision>
	{ _T("CE"), 0xFFFFFFFF }, //Coded element; <identifier (ST)>^<text (ST)>^<name of coding system (ST)>^<alternate identifier (ST)>^<alternate text (ST)>^<name of alternate coding system (ST)>
	{ _T("CF"), 0xFFFFFFFF }, //Coded element with formatted values; <identifier (ID)>^<formatted text (FT)>^<name of coding system (ST)>^<alternate identifier (ID)>^<alternate formatted text (FT)>^<name of alternate coding system (ST)>
	{ _T("CK"), 0xFFFFFFFF }, //Composite ID with check digit; <ID number (NM)>^<check digit (NM)>^<code identifying the check digit scheme employed (ID)>^< assigning authority (HD)>
	{ _T("CN"), 0xFFFFFFFF }, //Composite ID number and ID name; <ID number (ST)>^<family name (ST)>^<given name (ST)>^<middle initial or name (ST)>^<suffix (e.g., JR or III) (ST)>^<prefix (e.g., DR) (ST)>^<degree (e.g., MD) (ST)>^<source table (IS)>^<assigning authority (HD)>
	{ _T("CX"), 0xFFFFFFFF }, //Extended composite ID with check digit; <ID (ST)>^<check digit (ST)>^<code identifying the check digit scheme employed (ID)>^< assigning authority (HD) )>^<identifier type code (IS)>^< assigning facility (HD)>
	{ _T("XCN"), 0xFFFFFFFF }, //Extended composite ID number and name; <ID number (ST)>^<family name (ST)>^<given name (ST)>^<middle initial or name (ST)>^<suffix (e.g., JR or III) (ST)>^<prefix (e.g., DR) (ST)>^<degree (e.g., MD) (ST)>^<source table (IS)>^<assigning authority (HD)>^<name type code (ID)>^<identifier check digit (ST)>^<code identifying the check digit scheme employed (ID)>^<identifier type code (IS)>^<assigning facility (HD)>
	{ _T("CM"), 0xFFFFFFFF }, //Composite; 
	{ _T("AD"), 0xFFFFFFFF }, //Address; <street address (ST)>^< other designation (ST)>^<city (ST)>^<state or province (ST)>^<zip or postal code (ST)>^<country (ID)>^<address type (ID)>^<other geographic designation(ST)>
	{ _T("PN"), 0xFFFFFFFF }, //Person name; <family name (ST)>^<given name (ST)>^<middle initial or name (ST)>^<suffix (e.g., JR or III) (ST)>^<prefix (e.g., DR) (ST)>^<degree (e.g., MD) (ST)>
	{ _T("TN"), 0 }, //Telephone Number; [NN] [(999)]999- 9999[X99999][B99999][C any text]
	{ _T("XAD"), 0xFFFFFFFF }, //Extended Address; <street address (ST)>^<other designation (ST)>^<city (ST)>^<state or province (ST)>^<zip or postal code (ST)>^<country (ID)>^< address type (ID)>^<other geographic designation (ST)>^<county/parish code (IS)>^<census tract (IS)>
	{ _T("XPN"), 0xFFFFFFFF }, //Extended person Name; <family name (ST)>^<given name (ST)>^<middle initial or name (ST)>^<suffix (e.g., JR or III) (ST)>^<prefix (e.g., DR) (ST)>^<degree (e.g., MD) (ST)>^<name type code(ID) >
	{ _T("XON"), 0xFFFFFFFF }, //Extended composite name and ID number for organizations; <organization name (ST)>^<organization name type code (IS)>^<ID number (NM)>^<check digit (NM)>^<code identifying the check digit scheme employed (ID)>^<assigning authority (HD)>^<identifier type code (IS)>^<assigning facility ID (HD)>
	{ _T("XTN"), 0xFFFFFFFF }, //Extended Telecommunicati ons number; [NNN] [(999)]999- 9999 [X99999] [B99999] [C any text]^<telecommunicationuse code(ID)>^<telecommunication equipment type(ID)>^<email address(ST)>^<country code(NM)>^<area / city code(NM)>^<phone number(NM)>^<extension(NM)>^<any text(ST)>
	{ _T("CD"), 0xFFFFFFFF }, //Channel Definition; <channel identifier (*)>^<channel number (NM)> & <channel name (ST)>>^<electrode names (*) >^<channel sensitivity/units (*) >^<calibration parameters (*)>^<sampling frequency (NM)>^<minimum/maximum data values (*)>
	{ _T("MA"), 0xFFFFFFFF }, //Multiplexed array; <sample 1 from channel 1 (NM)> ^ <sample 1 from channel 2 (NM)> ^ <sample 1 from channel 3 (NM)> ...~<sample 2 from channel 1 (NM)> ^ <sample 2 from channel 2 (NM)> ^ <sample 2 from channel 3 (NM)> ...~<信道1采样1(NM)>^<信道2采样1(NM)>^ <信道3采样1(NM)>^ ...~<信道1采样2(NM)>^<信道2采样2(NM)>^ <信道3采样2(NM)> ...~")
	{ _T("NA"), 0xFFFFFFFF }, //Numeric array; <value1 (NM)> ^ <value2 (NM)> ^ <value3 (NM)> ^ <value4 (NM)> ^ ...
	{ _T("ED"), 0xFFFFFFFF }, //Encapsulated data; <source application (HD) > ^ <main type of data (ID)> ^ <data subtype (ID)> ^ <encoding (ID)> ^ <data (ST)>
	{ _T("CP"), 0xFFFFFFFF }, //Composite price;  <price (MO)> ^ <price type (ID)> ^ <from value (NM)> ^ <to value (NM)> ^ <range units (CE)> ^ <range type (ID)>
	{ _T("FC"), 0xFFFFFFFF }, //Financial class; <financial class (ID)> ^ <effective date (TS)>
	{ _T("QSC"), 0xFFFFFFFF }, //Query selection Criteria; <name of field (ST)> ^ <relational operator (ID)> ^ <value (ST)> ^ <relational conjunction (ID)>
	{ _T("QIP"), 0xFFFFFFFF }, //Query input parameter list; <field name (ST) > ^ <value1 (ST) & (ST) & value2 (ST) & value3 (ST) ...>
	{ _T("RCD"), 0xFFFFFFFF }, //Row column definition; <HL7 item number (ST)> ^ <HL7 data type (ST)> ^ <maximum column width (NM)>
	{ _T("DLN"), 0xFFFFFFFF }, //Driver’s license Number; <license number (ST)> ^ <issuing state, province, country (IS)> ^ <expiration date (DT)
	{ _T("JCC"), 0xFFFFFFFF }, //Job code/class; <job code (IS)> ^ <job class (IS)>
	{ _T("VH"), 0xFFFFFFFF }, //Visiting hours; <start day range (ID)> ^ <end day range (ID)> ^ <start hour range (TM)> ^ <end hour range (TM)>
	{ _T("PPN"), 0xFFFFFFFF }, //Performing person time stamp; <ID number (ST)> ^ <family name (ST)> ^ <given name (ST)> ^ <middle initial or name (ST)> ^ <suffix (e.g., JR or III) (ST)> ^ <prefix (e.g., DR) (ST)> ^ <degree (e.g., MD) (ST)> ^ <source table (IS)> ^ <assigning authority (HD)> ^ <name type code(ID)> ^ <identifier check digit (ST)> ^ <code identifying the check digit scheme employed (ID )> ^ <identifier type code (IS)> ^ <assigning facility (HD)> ^ < date/time action performed (TS)>
	{ _T("DR"), 0xFFFFFFFF }, //Date/time range; <range start date/time (TS)> ^ <range end date/time (TS)>
	{ _T("RI"), 0xFFFFFFFF }, //Repeat interval; <repeat pattern (IS)> ^ <explicit time interval (ST)>
	{ _T("SCV"), 0xFFFFFFFF }, //Scheduling class value pair; <parameter class (IS)> ^ <parameter value (IS)>
	{ _T("TQ"), 0xFFFFFFFF } //Timing/quantity; <quantity (CQ)> ^ <interval (*)> ^ <duration (*)> ^ <start date/time (TS)> ^ <enddate/time (TS)> ^ <priority (ID)> ^ <condition (ST)> ^ <text (TX)> ^ <conjunction (ID)> ^ <order sequencing (*)>
};

const hl7_item_t HL7_ITEMS[] = {
	{ _T("CK1000"), _T("ST"), _T("MessageType") },
	{ _T("CK1010"), _T("ST"), _T("MessageControlID") },
	{ _T("CK1020"), _T("ST"), _T("ProcessingID") },
	{ _T("CK1030"), _T("ST"), _T("VersionID") },
	{ _T("CP1040"), _T("ST"), _T("DateTimeOfMessage") },
	{ _T("CK1050"), _T("ST"), _T("FieldSeparator") },
	{ _T("CK1060"), _T("ST"), _T("EncodingCharacters") },
	{ _T("CP1070"), _T("ST"), _T("SendingApplication") },
	{ _T("CP1080"), _T("ST"), _T("SendingFacility") },
	{ _T("CP1090"), _T("ST"), _T("ReceivingApplication") },
	{ _T("CP1100"), _T("ST"), _T("ReceivingFacility") },
	{ _T("CK1110"), _T("ST"), _T("Security") },
	{ _T("CK1120"), _T("ST"), _T("SequenceNumber") },
	{ _T("CK1130"), _T("ST"), _T("ContinuationPointer") },
	{ _T("CK1140"), _T("ST"), _T("AcknowledgmentCode") },
	{ _T("CK1150"), _T("ST"), _T("AcceptAcknowledgmentType") },
	{ _T("CK1160"), _T("ST"), _T("ApplicationAcknowledgmentType") },
	{ _T("CK1170"), _T("ST"), _T("DelayedAcknowledgmentType") },
	{ _T("CK1180"), _T("ST"), _T("CountryCode") },
	{ _T("CK1190"), _T("ST"), _T("CharacterSet") },
	{ _T("CK1200"), _T("ST"), _T("PrincipalLanguageOfMessage") },
	{ _T("CK1210"), _T("ST"), _T("AlternateCharacterSetHandlingScheme") },
	{ _T("CK1220"), _T("ST"), _T("ErrorMessage") },
	{ _T("CP1220"), _T("ST"), _T("ErrorCondition") }
};


static int _find_item_index(tchar_t tag[HL7_IID_SIZE], int l, int h)
{
	int rt, i;

	i = (l + h) / 2;

	if (i < l || i > h)
		return -1;

	rt = xsncmp(tag, HL7_ITEMS[i].tag, HL7_IID_SIZE);
	if (rt < 0)
		return _find_item_index(tag, l, i - 1);
	else if (rt > 0)
		return _find_item_index(tag, i + 1, h);
	else
		return i;
}

static int find_item_index(tchar_t tag[HL7_IID_SIZE])
{
	int n;

	n = sizeof(HL7_ITEMS) / sizeof(hl7_item_t);

	return _find_item_index(tag, 0, n - 1);
}

/***********************************************************************************/
bool_t hl7_is_sequence(tchar_t vr[HL7_VR_SIZE])
{
	int n = sizeof(HL7_VR) / sizeof(hl7_vr_t);

	for (int i = 0; i < n; i++)
	{
		if (xsncmp(vr, HL7_VR[i].vr, xslen(HL7_VR[i].vr)) == 0)
			return (HL7_VR[i].vl == 0xFFFFFFFF)? 1 : 0;
	}

	return 0;
}

dword_t hl7_vr_size(const tchar_t* pvr)
{
	int i, n;

	n = sizeof(HL7_VR) / sizeof(hl7_vr_t);
	for (i = 0; i < n; i++)
	{
		if (xscmp(pvr, HL7_VR[i].vr) == 0)
			return HL7_VR[i].vl;
	}
	return 0;
}

const hl7_item_t* hl7_item_info(tchar_t tag[HL7_IID_SIZE])
{
	int i;

	i = find_item_index(tag);

	if (i < 0)
		return NULL;
	else
		return &(HL7_ITEMS[i]);
}



