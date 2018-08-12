#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


//---------------------------------------------------------------------------//
void main(int argc, char* argv[])
{
	HANDLE  hSerial;
	char    aPortName[16] = "\\\\.\\COM3";
	FILE   *pFile = NULL;

	printf("YM2149 Player (v.1)\n");

	if( argc==3 || argc==2 )
	{
		if( argc==3 )
		{
			memcpy(aPortName+4, argv[2], 5);
		}
		hSerial = CreateFile( aPortName, GENERIC_READ | GENERIC_WRITE, 0, 0,
							  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

		if( hSerial==INVALID_HANDLE_VALUE )
		{
			printf("Error: cannot open COM port: %s\n", aPortName);
		}
		else
		{
			// Настраиваем СОМ-порт 256kbps 8-N-1
			DWORD dwMask = 0;
			DCB dcbSerialParams = { 0 };
			dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
			if( GetCommState(hSerial, &dcbSerialParams) )
			{
				dcbSerialParams.BaudRate     = 19200;
				dcbSerialParams.ByteSize     = 8;
				dcbSerialParams.StopBits     = ONESTOPBIT;
				dcbSerialParams.Parity       = NOPARITY;
				SetCommState( hSerial, &dcbSerialParams );
				PurgeComm( hSerial, PURGE_RXCLEAR|PURGE_TXCLEAR );
			}

			pFile = fopen(argv[1], "rb");
		
			if( pFile )
			{
				BYTE aBuffer[16];
				BYTE wOffset = 0;
				DWORD nFrames = 0;
				WORD  wSignature = 0;
				DWORD nBytes, dwBytesIO;

				// find data bytes
				fread(&wSignature, 1, sizeof(wSignature), pFile);
				if( wSignature==0x4D59 )
				{
					printf("\tYM5,6 found\n");
					fseek(pFile, 10, SEEK_CUR);
					fread(&nFrames, 1, sizeof(nFrames), pFile);
					nFrames = _byteswap_ulong(nFrames);
					printf("\tnFrames=%d\n", nFrames);
					if( nFrames > 10000 ) nFrames = 10000;
					fseek(pFile, 0x50, SEEK_SET);
					
					nFrames /= 4;
				}
				else
				{
					printf("\tYM5,6 NOT found\n");
				exit(0);
				}
				
				fseek(pFile, wOffset, SEEK_SET);

				while( nFrames-- > 0 )
				{
					nBytes = fread(aBuffer, 1, sizeof(aBuffer), pFile);
					if( nBytes == 0 ) 
					{
						printf("\tEnd\n");
						break;
					}
					else
					{
						Sleep(19);
						WriteFile( hSerial, aBuffer, sizeof(aBuffer), &dwBytesIO, NULL );
					}
				}
				fclose(pFile);
			}
			else
			{
				printf("\tFailed to open file: %s\n", argv[1]);
			}
			CloseHandle(hSerial);
		}
	}
	else
	{
		printf("\tUsage: %s file.asc\n");
	}
}

