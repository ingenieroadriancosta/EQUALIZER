BOOL CPLOTCHILD::plot( unsigned char* UCData, int NPos ){
    if( InSL ){ return FALSE; }
    InPlot = TRUE;
    FreeBuff();
    GNPos = NPos;
    UCBuff = (float*)malloc( (NPos + 1024) * sizeof(float) );
    MaxVal = -1;
    MinVal = 100000;
    for( int i=0; i<NPos; i++ ){
        MaxVal = max( MaxVal, UCData[i] );
        MinVal = min( MinVal, UCData[i] );
        UCBuff[i] = ( 255 - UCData[i] )/255.0;
    }
    // memcpy( &UCBuff[0], &UCData[0], NPos );
    return plotPUC();
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
BOOL CPLOTCHILD::plot( char* UCData, int NPos ){
    if( InSL ){ return FALSE; }
    InPlot = TRUE;
    FreeBuff();
    GNPos = NPos;
    UCBuff = (float*)malloc( (NPos + 1024) * sizeof(float) );
    MaxVal = -1;
    MinVal = 100000;
    for( int i=0; i<NPos; i++ ){
        MaxVal = max( MaxVal, UCData[i] );
        MinVal = min( MinVal, UCData[i] );
        if( UCData[i]<-127 ){
            UCBuff[i] = 1;
        }else{
            UCBuff[i] = ( 127 - UCData[i] )/255.0;
        }
    }
    // memcpy( &UCBuff[0], &UCData[0], NPos );
    return plotPUC();
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
BOOL CPLOTCHILD::plot( unsigned short* UCData, int NPos ){
    if( InSL ){ return FALSE; }
    InPlot = TRUE;
    FreeBuff();
    GNPos = NPos;
    UCBuff = (float*)malloc( (NPos + 1024) * sizeof(float) );
    MaxVal = -1;
    MinVal = 100000;
    for( int i=0; i<NPos; i++ ){
        MaxVal = max( MaxVal, abs(UCData[i]) );
        MinVal = min( MinVal, abs(UCData[i]) );
        UCBuff[i] = ( 65535 - UCData[i] )/65535.0;
    }
    return plotPUC();
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
BOOL CPLOTCHILD::plot( short* UCData, int NPos ){
    if( InSL ){ return FALSE; }
    InPlot = TRUE;
    FreeBuff();
    GNPos = NPos;
    UCBuff = (float*)malloc( (NPos + 1024) * sizeof(float) );
    MaxVal = -1;
    MinVal = 100000;
    for( int i=0; i<NPos; i++ ){
        MaxVal = max( MaxVal, (UCData[i]) );
        MaxValabs = max( MaxValabs, abs(UCData[i]) );
        MinVal = min( MinVal, (UCData[i]) );
        MinValabs = min( MinValabs, abs(UCData[i]) );
        UCBuff[i] = ( 32767 - UCData[i] )/65535.0;
    }
    // static char T2T[128];sprintf( T2T, "(%g, %g)  (%g, %g)", MaxVal, MinVal, MaxValabs, MinValabs );PrintInfoMm( T2T );
    return plotPUC();
}
/// //////////////////////////////////////////////////////////////////////////////////////////////
BOOL CPLOTCHILD::plotPUC( void ){
    FillGrid();
    int W_Grid = WFR - 1;
	int H_Grid = HFR - 1;
	int x = 0;
	int y = 0;
	int Nx = 0;
	int Ny = 0;
	int YW = 0;
	const int GNPosC = GNPos - 1;
	if( RgbBuf==NULL ){
        exit( 0 );
	}
	for( int Index=0; Index<GNPos; Index++ ){
        x = (int)( ((long long)Index * (long long)W_Grid)/GNPosC );
		y = (H_Grid * UCBuff[Index]);
		YW = y * WFR;
		RgbBuf[ x + YW ] = ColorLine;
		if( Index==GNPosC ){
            continue;
		}
        Nx = (int)( ( (long long)(Index+1) * (long long)W_Grid )/GNPosC );
        Ny = (H_Grid*UCBuff[(Index+1)]);
        while( x!=Nx && x<W_Grid ){
            x++;
            RgbBuf[ x + YW ] = ColorLine;
        }
        //
        while( y>Ny ){
            y--;
            if( y<0 ){
                continue;
            }
            RgbBuf[ x + y * WFR ] = ColorLine;
        }
        //
        while( y!=Ny && y<H_Grid ){
            y++;
            if( y>=H_Grid ){
                continue;
            }
            RgbBuf[ x + y * WFR ] = ColorLine;
        }
        //
	}
	// toc;sprintf( TGBL, "%f\n", EndToc );PrintInfo( TGBL );
    SetBitmapBits( HBitmap, 4 * WFR * HFR, &RgbBuf[0] );
    if( DoubleBuffP ){
        HDC OHDC = GetDC( HWPlot );
        HDC NHDC = CreateCompatibleDC( OHDC );
        SelectObject( NHDC, HBitmap );
        BitBlt( OHDC, 0, 0, WFR, HFR, NHDC, 0, 0, SRCCOPY );
        ReleaseDC( HWPlot, OHDC );
        DeleteDC( NHDC );
    }else{
        SendMessage( HWPlot, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBitmap );
    }
    InPlot = FALSE;
    return TRUE;
}
