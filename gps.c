const char NmeaDataTypeGPGGA[] = "GPGGA";
const char NmeaDataTypeGPGSA[] = "GPGSA";
const char NmeaDataTypeGPGSV[] = "GPGSV";
const char NmeaDataTypeGPRMC[] = "GPRMC";

typedef struct
{
    char NmeaDataType[6];
    char NmeaUtcTime[11];
    char NmeaDataStatus[2];
    char NmeaLatitude[10];
    char NmeaLatitudePole[2];
    char NmeaLongitude[11];
    char NmeaLongitudePole[2];
    char NmeaFixQuality[2];
    char NmeaSatelliteTracked[3];
    char NmeaHorizontalDilution[6];
    char NmeaAltitude[8];
    char NmeaAltitudeUnit[2];
    char NmeaHeightGeoid[8];
    char NmeaHeightGeoidUnit[2];
    char NmeaSpeed[8];
    char NmeaDetectionAngle[8];
    char NmeaDate[8];
}tNmeaGpsData;

tNmeaGpsData NmeaGpsData;

static double Latitude = 0;
static double Longitude = 0;

bool Funcao_D( void )
{
    if( strncmp( ( const char* )NmeaGpsData.NmeaDataType, ( const char* )NmeaDataTypeGPGGA, 5 ) == 0 )
    {
        return ( NmeaGpsData.NmeaFixQuality[0] > 0x30 ) ? true : false;
    }
    else if ( strncmp( ( const char* )NmeaGpsData.NmeaDataType, ( const char* )NmeaDataTypeGPRMC, 5 ) == 0 )
    {
        return ( NmeaGpsData.NmeaDataStatus[0] == 0x41 ) ? true : false;
    }
    else
    {
        return false;
    }
}

int32_t Funcao_A( int8_t *nmeaStr, int32_t nmeaStrSize, int8_t * checksum )
{
    int i = 0;
    uint8_t checkNum = 0;

    // Check input parameters
    if( ( nmeaStr == NULL ) || ( checksum == NULL ) || ( nmeaStrSize <= 1 ) )
    {
        return -1;
    }

    if( nmeaStr[i] == '$' )
    {
        i += 1;
    }

    while( nmeaStr[i] != '*' )
    {
        checkNum ^= nmeaStr[i];
        i += 1;
        if( i >= nmeaStrSize )
        {
            return -1;
        }
    }

    checksum[0] = Nibble2HexChar( checkNum / 16 ); // upper nibble
    checksum[1] = Nibble2HexChar( checkNum % 16 ); // lower nibble

    return i + 1;
}

static bool Funcao_B( int8_t *serialBuff, int32_t buffSize )
{
    int32_t checksumIndex;
    int8_t checksum[2];

    checksumIndex = Funcao_A( serialBuff, buffSize, checksum );

    if( checksumIndex < 0 )
    {
        return false;
    }

    if( checksumIndex >= ( buffSize - 2 ) )
    {
        return false;
    }

    if( ( serialBuff[checksumIndex] == checksum[0] ) && 
        ( serialBuff[checksumIndex + 1] == checksum[1] ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t Funcao_C( int8_t *rxBuffer, int32_t rxBufferSize )
{
    uint8_t i = 1;
    uint8_t j = 0;
    uint8_t fieldSize = 0;
    
    if( rxBuffer[0] != '$' )
    {
        return FAIL;
    }

    if( Funcao_B( rxBuffer, rxBufferSize ) == false )
    {
        return FAIL;
    }

    fieldSize = 0;
    while( rxBuffer[i + fieldSize++] != ',' )
    {
        if( fieldSize > 6 )
        {
            return FAIL;
        }
    }
    for( j = 0; j < fieldSize; j++, i++ )
    {
        NmeaGpsData.NmeaDataType[j] = rxBuffer[i];
    }

    if( strncmp( ( const char* )NmeaGpsData.NmeaDataType, ( const char* )NmeaDataTypeGPGGA, 5 ) == 0 )
    {  
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 11 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaUtcTime[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 10 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaLatitude[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 2 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaLatitudePole[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 11 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaLongitude[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 2 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaLongitudePole[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 2 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaFixQuality[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 3 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaSatelliteTracked[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 6 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaHorizontalDilution[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 8 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaAltitude[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 2 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaAltitudeUnit[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 8 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaHeightGeoid[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 2 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaHeightGeoidUnit[j] = rxBuffer[i];
        }

        return SUCCESS;
    }
    else if ( strncmp( ( const char* )NmeaGpsData.NmeaDataType, ( const char* )NmeaDataTypeGPRMC, 5 ) == 0 )
    {    
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 11 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaUtcTime[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 2 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaDataStatus[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 10 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaLatitude[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 2 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaLatitudePole[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 11 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaLongitude[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 2 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaLongitudePole[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 8 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaSpeed[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 8 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaDetectionAngle[j] = rxBuffer[i];
        }
        fieldSize = 0;
        while( rxBuffer[i + fieldSize++] != ',' )
        {
            if( fieldSize > 8 )
            {
                return FAIL;
            }
        }
        for( j = 0; j < fieldSize; j++, i++ )
        {
            NmeaGpsData.NmeaDate[j] = rxBuffer[i];
        }

        return SUCCESS;
    }
    else
    {
        return FAIL;
    }
}
