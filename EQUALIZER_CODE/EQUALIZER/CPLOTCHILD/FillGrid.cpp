///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CPLOTCHILD::FillGrid( void ){ return FillGrid(TRUE);}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	CPLOTCHILD::FillGrid( BOOL OptRepint ){
	int W_Grid = WFR;
	int H_Grid = HFR;
	int WHPos = 0;
	int yT = 0;
	int IndDivY = 0;
	for( int y=0; y<H_Grid; y++ ){
		int IndDivX = 0;
		yT = y*W_Grid;
		for( int x=0; x<W_Grid; x++ ){
		    WHPos = x + yT;
            RgbBuf[WHPos] = ColorBack;
			if( (IndDivY*H_Grid)/10==y ){
				RgbBuf[WHPos] = ColorLineG;
			}else{
                if( (IndDivX*W_Grid)/10==x ){
                    RgbBuf[WHPos] = ColorBack;
                    RgbBuf[WHPos] = ColorLineG;
                    IndDivX++;
                }else{
                    if( x==W_Grid-1 || y==H_Grid-1 ){
                        RgbBuf[WHPos] = ColorLineG;
                    }
                }
			}
		}
		if ((IndDivY*H_Grid)/10==y) {
		   IndDivY++;
		}
	}
    int AdvGrid = 0;
    int* ITB = (int*)&ColorLineG;
    int IVal = ITB[0];
    IVal++;
    IVal--;
    ITB = (int*)&RgbBuf[0];
    for( int y=W_Grid/100; y<H_Grid-W_Grid/100; y = y + 1 ){
        yT = y*W_Grid;
        for( int x=W_Grid/100; x<W_Grid-W_Grid/100; x = x + 1 ){
            WHPos = x + yT;
            //if( ITB[WHPos]==IVal ){
                AdvGrid++;
                RgbBuf[WHPos] = ColorBack;
            //}
        }
    }
           /*/
        ELSE
           FOR int y=W_Grid/100; y<H_Grid-W_Grid/100; y = y + 1 LOOP
               FOR int x=W_Grid/100; x<W_Grid-W_Grid/100; x = x + 1 LOOP
                   int FromIndex = x + W_Grid*y;
                   RGB_BUFF[FromIndex] = ColorBack;
               ENDLOOP;
           ENDLOOP;
        ENDIF;
        //*/
        return TRUE;
}

