#define FALSE		0
#define TRUE		1
#define NUM_WORDS 	((uint8_t)20U)
#define NUM_CHRS	((uint8_t)15U)

typedef struct
{
	uint8_t		m_bFlagRead;
	uint8_t		m_bFlagDataReady;
	int8_t		tmp_words[NUM_WORDS][NUM_CHRS];
	int8_t		tmp_szChecksum[NUM_CHRS];

	uint8_t		m_bFlagComputedCks;
	uint8_t		m_bFlagReceivedCks;
	int16_t		m_nChecksum;
	int16_t		index_received_checksum ;

	int16_t		m_nWordIdx;
	int16_t		m_nPrevIdx;
	int16_t		m_nNowIdx;

	float		res_fSpeed;
	float		res_fBearing;
	uint8_t		res_nUTCDay;
	uint8_t 	res_nUTCMonth;
	uint8_t		res_nUTCYear;
	uint8_t		res_nUTCHour;
	uint8_t 	res_nUTCMin;
	uint8_t 	res_nUTCSec;
	int16_t		res_nSatellitesUsed;
	float		res_fAltitude;
	float		res_fLongitude;
	float		res_fLatitude;
} NMEA_Sentence_Type_t;

NMEA_Sentence_Type_t NMEA_Sentence;
 
int16_t Funcao_A(int8_t chr)
{
	if(chr == '$')
	{
		NMEA_Sentence.m_bFlagRead = TRUE;
		NMEA_Sentence.m_bFlagComputedCks = FALSE;
		NMEA_Sentence.m_nChecksum = 0;
		NMEA_Sentence.m_bFlagReceivedCks = FALSE;
		NMEA_Sentence.index_received_checksum = 0;
		NMEA_Sentence.m_nWordIdx = 0;
		NMEA_Sentence.m_nPrevIdx = 0;
		NMEA_Sentence.m_nNowIdx = 0;
	}
	if(NMEA_Sentence.m_bFlagRead)
	{
		if(chr == '\r' || chr == '\n')
		{
			NMEA_Sentence.tmp_words[NMEA_Sentence.m_nWordIdx][NMEA_Sentence.m_nNowIdx - NMEA_Sentence.m_nPrevIdx] = 0;
			NMEA_Sentence.m_nWordIdx++;
			NMEA_Sentence.tmp_szChecksum[NMEA_Sentence.index_received_checksum] = 0;
			NMEA_Sentence.m_bFlagRead = FALSE;

			Funcao_B();
		}
		else
		{
			if((NMEA_Sentence.m_bFlagComputedCks && (chr == '*')))
			{
				NMEA_Sentence.m_bFlagComputedCks = FALSE;
			}
			if(NMEA_Sentence.m_bFlagComputedCks)
			{
				NMEA_Sentence.m_nChecksum ^= chr;
			}
			if(chr == '$')
			{
				NMEA_Sentence.m_bFlagComputedCks = TRUE;
			}
			if(NMEA_Sentence.m_bFlagReceivedCks)
			{
				NMEA_Sentence.tmp_szChecksum[NMEA_Sentence.index_received_checksum] = chr;
				NMEA_Sentence.index_received_checksum++;
			}
			if(chr == '*')
			{
				NMEA_Sentence.m_bFlagReceivedCks = TRUE;
			}
			NMEA_Sentence.tmp_words[NMEA_Sentence.m_nWordIdx][NMEA_Sentence.m_nNowIdx - NMEA_Sentence.m_nPrevIdx] = chr;
			if(chr == ',')
			{
				NMEA_Sentence.tmp_words[NMEA_Sentence.m_nWordIdx][NMEA_Sentence.m_nNowIdx - NMEA_Sentence.m_nPrevIdx] = 0;
				NMEA_Sentence.m_nWordIdx++;
				NMEA_Sentence.m_nPrevIdx = NMEA_Sentence.m_nNowIdx;
			}
			else
			{
				NMEA_Sentence.m_nNowIdx++;
			}
		}
	}
	return NMEA_Sentence.m_nWordIdx;
}

void Funcao_B(void)
{
	float degrees = 0.0f;
	float minutes = 0.0f;
	int16_t received_cks = 16 * digit2dec(NMEA_Sentence.tmp_szChecksum[0]) + digit2dec(NMEA_Sentence.tmp_szChecksum[1]);
	if(NMEA_Sentence.m_nChecksum != received_cks)
	{
		return;
	}
	if(mstrcmp(NMEA_Sentence.tmp_words[0],(int8_t *)(&"$GNGGA")) == 0)
	{
		if(NMEA_Sentence.tmp_words[6][0] == '0')
		{
			NMEA_Sentence.res_fLatitude = 0.0f;
			NMEA_Sentence.res_fLongitude = 0.0f;
			NMEA_Sentence.m_bFlagDataReady = FALSE;
			return;
		}
		NMEA_Sentence.res_nUTCHour = digit2dec(NMEA_Sentence.tmp_words[1][0]) * 10 + digit2dec(NMEA_Sentence.tmp_words[1][1]);
		NMEA_Sentence.res_nUTCMin = digit2dec(NMEA_Sentence.tmp_words[1][2]) * 10 + digit2dec(NMEA_Sentence.tmp_words[1][3]);
		NMEA_Sentence.res_nUTCSec = digit2dec(NMEA_Sentence.tmp_words[1][4]) * 10 + digit2dec(NMEA_Sentence.tmp_words[1][5]);
		NMEA_Sentence.res_fLatitude = string2float(NMEA_Sentence.tmp_words[2]);
		NMEA_Sentence.res_fLongitude = string2float(NMEA_Sentence.tmp_words[4]);
		if (NMEA_Sentence.tmp_words[3][0] == 'S')
		{
			NMEA_Sentence.res_fLatitude  *= -1.0f;
		}
		if (NMEA_Sentence.tmp_words[5][0] == 'W')
		{
			NMEA_Sentence.res_fLongitude *= -1.0f;
		}
		degrees = truncf(NMEA_Sentence.res_fLatitude / 100.0f);
		minutes = NMEA_Sentence.res_fLatitude - (degrees * 100.0f);
		NMEA_Sentence.res_fLatitude = degrees + minutes / 60.0f;

		degrees = trunc(NMEA_Sentence.res_fLongitude / 100.0f);
		minutes = NMEA_Sentence.res_fLongitude - (degrees * 100.0f);
		NMEA_Sentence.res_fLongitude = degrees + minutes / 60.0f;

		NMEA_Sentence.res_nSatellitesUsed = (int16_t) string2float(NMEA_Sentence.tmp_words[7]);
		NMEA_Sentence.res_fAltitude = string2float(NMEA_Sentence.tmp_words[9]);
		NMEA_Sentence.m_bFlagDataReady = TRUE;
	}

	if (mstrcmp(NMEA_Sentence.tmp_words[0],(int8_t *)(&"$GNRMC")) == 0)
	{
		if (NMEA_Sentence.tmp_words[2][0] == 'V')
		{
			NMEA_Sentence.res_fLatitude = 0.0f;
			NMEA_Sentence.res_fLongitude = 0.0f;
			NMEA_Sentence.m_bFlagDataReady = FALSE;
			return;
		}
		NMEA_Sentence.res_nUTCHour = digit2dec(NMEA_Sentence.tmp_words[1][0]) * 10 + digit2dec(NMEA_Sentence.tmp_words[1][1]);
		NMEA_Sentence.res_nUTCMin = digit2dec(NMEA_Sentence.tmp_words[1][2]) * 10 + digit2dec(NMEA_Sentence.tmp_words[1][3]);
		NMEA_Sentence.res_nUTCSec = digit2dec(NMEA_Sentence.tmp_words[1][4]) * 10 + digit2dec(NMEA_Sentence.tmp_words[1][5]);
		NMEA_Sentence.res_fLatitude = string2float(NMEA_Sentence.tmp_words[3]);
		NMEA_Sentence.res_fLongitude = string2float(NMEA_Sentence.tmp_words[5]);
		if (NMEA_Sentence.tmp_words[4][0] == 'S')
		{
			NMEA_Sentence.res_fLatitude  *= -1.0f;
		}
		if (NMEA_Sentence.tmp_words[6][0] == 'W')
		{
			NMEA_Sentence.res_fLongitude *= -1.0f;
		}
		degrees = trunc(NMEA_Sentence.res_fLatitude / 100.0f);
		minutes = NMEA_Sentence.res_fLatitude - (degrees * 100.0f);
		NMEA_Sentence.res_fLatitude = degrees + minutes / 60.0f;

		degrees = trunc(NMEA_Sentence.res_fLongitude / 100.0f);
		minutes = NMEA_Sentence.res_fLongitude - (degrees * 100.0f);
		NMEA_Sentence.res_fLongitude = degrees + minutes / 60.0f;

		NMEA_Sentence.res_fSpeed = string2float(NMEA_Sentence.tmp_words[7]);
		NMEA_Sentence.res_fSpeed /= 1.852;
		NMEA_Sentence.res_fBearing = string2float(NMEA_Sentence.tmp_words[8]);
		NMEA_Sentence.res_nUTCDay = digit2dec(NMEA_Sentence.tmp_words[9][0]) * 10 + digit2dec(NMEA_Sentence.tmp_words[9][1]);
		NMEA_Sentence.res_nUTCMonth = digit2dec(NMEA_Sentence.tmp_words[9][2]) * 10 + digit2dec(NMEA_Sentence.tmp_words[9][3]);
		NMEA_Sentence.res_nUTCYear = digit2dec(NMEA_Sentence.tmp_words[9][4]) * 10 + digit2dec(NMEA_Sentence.tmp_words[9][5]);
		NMEA_Sentence.m_bFlagDataReady = TRUE;
	}
}

int16_t digit2dec(int8_t digit) {
	
	if (digit >= 65)
	{
		return digit - 55;
	}
	else
	{
		return digit - 48;
	}
}

float string2float(int8_t *s)
{
	
	int32_t  integer_part = 0;
	float decimal_part = 0.0f;
	float decimal_pivot = 0.1f;
	uint8_t isdecimal = FALSE;
	uint8_t isnegative = FALSE;

	int8_t c;
	while (*s++)
	{
		c = *s;
		// skip special/sign chars
		if (c == '-')
		{
			isnegative = TRUE;
			continue;
		}
		if (c == '+')
		{
			continue;
		}
		if (c == '.')
		{ 
			isdecimal = TRUE;
			continue;
		}
		if (!isdecimal)
		{
			integer_part = (10 * integer_part) + (c - 48);
		}
		else
		{
			decimal_part += decimal_pivot * (float)(c - 48);
			decimal_pivot /= 10.0f;
		}
	}
	// add integer part
	decimal_part += (float) integer_part;

	// check negative
	if (isnegative)
	{
		decimal_part = - decimal_part;
	}
	return decimal_part;
}

int16_t mstrcmp(const int8_t *s1, const int8_t *s2)
{
	while((*s1 && *s2) && (*s1 == *s2))
	s1++,s2++;
	return (*s1 - *s2);
}
